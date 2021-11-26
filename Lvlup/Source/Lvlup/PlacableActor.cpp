// Fill out your copyright notice in the Description page of Project Settings.


#include "PlacableActor.h"
#include <Kismet/GameplayStatics.h>
#include <Lvlup/LvlupGameMode.h>

// Sets default values
APlacableActor::APlacableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Static_Mesh");
	RootComponent = Mesh;

}

// Called when the game starts or when spawned
void APlacableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlacableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlacableActor::Destroyed()
{
	UObject* World = GetWorld();
	if (World)
	{
		ALvlupGameMode* GameMode = Cast<ALvlupGameMode>(UGameplayStatics::GetGameMode(World));
		if (GameMode)
		{
			GameMode->_CurrentSphere--;
			GameMode->SpawnLocations.RemoveSingle(this->GetActorLocation());
		}
	}
}

