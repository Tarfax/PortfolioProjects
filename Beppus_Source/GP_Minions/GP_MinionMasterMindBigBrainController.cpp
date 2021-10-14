// Fill out your copyright notice in the Description page of Project Settings.

#include "GP_MinionMasterMindBigBrainController.h"
#include "GP_Minion.h"

#include "GP4/GP4.h"
#include "GP4/GP4GameModeBase.h"
#include "GP4/LevelStreaming/GP_MinionSpawnPortal.h"

// Sets default values
AGP_MinionMasterMindBigBrainController::AGP_MinionMasterMindBigBrainController(){
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGP_MinionMasterMindBigBrainController::BeginPlay(){
	Super::BeginPlay();
	
}

AGP_MinionMasterMindBigBrainController* AGP_MinionMasterMindBigBrainController::Get(){
	static TWeakObjectPtr<AGP_MinionMasterMindBigBrainController> Instance;
	if ( Instance.IsValid() ){
		return Instance.Get();
	}
	Instance = NewObject<AGP_MinionMasterMindBigBrainController>();
	return Instance.Get();
}

void AGP_MinionMasterMindBigBrainController::EndPlay( const EEndPlayReason::Type EndPlayReason ){
	LOG("MASTERMIND END PLAY");
	if ( AllMinionsMoveToDelegate.IsBound() ){
		AllMinionsMoveToDelegate.Clear();
	}
	if ( AllMinionsExplodeDelegate.IsBound() ){
		AllMinionsExplodeDelegate.Clear();
	}
	
}

// Called every frame
void AGP_MinionMasterMindBigBrainController::Tick( float DeltaTime ){
	Super::Tick(DeltaTime);

}

void AGP_MinionMasterMindBigBrainController::MinionEnteredGoal( AGP_Minion* MinionInGoal ){
	MinionsInGoal.Add(MinionInGoal);
	OnMinionEnteredGoal.Broadcast();
}

bool AGP_MinionMasterMindBigBrainController::IsMinionPortalsDoneSpawning(){
	for ( auto Portal : MinionSpawnPortals ){
		if ( !Portal->bHasSpawnedAll ){
			return false;
		}
	}
	OnAllMinionsSpawned.Broadcast();
	return true;
}

void AGP_MinionMasterMindBigBrainController::Reset(){

	AllMinionsExplodeDelegate.Broadcast();
}
