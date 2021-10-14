// Fill out your copyright notice in the Description page of Project Settings.


#include "GP_MinionCommand_SawDeath.h"
UGP_MinionCommand_SawDeath::UGP_MinionCommand_SawDeath(){
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
}
void UGP_MinionCommand_SawDeath::ExecuteCommand( AGP_Minion* Minion ){
	Super::ExecuteCommand(Minion);

}
