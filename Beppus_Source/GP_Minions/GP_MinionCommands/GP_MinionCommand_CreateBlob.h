// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GP_MinionCommand_Base.h"
#include "GP_MinionCommand_CreateBlob.generated.h"


UCLASS(Blueprintable, BlueprintType)
class GP4_API UGP_MinionCommand_CreateBlob: public UGP_MinionCommand_Base {
	GENERATED_BODY()

	public:
	virtual void ExecuteCommand(AGP_Minion* Minion) override;

	

};