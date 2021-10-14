// Fill out your copyright notice in the Description page of Project Settings.


#include "GP_MinionCommand_Possess.h"

#include "GP4/GP4.h"
#include "GP4/GP_Character/GP_PlayerController.h"
#include "GP4/GP_Minions/GP_AIMinionController.h"
#include "GP4/GP_Minions/GP_Minion.h"
#include "GP4/GP_Minions/GP_PatrollingAIController.h"

UGP_MinionCommand_Possess::UGP_MinionCommand_Possess() {
	bIsOverridableCommand = false;
}

void UGP_MinionCommand_Possess::ExecuteCommand(AGP_Minion* Minion) {
	if (Minion->IsPendingKillOrUnreachable()) {
		return;
	}

	if (Minion->IsPossessedByPlayer()) {
		FinishCommand(Minion);
	}
	else {
		Super::ExecuteCommand(Minion);
		if (Minion->bIsPendingKill == false) {
			GetPossessedByPlayer(Minion);
		}
	}

}

void UGP_MinionCommand_Possess::GetPossessedByPlayer(AGP_Minion* Minion) {
	Minion->ConsumeMovementInputVector();
	Minion->PlayerController->Possess(Minion);
	//this->FinishCommand(Minion);

}

void UGP_MinionCommand_Possess::GetPossessedByAI(AGP_Minion* Minion) {
	Minion->ConsumeMovementInputVector();
	Minion->MinionAiController->Possess(Minion);
	//this->FinishCommand(Minion);
}
