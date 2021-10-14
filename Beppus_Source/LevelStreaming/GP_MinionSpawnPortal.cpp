// Fill out your copyright notice in the Description page of Project Settings.

#include "GP_MinionSpawnPortal.h"

#include "GP4/GP4.h"
#include "GP4/GP4GameModeBase.h"
#include "GP4/GP_Minions/GP_Minion.h"
#include "GP4/GP_GameStateBase.h"

class AGP_GameStateBase;
// Sets default values
AGP_MinionSpawnPortal::AGP_MinionSpawnPortal() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGP_MinionSpawnPortal::BeginPlay() {
	Super::BeginPlay();
	MasterMind = AGP_MinionMasterMindBigBrainController::Get();
	ensure(MasterMind != nullptr);
	MasterMind->MinionSpawnPortals.Add(this);
	
	GetWorld()->GetAuthGameMode<AGP4GameModeBase>()->OnLevelFinishedLoading_Delegate.AddDynamic(
		this,
		&AGP_MinionSpawnPortal::SpawnMinionsFromPortal);
	bHasSpawnedFirstTime = false;
	
	GetWorld()->GetAuthGameMode<AGP4GameModeBase>()->OnLevelFinishedResetting_Delegate.AddDynamic(
		this,
		&AGP_MinionSpawnPortal::SpawnMinionsFromPortal);
	GetWorld()->GetAuthGameMode<AGP4GameModeBase>()->OnLevelStartedResetting_Delegate.AddDynamic(
		this,
		&AGP_MinionSpawnPortal::Reset);
}

void AGP_MinionSpawnPortal::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	MasterMind->MinionSpawnPortals.Remove(this);

	GetWorld()->GetAuthGameMode<AGP4GameModeBase>()->OnLevelFinishedResetting_Delegate.RemoveDynamic(
		this,
		&AGP_MinionSpawnPortal::SpawnMinionsFromPortal);

	GetWorld()->GetAuthGameMode<AGP4GameModeBase>()->OnLevelStartedResetting_Delegate.RemoveDynamic(
		this,
		&AGP_MinionSpawnPortal::Reset);
}

void AGP_MinionSpawnPortal::Reset() {
	bHasSpawnedAll = false;
}

void AGP_MinionSpawnPortal::SpawnMinionsFromPortal() {
	if (bHasSpawnedAll) {
		return;
	}
	LOG("Spawn??");
	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRotation = GetActorRotation();
	for (int i = 0; i < NumberOfMinionsToSpawn; ++i) {
		SpawnLocation.Y += MinionSpawnYOffset * i;
		AGP_Minion* Minion = GetWorld()->SpawnActor<AGP_Minion>(MinionClass, SpawnLocation, SpawnRotation);
		OnMinionSpawned(Minion);
		MasterMind->OnMinionSpawnedDelegate.Broadcast(Minion);
	}
	bHasSpawnedAll = true;

	if (bHasSpawnedFirstTime == false) {
		GetWorld()->GetAuthGameMode<AGP4GameModeBase>()->OnLevelFinishedLoading_Delegate.RemoveDynamic(
			this,
			&AGP_MinionSpawnPortal::SpawnMinionsFromPortal);
		bHasSpawnedFirstTime = true;
	}
}
