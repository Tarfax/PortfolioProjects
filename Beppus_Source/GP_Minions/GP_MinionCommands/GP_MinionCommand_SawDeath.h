// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GP_MinionCommand_Base.h"

#include "UObject/Object.h"
#include "GP_MinionCommand_SawDeath.generated.h"

/**
 *
 */
UCLASS()
class GP4_API UGP_MinionCommand_SawDeath : public UGP_MinionCommand_Base {
	GENERATED_BODY()
public:
	UGP_MinionCommand_SawDeath();
	UPROPERTY()
	USceneComponent* SceneComponent;
	virtual void ExecuteCommand( AGP_Minion* Minion ) override;
};
