// Fill out your copyright notice in the Description page of Project Settings.

#pragma warning(disable : 4996)

#include "GP_MinionDeath.h"

#include "DestructibleComponent.h"
#include "GP_Minion.h"
#include "GP4/GP4.h"
#include "GP4/FG_SaveGame/GP_SaveGameManager.h"
#include "GP4/LevelStreaming/GP_RestartComp.h"

// Sets default values
AGP_MinionDeath::AGP_MinionDeath() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;
	// BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	// BoxCollider->SetupAttachment(SceneComponent);
	DestructibleMesh = CreateDefaultSubobject<UDestructibleComponent>(TEXT("DestructibleMesh"));
	DestructibleMesh->SetupAttachment(SceneComponent);

	// RestartComp= CreateDefaultSubobject<UGP_RestartComp>(TEXT("RestartComp"));
}

// Called when the game starts or when spawned
void AGP_MinionDeath::BeginPlay() { Super::BeginPlay(); }

// Called every frame
void AGP_MinionDeath::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

void AGP_MinionDeath::BeginMinionDeath(AGP_Minion* Minion = nullptr) {
	if (bHasBegunDeath) { return; }
	else {
		bHasBegunDeath = true;
		OnBeginMinionDeath(MinionToKill);
	
	}
}

void AGP_MinionDeath::DestroyDestructible() { OnDestroyDestructible(); }

void AGP_MinionDeath::EndMinionDeath() {
	MinionDeathCompleteDelegate.Broadcast();
}
