// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlacableActor.generated.h"

UCLASS()
class LVLUP_API APlacableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlacableActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "-Misc")
	AActor* PlayerActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "-Misc")
	float AcceptableDistance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "-Misc")
	float MaxScale;

	float NumberOfSteps;

	/** Called when get hit */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Destroyed() override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "-Misc")
	UStaticMeshComponent* StaticMeshComp;

private:
	float ScaleStep;
	bool bIsTooClose();

	float TempTime;
	float TimeStep;

	

	

};
