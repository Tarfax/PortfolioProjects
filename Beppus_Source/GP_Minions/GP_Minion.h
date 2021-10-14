// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GP_MinionDeathComponent.h"

#include "GP4/GP_Character/GP_Pawn_Base.h"
#include "GP4/GP_Minions/GP_MinionCommands/EGP_MinionCommand.h"
#include "GP_MinionCommands/GP_MinionCommand_Base.h"
#include "GP_Minion.generated.h"

class AGP_PatrollingAIController;
class AGP_PuzzlePiece_Bomb;
class AGP_MinionMasterMindBigBrainController;
class UBoxComponent;
class UCapsuleComponent;
class AGP_PlayerController;
class AGP_AIMinionController;
class AGP_LedgePlatform;
class UGP_RestartComp;

UCLASS()
class GP4_API AGP_Minion : public AGP_Pawn_Base {

	GENERATED_BODY()

public:
	AGP_Minion();

	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UBoxComponent* BoxComponent;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UCapsuleComponent* CapsuleComponent;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	USkeletalMeshComponent* SkeletalMeshComponent;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UGP_MinionDeathComponent* DeathComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Level Handling")
	UGP_RestartComp* RestartComp;
private:
	UPROPERTY()
	bool bIsPossessedByPlayer = false;
public:
	UFUNCTION(BlueprintCallable)
	bool IsPossessedByPlayer();
	UFUNCTION(BlueprintCallable)
	void SetIsPossessedByPlayer(bool value);
	//Commands
	UPROPERTY(Transient,BlueprintReadOnly)
	UGP_MinionCommand_Base* CurrentCommand;
	UFUNCTION(BlueprintCallable)
	void CancelCommand();

	TArray<AGP_LedgePlatform*> LedgePlatforms;

	UFUNCTION(BlueprintCallable)
	AGP_MinionDeath* Kill(TSubclassOf<AGP_MinionDeath> DeathType);
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsPendingKill = false;
	UFUNCTION(BlueprintImplementableEvent)
	void OnKill(AGP_MinionDeath* DeathType);
	UFUNCTION(BlueprintCallable)
	void DestroyMinion();

	UFUNCTION(BlueprintImplementableEvent)
	void OnJump();
	UFUNCTION(BlueprintImplementableEvent)
	void OnMove(float Vel);

	UFUNCTION(BlueprintCallable)
	bool ReceivedCommand(TSubclassOf<UGP_MinionCommand_Base> Command,bool ForceCommand = false);

	UFUNCTION()
	void MouseOver(int StencilValue, TSubclassOf<UGP_MinionCommand_Base> Command);
	UFUNCTION(BlueprintImplementableEvent)
	void OnMouseOver(TSubclassOf<UGP_MinionCommand_Base> Command);
	UFUNCTION()
	void MouseOverEnd();
	UFUNCTION(BlueprintImplementableEvent)
	void OnMouseOverEnd();
	UFUNCTION(BlueprintImplementableEvent)
	void OnRecieveBomb(AGP_PuzzlePiece_Bomb* Bomb);
	UFUNCTION(BlueprintImplementableEvent)
	void OnKillWithDelay();
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	AGP_PatrollingAIController* MinionAiController;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	AGP_PlayerController* PlayerController;

	UPROPERTY(BlueprintReadOnly, Transient, VisibleAnywhere)
	AGP_MinionMasterMindBigBrainController* MasterMind;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSubclassOf<AGP_MinionDeath> FallDamageDeathClass;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bDebug = false;

	UFUNCTION(BlueprintImplementableEvent)
	void OnGetPossessedByAI();
	UFUNCTION(BlueprintImplementableEvent)
	void OnGetPossessedByPlayer();

	UFUNCTION(BlueprintCallable)
	void ChangeMovementSpeed(float newValue);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:
	virtual void Tick(float DeltaTime) override;
	virtual void HandleUpInput(const float Value) override;
	virtual void HandleRightInput(const float Value) override;
	virtual void HandleSpaceInput() override;

	UPROPERTY(BlueprintReadWrite)
	FVector2D Velocity;
	UPROPERTY(BlueprintReadWrite)
	FVector2D Acceleration;

	FVector CurrentSurfaceNormal;

	UPROPERTY(BlueprintReadWrite)
	bool IsTeleporting;	


	bool bIsOnGround = false;
	bool bExternalIsOnGround = true;
	bool bIsJumping = false;
	bool IsJumpQueued = false;
	bool IsInputEnabled = true;
	UPROPERTY(BlueprintReadWrite)
	bool IsMovementEnabled = true;

	int AirFrames;


	float PreJumpTimer = 0;
	float AirJumpGraceTimer = 0;
	float MovementInput;
	float FallStartHeight;

	void HandleMovement(float DeltaTime);
	void HandleCollision(FVector MoveDelta, float DeltaTime);
	void InternalGroundCheck(FHitResult SweepHitResults);
	void ExternalGroundCheck();
	bool ShouldUseStep();
	UFUNCTION(BlueprintCallable)
	void ResetMinionFallHeight();
	UFUNCTION(BlueprintCallable)
	bool GetIsGrounded();
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="MinionAI")
	bool bStartWithAIMovementEnabled = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Movement")
	float Gravity = 1800.f;
	UPROPERTY(EditAnywhere, Category="Movement")
	float JumpStrength = 1000.f;
	UPROPERTY(VisibleInstanceOnly, Category="Movement")
	float MovementSpeed = 125.f;
	UPROPERTY(EditAnywhere, Category="Movement")
	float StepHeight = 10.f;
	UPROPERTY(EditAnywhere, Category="Movement")
	float WalkableAngle = 45.f;
	UPROPERTY(EditAnywhere, Category="Movement")
	float AIMovementSpeed = 70.f;
	UPROPERTY(EditAnywhere, Category="Movement")
	float HoverMovementSpeed = 25.f;
	UPROPERTY(EditAnywhere, Category="Movement")
	float PlayerMovementSpeed = 125.f;
	UPROPERTY(EditAnywhere, Category="Movement")
	float AccelerationTime = .1f;
	UPROPERTY(EditAnywhere, Category="Movement")
	float RetardationTime = .1f;
	UPROPERTY(EditAnywhere, Category="Movement")
	float ReactivePercentage = 0.5f;
	UPROPERTY(EditAnywhere, Category="Movement")
	float AirControl = 0.5f;

	UPROPERTY(EditAnywhere, Category="Movement")
	float PreJumpTime = 0.2;

	UPROPERTY(EditAnywhere, Category="Movement")
	float AirJumpGraceTime = 0.1f;

	UPROPERTY(EditAnywhere, Category="Movement")
	float FallDamageLengt = 1000.f;
	UPROPERTY(BlueprintReadWrite)
	bool bIsFallingToDeath;
	UFUNCTION(BlueprintImplementableEvent)
	void OnStartFallingToDeath();
	UFUNCTION(BlueprintImplementableEvent)
	void OnDiedOnFall();
	UFUNCTION(BlueprintImplementableEvent)
	void OnSurvivedFall();

	UPROPERTY(EditAnywhere, Category="Movement")
	float PushSpeed = 75.f;
	UPROPERTY(EditAnywhere, Category="Movement")
	float ClimbDuration = 1;
	UPROPERTY(EditAnywhere, Category="Movement")
	float ClimbCorrectionSpeed = 200;
	UPROPERTY(EditAnywhere, Category="Movement")
	FVector2D ClimbRange = FVector2D(200, 0);

	UPROPERTY(EditDefaultsOnly, Category="Collision")
	TEnumAsByte<ECollisionChannel> GroundCheckCollisionChannels;
	UPROPERTY(EditDefaultsOnly, Category="Collision")
	float GroundCheckDistance = 50;

	void Jump();

};
