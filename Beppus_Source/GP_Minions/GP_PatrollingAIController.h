// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GP_AIMinionController.h"

#include "GameFramework/Actor.h"
#include "GP_PatrollingAIController.generated.h"

class UGP_MinionCommand_Base;
UCLASS()
class GP4_API AGP_PatrollingAIController : public AGP_AIMinionController {
	GENERATED_BODY()

public:
	AGP_PatrollingAIController();
	virtual void OnPossess( APawn* InPawn ) override;
	virtual void OnUnPossess() override;
	virtual void Reset() override;
	UFUNCTION(BlueprintCallable)
	void BeginPatrolMove();
	UFUNCTION(BlueprintCallable)
	void EndPatrolMove();
	UPROPERTY(Transient)
	AGP_Minion* Minion;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TEnumAsByte<ECollisionChannel>> CollisionChannels;
	/**
	 * @brief Show debug logs and draw lines?
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MinionAI")
	bool bDebug = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MinionAI")
	float TraceLength = 120.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MinionAI")
	bool bShouldBeMoving = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MinionAI")
	float SecondTraceHeightOffset = 60.f;

	FCollisionObjectQueryParams COQP;
protected:

	UPROPERTY(VisibleInstanceOnly, Category="MinionAI")
	FVector CurrentDirection = FVector::RightVector;

	virtual void BeginPlay() override;
	virtual void EndPlay( const EEndPlayReason::Type EndPlayReason ) override;

public:
	virtual void Tick( float DeltaTime ) override;
protected:
	void TurnAround();

	void SetCollisionQueryParams();
	virtual void HandleMove() override;
	virtual void ResumeMove( FVector Direction ) override;
	virtual void PauseMove( FVector Direction ) override;
	virtual void FinishMove() override;

};
