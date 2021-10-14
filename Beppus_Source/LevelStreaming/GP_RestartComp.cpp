// Fill out your copyright notice in the Description page of Project Settings.

#include "GP_RestartComp.h"


// #include "GP4/GP4.h"
// #include "GP4/GP_Puzzles/GP_DynamicObject.h"
#include "GP_DynamicObjectHandlerComponent.h"
#include "GP4/GP4GameModeBase.h"

// Sets default values for this component's properties
UGP_RestartComp::UGP_RestartComp(){
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bAutoActivate = true;
	bAutoRegister = true;
}

// Called when the game starts
void UGP_RestartComp::BeginPlay(){
	Super::BeginPlay();
	GameMode = GetWorld()->GetAuthGameMode<AGP4GameModeBase>();
	ensure(GameMode);
	AnnounceSelf();
	// ...
}
void UGP_RestartComp::EndPlay( const EEndPlayReason::Type EndPlayReason ){
	ensure(GameMode);
	UnAnnounceSelf();
	Super::EndPlay(EndPlayReason);
}

void UGP_RestartComp::AnnounceSelf(){
	GameMode->DynamicObjectHandler->ReceiveAnnounce(this);
}
void UGP_RestartComp::UnAnnounceSelf(){
	GameMode->DynamicObjectHandler->ReceiveUnAnnounce(this);
}
void UGP_RestartComp::HandleLoadRequest(){
	OnLoadRequest.Broadcast();
	if ( bUseTimer ){
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UGP_RestartComp::LoadComplete, DelayTime, false);
	}
}

void UGP_RestartComp::LoadComplete(){
	OnLoadComplete.Broadcast();
	GameMode->DynamicObjectHandler->LoadCompleteCallback(this);
	if ( bUseTimer ){
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	}
}
void UGP_RestartComp::HandleUnloadRequest(){
	OnUnloadRequest.Broadcast();
	if ( bUseTimer ){
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UGP_RestartComp::UnloadComplete, DelayTime, false);
	}
}
void UGP_RestartComp::UnloadComplete(){
	OnUnloadComplete.Broadcast();
	GameMode->DynamicObjectHandler->UnloadCompleteCallback(this);
	if ( bUseTimer ){
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	}
}

