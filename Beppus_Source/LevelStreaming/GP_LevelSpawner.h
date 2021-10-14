// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/LevelStreamingDynamic.h"
#include "GameFramework/Actor.h"
#include "GP4/GP4GameModeBase.h"
#include "GP4/GP4.h"
#include "GP_LevelSpawner.generated.h"

struct F_LevelStreamBounds;

UCLASS()
class AGP_LevelSpawner : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGP_LevelSpawner();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void LoadSubLevels(int LevelToLoad, bool ShouldLoadStaticLevel);

	//Helper Functions
	UFUNCTION()
	void OnSubLevelLoaded();

	UPROPERTY(EditDefaultsOnly)
	ULevelSpawnerData* LevelSpawnerData;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<ULevelStreamingDynamic*> InstancedLevels;

	UPROPERTY(BlueprintReadOnly)
	AGP4GameModeBase* GameMode;

	UPROPERTY(EditDefaultsOnly)
	bool bUseCustomLevelNumber = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int LevelNumber;


	int LoadedLevelCounter;
	int UnLoadedLevelCounter;
	FTimerHandle TimerHandle;
};
