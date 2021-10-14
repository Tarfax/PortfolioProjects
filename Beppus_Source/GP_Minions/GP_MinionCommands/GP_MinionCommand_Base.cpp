// Fill out your copyright notice in the Description page of Project Settings.

#include "GP_MinionCommand_Base.h"

#include "GP4/GP4.h"
#include "GP4/GP_Minions/GP_AIMinionController.h"
#include "GP4/GP_Minions/GP_Minion.h"
#include "GP4/GP_Minions/GP_PatrollingAIController.h"


// Add default functionality here for any UGP_MinionCommand_Base functions that are not pure virtual.

UGP_MinionCommand_Base::UGP_MinionCommand_Base(){

}



void UGP_MinionCommand_Base::ExecuteCommand( AGP_Minion* Minion ){
	MasterMind = Minion->MinionAiController->MasterMind;
	OnExecuteCommand(Minion);

}
void UGP_MinionCommand_Base::FinishCommand( AGP_Minion* Minion ){
	OnFinishedCommand(Minion);
	Minion->CurrentCommand = nullptr;
	MarkPendingKill();
}
void UGP_MinionCommand_Base::StartDelay( float seconds ){
	TimerDynamicDelegate.BindDynamic(this, &UGP_MinionCommand_Base::OnDelayFinished);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDynamicDelegate, seconds, false);
//	LOG("StartDelayCalled")
}
