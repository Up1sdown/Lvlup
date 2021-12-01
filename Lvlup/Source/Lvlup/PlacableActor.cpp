// Fill out your copyright notice in the Description page of Project Settings.


#include "PlacableActor.h"
#include "LvlupGameMode.h"
#include <Kismet/GameplayStatics.h>


// Sets default values
APlacableActor::APlacableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	//RootComponent = StaticMeshComp;

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
			GameMode->SpawnedActors.RemoveSingle(this);
		}
	}
}

