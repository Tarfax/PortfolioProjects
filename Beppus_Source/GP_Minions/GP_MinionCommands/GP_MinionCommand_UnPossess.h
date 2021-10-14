// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GP_MinionCommand_Base.h"
#include "GP_MinionCommand_Possess.h"

#include "GP_MinionCommand_UnPossess.generated.h"

/**
 *
 */
UCLASS(Blueprintable, BlueprintType)
class GP4_API UGP_MinionCommand_UnPossess : public UGP_MinionCommand_Base{
	GENERATED_BODY()
public:
	UGP_MinionCommand_UnPossess();
	virtual void ExecuteCommand( AGP_Minion* Minion ) override;
	void GetPossessedByPlayer( AGP_Minion* Minion );
	void GetPossessedByAI( AGP_Minion* Minion );

};
