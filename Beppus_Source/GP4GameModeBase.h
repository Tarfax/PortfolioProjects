// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GP_Minions/GP_MinionMasterMindBigBrainController.h"

// #include "LevelStreaming/GP_LevelStreamInterface.h"

#include "GP4GameModeBase.generated.h"

class UGP_SaveGameManager;
class UGP_SaveGame;
class ULevelStreamingDynamic;
class UGP_DynamicObjectHandlerComponent;
class AGP_LevelSpawner;
/**
 * 
 */
class ULevelSpawnerData;
class AGP_WorldLightInterpolation;
class UGP_LevelStreamingSystemComponent;

UCLASS()
class GP4_API AGP4GameModeBase : public AGameModeBase {
	GENERATED_BODY()
public:
	AGP4GameModeBase();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UGP_DynamicObjectHandlerComponent* DynamicObjectHandler;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UGP_LevelStreamingSystemComponent* LevelStreamingSystem;
	
	UPROPERTY()
	UGP_SaveGameManager* SaveGameManager;

	virtual void BeginPlay() override;
	virtual void EndPlay( const EEndPlayReason::Type EndPlayReason ) override;
	virtual void ResetLevel() override;
	virtual void RestartPlayer( AController* NewPlayer ) override;
	virtual void RestartPlayerAtPlayerStart( AController* NewPlayer, AActor* StartSpot ) override;
	virtual void RestartPlayerAtTransform( AController* NewPlayer, const FTransform& SpawnTransform ) override;

	virtual void Reset() override;
	virtual void InitGame( const FString& MapName, const FString& Options, FString& ErrorMessage ) override;
	virtual void InitGameState() override;
	virtual void StartPlay() override;



	void HandleCameraPawnTravelComplete();
	UFUNCTION(BlueprintImplementableEvent)
	void OnCameraPawnTravelComplete();
	UFUNCTION()
	void HandleAllDynamicObjectsHasFinishedLoading();
	UFUNCTION()
	void HandleAllDynamicObjectsHasFinishedUnloading();
	UFUNCTION()
	void HandleLevelSpawnerHasFinishedLoading();

	void CheckIfLevelHasLoaded();
	UFUNCTION()
	void LoadSubLevels();

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FGeneralPurposeDelegate OnLevelStartedLoading_Delegate;
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FGeneralPurposeDelegate OnLevelFinishedLoading_Delegate;
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FGeneralPurposeDelegate OnLevelStartedResetting_Delegate;
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FGeneralPurposeDelegate OnLevelFinishedResetting_Delegate;
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FGeneralPurposeDelegate OnLevelStartedUnloading_Delegate;
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FGeneralPurposeDelegate OnLevelFinishedUnloading_Delegate;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	AGP_WorldLightInterpolation* WorldLightInterpolation;

	// --> Loading levels <-- 
	UFUNCTION(BlueprintCallable)
	void LoadNewLevel( int LevelToLoad );
	UFUNCTION()
	void StartLoadingNewLevel();

	UFUNCTION(BlueprintCallable)
	void LoadStaticLevels();
	UFUNCTION()
	void LoadMainLightAndBackgroundLevel();
	UFUNCTION(BlueprintCallable)
	void OnLevelSpawnerLoaded( AGP_LevelSpawner* LevelSpawner, int CustomLevelNumber );

	FTimerHandle TimerHandle;
	FTimerHandle TimerHandlerLightLevel;
	UFUNCTION()
	void UnloadPreviousLevel();
	int NextLevelToLoad;
	int CurrentlyLoadedLevel;
	UPROPERTY(BlueprintReadWrite)
	AGP_LevelSpawner* AnnouncedLevelSpawner;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	ULevelSpawnerData* LevelSpawnerData;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ULevelStreamingDynamic* InstancedLevel;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ULevelStreamingDynamic* PreviousInstancedLevel;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool HasLoadedFromMainLevelStream;

	bool bDynamicObjectsHasFinishedReset;
	bool bDynamicObjectsHasFinishedLoading;
	bool bDynamicObjectsHasFinishedUnloading;

	bool bAllSubLevelsHasFinishedLoading;
	bool bCameraPawnTravelToLocationComplete;

	bool bIsResettingLevel;
	bool bIsLoadingNewLevel;

	bool bIsNewGame = true;

	bool bHasRequestedReload;
	bool bHasGameStarted;

	UPROPERTY(VisibleAnywhere)
	TArray<class AActor*> CompsWaitingForSignal;
};
