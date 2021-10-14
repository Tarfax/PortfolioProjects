// Copyright Epic Games, Inc. All Rights Reserved.

#include "GP4GameModeBase.h"

#include "EngineUtils.h"
#include "GP4.h"
#include "LevelStreaming/GP_LevelSpawner.h"

#include "Engine/LevelStreaming.h"
#include "Engine/LevelStreamingDynamic.h"
#include "FG_SaveGame/GP_SaveGame.h"
#include "FG_SaveGame/GP_SaveGameManager.h"

#include "Kismet/GameplayStatics.h"
#include "LevelStreaming/GP_DynamicObjectHandlerComponent.h"
#include "LevelStreaming/GP_LevelSpawnerData.h"

class UKismetSystemLibrary;

AGP4GameModeBase::AGP4GameModeBase() {
	bFindCameraComponentWhenViewTarget = false;
	DynamicObjectHandler = CreateDefaultSubobject<UGP_DynamicObjectHandlerComponent>(TEXT("LevelStreamComp"));
}

void AGP4GameModeBase::BeginPlay() {
	Super::BeginPlay();
	LevelSpawnerData->UpdateLevels();
	DynamicObjectHandler->OnAllObjectsLoaded.AddDynamic(
		this,
		&AGP4GameModeBase::HandleAllDynamicObjectsHasFinishedLoading);
	DynamicObjectHandler->OnAllObjectsUnloaded.AddDynamic(
		this,
		&AGP4GameModeBase::HandleAllDynamicObjectsHasFinishedUnloading);
	SaveGameManager = UGP_SaveGameManager::Get();

	LoadMainLightAndBackgroundLevel();


}

void AGP4GameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	SaveGameManager->SaveGame();
	DynamicObjectHandler->OnAllObjectsLoaded.RemoveDynamic(
		this, &AGP4GameModeBase::HandleAllDynamicObjectsHasFinishedLoading);
	DynamicObjectHandler->OnAllObjectsUnloaded.RemoveDynamic(
		this, &AGP4GameModeBase::HandleAllDynamicObjectsHasFinishedUnloading);
	TimerHandlerLightLevel.Invalidate();
	TimerHandle.Invalidate();
}

void AGP4GameModeBase::ResetLevel() {
	bIsResettingLevel = true;
	bIsLoadingNewLevel = false;
	bHasRequestedReload = false;
	bCameraPawnTravelToLocationComplete = false;
	bDynamicObjectsHasFinishedReset = false;
	bDynamicObjectsHasFinishedLoading = false;
	bDynamicObjectsHasFinishedUnloading = false;
	DynamicObjectHandler->RequestUnload(false);
	OnLevelStartedResetting_Delegate.Broadcast();
}

void AGP4GameModeBase::RestartPlayer(AController* NewPlayer) {
	Super::RestartPlayer(NewPlayer);
}

void AGP4GameModeBase::RestartPlayerAtPlayerStart(AController* NewPlayer, AActor* StartSpot) {
	Super::RestartPlayerAtPlayerStart(NewPlayer, StartSpot);
}

void AGP4GameModeBase::RestartPlayerAtTransform(AController* NewPlayer, const FTransform& SpawnTransform) {
	Super::RestartPlayerAtTransform(NewPlayer, SpawnTransform);
}

void AGP4GameModeBase::Reset() {
	ResetLevel();
}

void AGP4GameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) {
	Super::InitGame(MapName, Options, ErrorMessage);
}

void AGP4GameModeBase::InitGameState() {
	Super::InitGameState();
}

void AGP4GameModeBase::StartPlay() {
	Super::StartPlay();
}

void AGP4GameModeBase::HandleCameraPawnTravelComplete() {
	bCameraPawnTravelToLocationComplete = true;
	OnCameraPawnTravelComplete();
	CheckIfLevelHasLoaded();
}

void AGP4GameModeBase::HandleAllDynamicObjectsHasFinishedLoading() {
	bDynamicObjectsHasFinishedLoading = true;
	CheckIfLevelHasLoaded();
}

void AGP4GameModeBase::HandleAllDynamicObjectsHasFinishedUnloading() {
	bDynamicObjectsHasFinishedUnloading = true;
	CheckIfLevelHasLoaded();
}

void AGP4GameModeBase::HandleLevelSpawnerHasFinishedLoading() {
	bAllSubLevelsHasFinishedLoading = true;
	CheckIfLevelHasLoaded();
}

void AGP4GameModeBase::CheckIfLevelHasLoaded() {
	if (bIsResettingLevel == true) {
		if (bCameraPawnTravelToLocationComplete && bDynamicObjectsHasFinishedLoading) {
			OnLevelFinishedResetting_Delegate.Broadcast();
			bIsResettingLevel = false;
			return;
		}
		if (!bHasRequestedReload && !bDynamicObjectsHasFinishedLoading && bDynamicObjectsHasFinishedUnloading) {
			DynamicObjectHandler->RequestLoad();
			bHasRequestedReload = true;
			return;
		}
		return;
	}
	if (bIsLoadingNewLevel == true) {
		if (bCameraPawnTravelToLocationComplete && bAllSubLevelsHasFinishedLoading) {
			OnLevelFinishedLoading_Delegate.Broadcast();
			bIsLoadingNewLevel = false;
		}
	}
}

void AGP4GameModeBase::LoadStaticLevels() {
	bDynamicObjectsHasFinishedUnloading = true;
	if (HasLoadedFromMainLevelStream == false) {
		for (int i = 0; i < LevelSpawnerData->Levels.Num(); i++) {
			bool IsSuccess;
			ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(
				GetWorld(),
				LevelSpawnerData->Levels[i].StaticDecorations,
				LevelSpawnerData->Levels[i].LevelLocation,
				FRotator::ZeroRotator,
				IsSuccess);
		}
		HasLoadedFromMainLevelStream = true;
	}
}

void AGP4GameModeBase::LoadMainLightAndBackgroundLevel() {
	bool bOutSuccess;
	ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(GetWorld(),
	                                                         LevelSpawnerData->MainLightingLevel,
	                                                         FVector::ZeroVector,
	                                                         FRotator::ZeroRotator,
	                                                         bOutSuccess);
	bHasGameStarted = bOutSuccess;
}

void AGP4GameModeBase::LoadNewLevel(int LevelToLoad) {
	LOG("AGP4GameModeBase::LoadNewLevel");
	// AllSubLevelsHasFinishedLoading = false;
	// DynamicObjectsHasFinishedUnloading = false;
	bCameraPawnTravelToLocationComplete = false;
	if (bIsLoadingNewLevel == false) {
		OnLevelStartedLoading_Delegate.Broadcast();
		bIsLoadingNewLevel = true;
		LOG("---> AGP4GameModeBase::LoadNewLevel -> IsLoadingNewLevel == false");
	}
	bIsResettingLevel = false;
	if (LevelToLoad >= LevelSpawnerData->Levels.Num()) {
		NextLevelToLoad = 0;
	}
	else {
		NextLevelToLoad = LevelToLoad;
	}
	DynamicObjectHandler->OnAllObjectsUnloaded.AddDynamic(this, &AGP4GameModeBase::StartLoadingNewLevel);
	DynamicObjectHandler->RequestUnload(false);
}

void AGP4GameModeBase::StartLoadingNewLevel() {
	LOG("AGP4GameModeBase::StartLoadingNewLevel");
	DynamicObjectHandler->OnAllObjectsUnloaded.RemoveDynamic(this, &AGP4GameModeBase::StartLoadingNewLevel);
	F_LevelStreamingData LevelStreamingData = LevelSpawnerData->Levels[NextLevelToLoad];
	bool IsSuccess;
	InstancedLevel = ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(
		GetWorld(),
		LevelStreamingData.MainLevel,
		LevelStreamingData.LevelLocation,
		FRotator::ZeroRotator,
		IsSuccess);
	//InstancedLevel->SetShouldBeLoaded(true);
	AGP_LevelInstanceData* InstanceData = Cast<AGP_LevelInstanceData>(
		LevelSpawnerData->Levels[NextLevelToLoad].LevelInstanceData.GetDefaultObject());
	if (InstanceData) {
		SaveGameManager->UpdateSave(InstanceData->LevelNumber);
	}
	//	InstancedLevel->OnLevelLoaded.AddDynamic(this, &AGP4GameModeBase::BeginAnimateIn);
	//Unload previous level when new level has loaded in and is ready for action.
	OnLevelFinishedLoading_Delegate.AddDynamic(this, &AGP4GameModeBase::UnloadPreviousLevel);
}


void AGP4GameModeBase::OnLevelSpawnerLoaded(AGP_LevelSpawner* LevelSpawner, int CustomLevelNumber) {
	bIsResettingLevel = false;
	bCameraPawnTravelToLocationComplete = false;
	bDynamicObjectsHasFinishedReset = false;
	bDynamicObjectsHasFinishedLoading = false;
	bDynamicObjectsHasFinishedUnloading = false;
	//On Level Started Loading
	if (bIsNewGame == true) {
		OnLevelStartedLoading_Delegate.Broadcast();
		bIsNewGame = false;
		bIsLoadingNewLevel = true;
	}
	AnnouncedLevelSpawner = LevelSpawner;
	if (HasLoadedFromMainLevelStream == false) {
		NextLevelToLoad = CustomLevelNumber;
	}
	CurrentlyLoadedLevel = NextLevelToLoad;
	// AnnouncedLevelSpawner->LoadSubLevels(NextLevelToLoad, HasLoadedFromMainLevelStream);

	if (bHasGameStarted == true) {
		AnnouncedLevelSpawner->LoadSubLevels(NextLevelToLoad, HasLoadedFromMainLevelStream);
		if (SaveGameManager != nullptr) {
			SaveGameManager->UpdateSave(CurrentlyLoadedLevel);
		}
	}
	else {
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this,
		                                       &AGP4GameModeBase::LoadSubLevels, 0.25f, false, -1.0);
	}
}


void AGP4GameModeBase::LoadSubLevels() {
	LOG("LoadSubLevels")
	if (bHasGameStarted == true) {
		AnnouncedLevelSpawner->LoadSubLevels(NextLevelToLoad, HasLoadedFromMainLevelStream);
		if (SaveGameManager != nullptr) {
			SaveGameManager->UpdateSave(CurrentlyLoadedLevel);
		}
	}
	else {
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this,
		                                       &AGP4GameModeBase::LoadSubLevels, 0.10f, false, -1.0);
	}
}

void AGP4GameModeBase::UnloadPreviousLevel() {
	LOG("AGP4GameModeBase::UnloadPreviousLevel");
	OnLevelFinishedLoading_Delegate.RemoveDynamic(this, &AGP4GameModeBase::UnloadPreviousLevel);
	if (PreviousInstancedLevel != nullptr) {
		PreviousInstancedLevel->SetIsRequestingUnloadAndRemoval(true);
	}
	PreviousInstancedLevel = InstancedLevel;
	OnLevelFinishedUnloading_Delegate.Broadcast();
}
