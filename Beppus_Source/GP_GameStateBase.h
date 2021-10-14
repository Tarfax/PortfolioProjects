// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GP_Minions/GP_MinionMasterMindBigBrainController.h"

#include "GP_GameStateBase.generated.h"

class AGP4GameModeBase;
class AGP_PuzzlePiece_Portal;

UCLASS()
class GP4_API AGP_GameStateBase : public AGameStateBase {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGP_GameStateBase();
	UPROPERTY(Transient, BlueprintReadOnly)
	AGP_MinionMasterMindBigBrainController* MasterMind;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void HandleBeginPlay() override;

	UPROPERTY()
	AGP4GameModeBase* GameMode;
	
	UPROPERTY(VisibleAnywhere)
	TArray<AGP_PuzzlePiece_Portal*> Portals;


	void AddPortal(AGP_PuzzlePiece_Portal* Portal);
	void RemovePortal(AGP_PuzzlePiece_Portal* Portal);

	virtual bool HasBegunPlay() const override;
	virtual float GetPlayerRespawnDelay( AController* Controller ) const override;
	virtual void ReceivedGameModeClass() override;
	UFUNCTION()
	void HandlePortalsOnRestart();
	virtual void Reset() override;

};
