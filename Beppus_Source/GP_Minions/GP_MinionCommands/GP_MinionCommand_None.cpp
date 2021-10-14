// Fill out your copyright notice in the Description page of Project Settings.


#include "GP_MinionCommand_None.h"
void UGP_MinionCommand_None::ExecuteCommand( AGP_Minion* Minion ){
Super::ExecuteCommand(Minion);
	FinishCommand(Minion);
}
