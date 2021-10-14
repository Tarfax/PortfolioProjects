// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "GP4/GP_Minions/GP_MinionMasterMindBigBrainController.h"
#include "GP4/GP_Puzzles/GP_DynamicObject.h"

#include "GP_MinionSpawnPortal.generated.h"

class AGP_Minion;
UCLASS(Blueprintable, BlueprintType)
class GP4_API AGP_MinionSpawnPortal : public AGP_DynamicObject {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGP_MinionSpawnPortal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay( const EEndPlayReason::Type EndPlayReason ) override;
public:
	UPROPERTY(Transient, VisibleInstanceOnly)
	AGP_MinionMasterMindBigBrainController* MasterMind;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Minions")
	int NumberOfMinionsToSpawn = 1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Minions")
	TSubclassOf<AGP_Minion> MinionClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Minions")
	float MinionSpawnYOffset = 10.f;
	bool bHasSpawnedAll = false;
	bool bHasSpawnedFirstTime = false;

	UFUNCTION()
	virtual void Reset() override;
	UFUNCTION(BlueprintCallable)
	void SpawnMinionsFromPortal();
	UFUNCTION(BlueprintImplementableEvent)
	void OnMinionSpawned( AGP_Minion* SpawnedMinion );

};
