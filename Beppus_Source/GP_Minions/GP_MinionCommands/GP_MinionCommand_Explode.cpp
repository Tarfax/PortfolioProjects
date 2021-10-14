// Fill out your copyright notice in the Description page of Project Settings.

#include "GP_MinionCommand_Explode.h"

#include "GP4/GP_Minions/GP_Minion.h"

UGP_MinionCommand_Explode::UGP_MinionCommand_Explode() { bIsOverridableCommand = false; }

void UGP_MinionCommand_Explode::ExecuteCommand(AGP_Minion* Minion) {
	Super::ExecuteCommand(Minion);
	// Minion->Kill(DeathType);
}
