// Fill out your copyright notice in the Description page of Project Settings.


#include "PlacableActor.h"
#include "LvlupGameMode.h"
#include "LvlupProjectile.h"
#include <Kismet/GameplayStatics.h>



bool APlacableActor::bIsTooClose()
{
	float DistanceToPlayer = FVector::Dist(GetActorLocation(), PlayerActor->GetActorLocation());
	return DistanceToPlayer < AcceptableDistance;	
}


// Sets default values
APlacableActor::APlacableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	StaticMeshComp->OnComponentHit.AddDynamic(this, &APlacableActor::OnHit);
	//RootComponent = StaticMeshComp;

}

// Called when the game starts or when spawned
void APlacableActor::BeginPlay()
{
	Super::BeginPlay();
	
	ScaleStep = (MaxScale - GetActorScale().X) / NumberOfSteps;
	TimeStep = GetLifeSpan() / NumberOfSteps;
	
}


void APlacableActor::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//Destroy Actor if hit by projectile
	if ((OtherActor != nullptr) && (OtherComp != nullptr) && Cast<ALvlupProjectile>(OtherActor))
	{			
		Destroy();
		ALvlupGameMode* GameMode = Cast<ALvlupGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (GameMode)
		{
			GameMode->AddToScore(GetActorScale().X);
		}
	}
}

// Called every frame
void APlacableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsTooClose())
	{
		Destroy();
	}

	
	TempTime += DeltaTime;

	if (GetActorScale().X < MaxScale)
	{
		if (TimeStep < TempTime)
		{
			TempTime -= TimeStep;
			SetActorScale3D(GetActorScale() + FVector(ScaleStep, ScaleStep, ScaleStep));
		}
	}

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

	Super::Destroyed();
}

