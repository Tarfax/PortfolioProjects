// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GP_MinionCommand_Base.h"
#include "GP4/GP_Minions/GP_MinionDeath.h"
#include "GP_MinionCommand_Explode.generated.h"

UCLASS(Blueprintable, BlueprintType)
class GP4_API UGP_MinionCommand_Explode : public UGP_MinionCommand_Base {
	GENERATED_BODY()

public:
	UGP_MinionCommand_Explode();
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AGP_MinionDeath> DeathType;

	virtual void ExecuteCommand(AGP_Minion* Minion) override;

};
