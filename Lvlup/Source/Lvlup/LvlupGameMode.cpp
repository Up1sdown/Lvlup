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
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	//DefaultPawnClass = PlayerPawnClassFinder.Class;	

	bSpawningEnabled = false;

	// use our custom HUD class
	HUDClass = ALvlupHUD::StaticClass();

	PrimaryActorTick.bCanEverTick = true;

}

ATriggerBox* ALvlupGameMode::FindTriggerBoxByTag(FName TagToFind)
{
	TArray<AActor*> FoundTriggerBoxes;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATriggerBox::StaticClass(), FoundTriggerBoxes);
	for (AActor* FoundTriggerBox : FoundTriggerBoxes)
	{
		ATriggerBox* TriggerBoxReference = Cast<ATriggerBox>(FoundTriggerBox);
		if (TriggerBoxReference)
		{
			for (auto Tag : TriggerBoxReference->Tags)
			{
				if (Tag == TagToFind)
				{
					return TriggerBoxReference;					
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
	TriggerBoxLocation = TriggerBox->GetActorLocation();
	Extent = TriggerBox->GetComponentsBoundingBox().GetExtent();	

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


void ALvlupGameMode::EnableSpawning()
{
	bSpawningEnabled = true;
}

void ALvlupGameMode::AddToScore(float TargetScale)
{
	if (PlayerCharacter)
	{		
		PlayerCharacter->PlayerScore += (1.01f - TargetScale) * 10;
		//GEngine->AddOnScreenDebugMessage(-1,10,FColor::Red, FString::SanitizeFloat(PlayerCharacter->PlayerScore));
	}
}

void ALvlupGameMode::DisableSpawning()
{
	bSpawningEnabled = false;
}

uint8 ALvlupGameMode::GetNumberOfActors()
{
	return SpawnedActors.Num();
}

uint8 ALvlupGameMode::GetMaxNumberOfTargets()
{
	return MaxSphere;
}

bool ALvlupGameMode::bIsEnoughSpace(FVector NewLocation)
{	
	for (auto SpawnedActor : SpawnedActors)
	{		
		if (FVector::Dist(NewLocation, SpawnedActor->GetActorLocation()) < DistanceBetweenActors)
		{
			return false;
		}
	}
	return true;
}


bool ALvlupGameMode::bIsTooClose(FVector NewLocation)
{
	float DistanceToPlayer = FVector::Dist(NewLocation, PlayerCharacter->GetActorLocation());
	return DistanceToPlayer < UnacceptableDistance;	
}

void ALvlupGameMode::SpawnActor()
{
	if (TriggerBox)
	{
		//Check for a valid World:
		UWorld* const World = GetWorld();

		if (World)
		{
			//Set the spawn parameters
			FActorSpawnParameters SpawnParams;					
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;

			//Get a random location
			FVector SpawnLocation = GetRandLocationInTriggerBox();
			FTransform SpawnTransform = FTransform(SpawnLocation);
			
			if (bIsEnoughSpace(SpawnLocation))
			{
				//APlacableActor* SpawnedPlacableActor = World->SpawnActor<APlacableActor>(ActorToSpawn, SpawnLocation, SpawnRotation, SpawnParams);
				APlacableActor* SpawnedPlacableActor = World->SpawnActorDeferred<APlacableActor>(ActorToSpawn, SpawnTransform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding); // DontSpawnIfColliding
				if (IsValid(SpawnedPlacableActor))
				{
					SpawnedPlacableActor->SetLifeSpan(FMath::FRandRange(LifespanMin, LifespanMax));					
					SpawnedActors.Add(SpawnedPlacableActor);
					SpawnedPlacableActor->PlayerActor = PlayerCharacter;
					SpawnedPlacableActor->MaxScale = FinalScale;
					SpawnedPlacableActor->AcceptableDistance = UnacceptableDistance;
					SpawnedPlacableActor->NumberOfSteps = NumberOfSteps;
					SpawnTransform.SetScale3D(FVector(InitialScale, InitialScale, InitialScale));					
					SpawnedPlacableActor->FinishSpawning(SpawnTransform);					
				}				
			}					
		}
	}
}

void ALvlupGameMode::BeginPlay()
{
	Super::BeginPlay();		
	
	TriggerBox = FindTriggerBoxByTag(SearchTag);
	PlayerCharacter = Cast<ALvlupCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void ALvlupGameMode::Tick(float DeltaSeconds)
{
	if (!bSpawningEnabled)
	{
		return;
	}

	if (SpawnedActors.Num() < MaxSphere)
	{
		SpawnActor();
		//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::FromInt(SpawnedActors.Num()));
	}	
}

