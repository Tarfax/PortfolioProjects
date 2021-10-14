// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GP_MinionCommand_Base.h"


#include "GP_MinionCommand_Possess.generated.h"

/**
 *
 */
UCLASS(Blueprintable, BlueprintType)
class GP4_API UGP_MinionCommand_Possess : public UGP_MinionCommand_Base {
	GENERATED_BODY()

public:
	UGP_MinionCommand_Possess();
	virtual void ExecuteCommand( AGP_Minion* Minion ) override;
	UFUNCTION(BlueprintCallable)
	void GetPossessedByPlayer( AGP_Minion* Minion );
	UFUNCTION(BlueprintCallable)
	void GetPossessedByAI(AGP_Minion* Minion);
};
