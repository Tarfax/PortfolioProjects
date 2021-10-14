// Fill out your copyright notice in the Description page of Project Settings.


#include "GP_MinionCommand_UnPossess.h"

#include "GP_MinionCommand_Possess.h"

#include "GP4/GP4.h"
#include "GP4/GP_Character/GP_PlayerController.h"
#include "GP4/GP_Minions/GP_Minion.h"
#include "GP4/GP_Minions/GP_PatrollingAIController.h"

UGP_MinionCommand_UnPossess::UGP_MinionCommand_UnPossess(){
	bIsOverridableCommand = true;
}
void UGP_MinionCommand_UnPossess::ExecuteCommand( AGP_Minion* Minion ){
	if(Minion->IsPendingKillOrUnreachable()) {
		return;
	}
	if ( Minion->IsPossessedByPlayer() ){
	Super::ExecuteCommand(Minion);
		GetPossessedByAI(Minion);
	}
	else{
		FinishCommand(Minion);
	}
}
void UGP_MinionCommand_UnPossess::GetPossessedByPlayer( AGP_Minion* Minion ){
	Minion->ConsumeMovementInputVector();
	Minion->PlayerController->Possess(Minion);
	//this->FinishCommand(Minion);

}
void UGP_MinionCommand_UnPossess::GetPossessedByAI( AGP_Minion* Minion ){
	Minion->ConsumeMovementInputVector();
	Minion->MinionAiController->Possess(Minion);
	//this->FinishCommand(Minion);
}
