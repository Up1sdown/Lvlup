// Copyright Epic Games, Inc. All Rights Reserved.

#include "LvlupGameMode.h"
#include "LvlupHUD.h"
#include "LvlupCharacter.h"
#include "PlacableActor.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/TriggerBox.h"
#include "Math/Vector.h"

ALvlupGameMode::ALvlupGameMode()
	: Super()
{

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	_CurrentSphere = 0;

	// use our custom HUD class
	HUDClass = ALvlupHUD::StaticClass();

	PrimaryActorTick.bCanEverTick = true;

}

ATriggerBox* ALvlupGameMode::FindTriggerBoxByTag(FName Tag)
{
	TArray<AActor*> FoundTriggerBox;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATriggerBox::StaticClass(), FoundTriggerBox);
	for (auto TempTriggerBox : FoundTriggerBox)
	{
		ATriggerBox* TriggerBox = Cast<ATriggerBox>(TempTriggerBox);
		if (TriggerBox)
		{
			for (auto TempTag : TriggerBox->Tags)
			{
				if (TempTag == Tag)
				{
					//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("TriggerBox found"));
					return TriggerBox;					
				}
			}
		}
	}
	return nullptr;
}

FVector ALvlupGameMode::GetRandLocationInTriggerBox()
{	
	FVector RandLocation;
	float MinX, MinY, MinZ;
	float MaxX, MaxY, MaxZ;

	FVector TriggerBoxLocation;
	FVector Extent;

	//Get TriggerBox location and extent
	TriggerBoxLocation = _TriggerBox->GetActorLocation();
	Extent = _TriggerBox->GetComponentsBoundingBox().GetExtent();	

	//Calculate the minimum  X, Y, and Z
	MinX = TriggerBoxLocation.X - Extent.X;
	MinY = TriggerBoxLocation.Y - Extent.Y;
	MinZ = TriggerBoxLocation.Z - Extent.Z;

	//Calculate the maximum X, Y, and Z
	MaxX = TriggerBoxLocation.X + Extent.X;
	MaxY = TriggerBoxLocation.Y + Extent.Y;
	MaxZ = TriggerBoxLocation.Z + Extent.Z;

	//The random spawn location will fall between the min and max X, Y, and Z
	RandLocation.X = FMath::FRandRange(MinX, MaxX);
	RandLocation.Y = FMath::FRandRange(MinY, MaxY);
	RandLocation.Z = FMath::FRandRange(MinZ, MaxZ);

	return RandLocation;

}

bool ALvlupGameMode::bIsEnoughSpace(FVector NewLocation)
{	
	for (auto Location : SpawnLocations)
	{		
		if (FVector::Dist(NewLocation, Location) < _DistanceBetweenSpawn)
		{
			return false;
		}
	}	
	return true;
}

void ALvlupGameMode::SpawnTarget()
{
	if (_TriggerBox)
	{
		//Check for a valid World:
		UWorld* const World = GetWorld();

		if (World)
		{
			//Set the spawn parameters
			FActorSpawnParameters SpawnParams;		
			//SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;

			//Get a random location
			FVector SpawnLocation = GetRandLocationInTriggerBox();
			FRotator SpawnRotation;
			//FTransform SpawnTransform = FTransform(FQuat(), SpawnLocation, FVector(2, 2, 2));

			if (bIsEnoughSpace(SpawnLocation))
			{
				APlacableActor* SpawnedPlacableActor = World->SpawnActor<APlacableActor>(_PlacableActor, SpawnLocation, SpawnRotation, SpawnParams);
				if (SpawnedPlacableActor)
				{
					SpawnedPlacableActor->SetLifeSpan(FMath::FRandRange(_LifespanMin, _LifespanMax));
					SpawnLocations.Add(SpawnLocation);
					
				}
				_CurrentSphere++;
			}
			//APlacableActor* SpawnedPlacableActor = World->SpawnActorDeferred<APlacableActor>(_PlacableActor, SpawnTransform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding);
			
			
		}
	}
}

void ALvlupGameMode::BeginPlay()
{
	Super::BeginPlay();		
	
	_TriggerBox = FindTriggerBoxByTag(_TagToFind);
}

void ALvlupGameMode::Tick(float DeltaSeconds)
{
	if (_CurrentSphere >= _MaxSphere)
	{
		return;
	}

	SpawnTarget();
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::FromInt(_CurrentSphere));
}

