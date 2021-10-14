// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GP_MinionMasterMindBigBrainController.h"

#include "GP_AIMinionController.generated.h"

UCLASS()
class GP4_API AGP_AIMinionController : public AAIController {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGP_AIMinionController();

	virtual void OnPossess( APawn* InPawn ) override;
	virtual void OnUnPossess() override;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="MinionAI")
	TEnumAsByte<ECollisionChannel> CollisionChannel;



	UPROPERTY(Transient)
	AGP_MinionMasterMindBigBrainController* MasterMind;
	/**
	 * @brief Modifies the input given to the minion thus affecting its speed.
	 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="MinionAI")
	float AIMovementSpeed = 0.5f;
	UPROPERTY(VisibleInstanceOnly, Category="MinionAI")
	bool bIsAtTargetLocation = false;
	UPROPERTY(VisibleInstanceOnly, Category="MinionAI")
	bool bHasMoveCommandIssued = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="MinionAI")
	float MoveCommandTime = 10.f;
	UPROPERTY(VisibleInstanceOnly, Category="MinionAI")
	float MoveCommandTimer = 5.f;
	/**
	 * @brief How close to the target location does the Minion need to be to be considered FinishedMove.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="MinionAI")
	float MoveToRange = 50.f;
	/**
	 * @brief Is the minion pausing its move, ex. if its falling, it will resume once grounded again.
	 */
	UPROPERTY(VisibleInstanceOnly, Category="MinionAI")
	bool bIsPausing = false;
protected:
	UPROPERTY(VisibleInstanceOnly, Category="MinionAI")
	FVector TargetMoveLocation = FVector::RightVector;

	virtual void BeginPlay() override;
	virtual void EndPlay( const EEndPlayReason::Type EndPlayReason ) override;
public:
	virtual void Tick( float DeltaTime ) override;

	/**
	 * @brief Used by ExplodeAllMinionsDelegate on the masterMind.
	 * @param ExplodeCommand
	 */
protected:
	UFUNCTION(BlueprintCallable)
	void ExplodeMinion( TSubclassOf<UGP_MinionCommand_Base> ExplodeCommand );

	/**
	 * @brief Initiates a Movement command.
	 * @param TargetLocation The location recieved in the Command
	 */
	UFUNCTION()
	virtual void MinionMoveTo( FVector TargetLocation );
	virtual void AddTargetLocationOffset();
	/**
	 * @brief Handles the whole AI MovementLoop and runs in Tick IF we have a movementCommand.
	 */
	virtual void HandleMove();
	//Below methods handles the "state" of movement;
	virtual void ResumeMove( FVector Direction );
	virtual void PauseMove( FVector Direction );

	void EndMoveEarly();

	UFUNCTION()
	virtual void FinishMove();

};
