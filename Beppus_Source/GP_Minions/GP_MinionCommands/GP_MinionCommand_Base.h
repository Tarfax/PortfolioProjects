// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GP_MinionCommand_Base.generated.h"

class AGP_MinionMasterMindBigBrainController;
class AGP_Minion;


UCLASS(Blueprintable, BlueprintType)

class GP4_API UGP_MinionCommand_Base : public UObject {
	GENERATED_BODY()

public:
	UGP_MinionCommand_Base();

	/**
	 * @brief Is this a command that will be overridden before it is finished.
	 */
	UPROPERTY(BlueprintReadOnly)
	bool bIsOverridableCommand = false;
	/**
	* @brief This is the blueprint event to respond to ExecuteCommand, gets called in ExecuteCommand.
	 * @param Minion // the minion that should execute the command
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void OnExecuteCommand( AGP_Minion* Minion );

	UPROPERTY(Transient)
	AGP_MinionMasterMindBigBrainController* MasterMind;
	/**
	* @brief This is the main way to just execute any command.
	* @param Minion
	*/
	UFUNCTION(BlueprintCallable)
	virtual void ExecuteCommand( AGP_Minion* Minion );

	/**
	 * @brief This should get called when the command is FINISHED!
	 * @param Minion
	 */
	UFUNCTION(BlueprintCallable)
	void FinishCommand( AGP_Minion* Minion );

	FTimerHandle TimerHandle;
	FTimerDynamicDelegate TimerDynamicDelegate;

	UFUNCTION(BlueprintCallable)
	void StartDelay( float seconds );

	UFUNCTION(BlueprintImplementableEvent)
	void OnDelayFinished();

	/**
	 * @brief This gets called in FinishCommand to give a BP event to respond to.
	 * @param Minion
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void OnFinishedCommand( AGP_Minion* Minion );

};
