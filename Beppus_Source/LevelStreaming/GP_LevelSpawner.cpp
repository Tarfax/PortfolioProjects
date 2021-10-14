// Fill out your copyright notice in the Description page of Project Settings.

#include "GP_LevelSpawner.h"

#include "GP_LevelSpawnerData.h"
#include "Engine/LevelStreamingDynamic.h"
#include "GP4/GP4.h"

// Sets default values
AGP_LevelSpawner::AGP_LevelSpawner() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AGP_LevelSpawner::BeginPlay() {
	Super::BeginPlay();
	GameMode = Cast<AGP4GameModeBase>(GetWorld()->GetAuthGameMode());
	LevelSpawnerData = GameMode->LevelSpawnerData;
	if (!bUseCustomLevelNumber) { LevelNumber = LevelSpawnerData->GetLevelSpawnerLevelNumber(this); }
	GameMode->OnLevelSpawnerLoaded(this, LevelNumber);
}

void AGP_LevelSpawner::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	UnLoadedLevelCounter = LoadedLevelCounter;
	for (int i = 0; i < InstancedLevels.Num(); i++) {
		ULevelStreamingDynamic* InstancedLevel = InstancedLevels[i];
		InstancedLevel->SetIsRequestingUnloadAndRemoval(true);
		//Ehh.. dunno. For debug?
		UnLoadedLevelCounter--;
	}
}

void AGP_LevelSpawner::LoadSubLevels(int LevelToLoad, bool HasLoadedFromMainLevelStream) {
	// LOG("LEVEL SPAWNER:: LOAD SUB LEVELS")
	F_LevelStreamingData levelStreamingData = LevelSpawnerData->Levels[LevelToLoad];
	if (HasLoadedFromMainLevelStream == false) {
		bool IsSuccess;
		ULevelStreamingDynamic* InstancedLevel = ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(
			this, levelStreamingData.StaticDecorations, levelStreamingData.LevelLocation, FRotator::ZeroRotator,
			IsSuccess);
		InstancedLevels.Add(InstancedLevel);
		LoadedLevelCounter++;
	}
	for (int i = 0; i < levelStreamingData.LevelsToLoad.Num(); i++) {
		TSoftObjectPtr<UWorld> Level = levelStreamingData.LevelsToLoad[i];
		bool IsSuccess;
		ULevelStreamingDynamic* InstancedLevel = ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(
			this, Level, levelStreamingData.LevelLocation, FRotator::ZeroRotator, IsSuccess);
		InstancedLevels.Add(InstancedLevel);
		InstancedLevel->OnLevelLoaded.AddDynamic(this, &AGP_LevelSpawner::OnSubLevelLoaded);
	}
}

void AGP_LevelSpawner::OnSubLevelLoaded() {
	LoadedLevelCounter++;
	if (LoadedLevelCounter == InstancedLevels.Num()) { GameMode->HandleLevelSpawnerHasFinishedLoading(); }
}
