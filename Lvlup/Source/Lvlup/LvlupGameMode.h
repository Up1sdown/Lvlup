// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LvlupGameMode.generated.h"

class FName;
class ATriggerBox;
class APlacableActor;

UCLASS(minimalapi)
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
	ATriggerBox* _TriggerBox;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "-Misc")
	FName _TagToFind;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "-Misc", meta = (ClampMin = "0", ClampMax = "100"))
	uint8 _MaxSphere;

	UPROPERTY(VisibleAnywhere , BlueprintReadOnly, Category = "-Misc")
	uint8 _CurrentSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "-Misc", meta = (ClampMin = "0"))
	float _DistanceBetweenSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "-Misc", meta = (ClampMin = "0"))
	float _LifespanMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "-Misc", meta = (ClampMin = "0"))
	float _LifespanMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "-Misc")
	TSubclassOf<APlacableActor> _PlacableActor;

	UFUNCTION(BlueprintCallable)
	ATriggerBox* FindTriggerBoxByTag(FName Tag);

	UFUNCTION()
	FVector GetRandLocationInTriggerBox();

	TArray<FVector> SpawnLocations;

private:

	

	bool bIsEnoughSpace(FVector NewLocation);

	void SpawnTarget();

	



};



