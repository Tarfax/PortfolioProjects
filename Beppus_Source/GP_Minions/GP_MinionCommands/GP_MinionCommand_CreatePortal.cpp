// Fill out your copyright notice in the Description page of Project Settings.


#include "GP_MinionCommand_CreatePortal.h"
void UGP_MinionCommand_CreatePortal::ExecuteCommand(AGP_Minion* Minion) {
	Super::ExecuteCommand(Minion);

	// //TODO Very temp solution to get the player reattached to the camera.
	// if ( Minion->IsPossessedByPlayer() ){
	// 	Minion->GetPossessedByAI();
	// }
	// Minion->Kill(EGP_MinionDeath::Explode);
	
}
