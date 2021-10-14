// Fill out your copyright notice in the Description page of Project Settings.


#include "GP_MinionCommand_Idle.h"
#include "GP4/GP_Minions/GP_Minion.h"

void UGP_MinionCommand_Idle::ExecuteCommand( AGP_Minion* Minion ){
	Super::ExecuteCommand(Minion);
	Minion->ConsumeMovementInputVector();
	FinishCommand(Minion);
}
