// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LvlupGameMode.generated.h"

class FName;
class ATriggerBox;
class APlacableActor;
class ALvlupCharacter;

UCLASS(minimalapi, config = GameMode)
class ALvlupGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	ALvlupGameMode();

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY()
	ATriggerBox* TriggerBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "-Misc")
	FName SearchTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "-Misc", meta = (ClampMin = "0", ClampMax = "100"))
	uint8 MaxSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "-Misc", meta = (ClampMin = "0"))
	float DistanceBetweenActors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "-Misc", meta = (ClampMin = "0"))
	float LifespanMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "-Misc", meta = (ClampMin = "0"))
	float LifespanMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "-Misc", meta = (ClampMin = "0"))
	float UnacceptableDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "-Misc", meta = (ClampMin = "0"))
	float InitialScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "-Misc", meta = (ClampMin = "0", ClampMax = "1"))
	float FinalScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "-Misc", meta = (ClampMin = "1"))
	float NumberOfSteps;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "-Misc")
	TSubclassOf<APlacableActor> ActorToSpawn;

	UFUNCTION(BlueprintCallable)
	ATriggerBox* FindTriggerBoxByTag(FName Tag);

	UFUNCTION()
	FVector GetRandLocationInTriggerBox();	

	TArray<APlacableActor*> SpawnedActors;

	UFUNCTION()
	void EnableSpawning();

	UFUNCTION(BlueprintCallable)
	void AddToScore(float PointsToAdd);

	UFUNCTION()
	void DisableSpawning();

	UFUNCTION(BlueprintCallable)
	uint8 GetNumberOfActors();
	
	UFUNCTION(BlueprintCallable)
	uint8 GetMaxNumberOfTargets();

private:	

	bool bIsEnoughSpace(FVector NewLocation);

	bool bIsTooClose(FVector NewLocation);

	void SpawnActor();

	bool bSpawningEnabled;

	UPROPERTY()
	ALvlupCharacter* PlayerCharacter;

};



