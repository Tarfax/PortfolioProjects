// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Team9AssembleGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class TEAM9ASSEMBLE_API ATeam9AssembleGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	public:
	virtual void BeginPlay() override;
};
