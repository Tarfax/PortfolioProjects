// Fill out your copyright notice in the Description page of Project Settings.


#include "GP_MinionCommand_MoveTo.h"
#include "GP4/GP_Minions/GP_AIMinionController.h"
#include "GP4/GP_Minions/GP_Minion.h"
#include "GP4/GP_Minions/GP_PatrollingAIController.h"


UGP_MinionCommand_MoveTo::UGP_MinionCommand_MoveTo(){
	bIsOverridableCommand = true;
}
void UGP_MinionCommand_MoveTo::ExecuteCommand( AGP_Minion* Minion ){
	Super::ExecuteCommand(Minion);
	Minion->MinionAiController->MasterMind->AllMinionsMoveToDelegate.Broadcast(Minion->GetActorLocation());

}
