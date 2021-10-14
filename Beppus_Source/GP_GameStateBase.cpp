// Fill out your copyright notice in the Description page of Project Settings.

#include "GP_GameStateBase.h"

#include "GP4.h"
#include "GP4GameModeBase.h"
#include "GP_Puzzles/GP_PuzzlePiece_Portal.h"

// Sets default values
AGP_GameStateBase::AGP_GameStateBase(){
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGP_GameStateBase::BeginPlay(){
	Super::BeginPlay();
	GameMode = Cast<AGP4GameModeBase>(GetWorld()->GetAuthGameMode());
	GameMode->OnLevelStartedResetting_Delegate.AddDynamic(this, &AGP_GameStateBase::HandlePortalsOnRestart);
}
void AGP_GameStateBase::EndPlay( const EEndPlayReason::Type EndPlayReason ){
	Super::EndPlay(EndPlayReason);
	GameMode->OnLevelStartedResetting_Delegate.RemoveDynamic(this, &AGP_GameStateBase::HandlePortalsOnRestart);
	HandlePortalsOnRestart();
}

// Called every frame
void AGP_GameStateBase::Tick( float DeltaTime ){
	Super::Tick(DeltaTime);
}


void AGP_GameStateBase::HandleBeginPlay(){

	MasterMind = AGP_MinionMasterMindBigBrainController::Get();

	Super::HandleBeginPlay();
}

void AGP_GameStateBase::AddPortal( AGP_PuzzlePiece_Portal* Portal ){
	if ( Portals.Num() > 1 ){
		Portals.Swap(0, 1);
		AGP_PuzzlePiece_Portal* RemovedPortal = Portals[1];
		// RemovePortal(RemovedPortal);
		RemovedPortal->Explode();
	}

	Portals.Add(Portal);

	if ( Portals.Num() > 1 ){
		Portals[0]->SetPortalConnection(Portal);
		Portal->SetPortalConnection(Portals[0]);
	}

	// for ( int i = 0; i < Portals.Num(); i++ ){
	// 	FString string = "";
	// 	string += Portals[i]->GetName();
	// 	// UE_LOG(LogTemp, Log, TEXT("Portals in list after add: %s"), *string);
	// }
}

void AGP_GameStateBase::RemovePortal( AGP_PuzzlePiece_Portal* Portal ){
	if ( Portals.Contains(Portal) == true ){
		int index = Portals.Find(Portal);

		// FString string = "";
		// string += Portals[index]->GetName();
		// UE_LOG(LogTemp, Log, TEXT("Portal about to destroy is: %s"), *string);

		Portals.RemoveAt(index, 1, true);
	}


	for ( int i = 0; i < Portals.Num(); i++ ){
		Portals[i]->RemovePortalConnection();

		// FString string = "";
		// string += Portals[i]->GetName();
		// UE_LOG(LogTemp, Log, TEXT("Portal index and Portals size: %i, %i"), i, Portals.Num());
		// UE_LOG(LogTemp, Log, TEXT("Portal left after remove: %s"), *string);
	}
}
bool AGP_GameStateBase::HasBegunPlay() const{
	return Super::HasBegunPlay();
}
float AGP_GameStateBase::GetPlayerRespawnDelay( AController* Controller ) const{
	return Super::GetPlayerRespawnDelay(Controller);
}
void AGP_GameStateBase::ReceivedGameModeClass(){
	Super::ReceivedGameModeClass();
	if ( MasterMind == nullptr ){
		MasterMind = AGP_MinionMasterMindBigBrainController::Get();
	}
	HandlePortalsOnRestart();


}
void AGP_GameStateBase::HandlePortalsOnRestart(){
	if ( Portals.Num() > 0 ){
		// for ( auto Portal : Portals ){
		// 	Portal->Destroy();
		// }
		Portals.Empty();
	}
}
void AGP_GameStateBase::Reset(){
	Super::Reset();
	HandlePortalsOnRestart();
}
