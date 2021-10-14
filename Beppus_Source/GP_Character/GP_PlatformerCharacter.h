// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Pawn.h"
#include "GP_PlatformerCharacter.generated.h"

class UCapsuleComponent;

UCLASS()
class GP4_API AGP_PlatformerCharacter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGP_PlatformerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* CapsuleCollider;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void MoveRight(float value);
	void Jump();

	FVector2D Velocity;
	FVector2D Acceleration;

	FVector CurrentSurfaceNormal;

	bool bIsOnGround = false;
	bool bIsJumping = false;
	bool IsJumpQueued = false;
	
	float PreJumpTimer = 0;
	float AirJumpGraceTimer = 0;
	float MovementInput;


	UPROPERTY(EditAnywhere)
	float Gravity = 1800.f;
	UPROPERTY(EditAnywhere)
	float JumpStrength = 1000.f;
	UPROPERTY(EditAnywhere)
	float MovementSpeed = 100.f;
	UPROPERTY(EditAnywhere)
	float AccelerationTime = .1f;
	UPROPERTY(EditAnywhere)
	float RetardationTime = .1f;
	UPROPERTY(EditAnywhere)
	float ReactivePercentage = 0.5f;
	UPROPERTY(EditAnywhere)
	float AirControl = 0.5f;

	UPROPERTY(EditAnywhere)
	float PreJumpTime = 0.2;

	UPROPERTY(EditAnywhere)
	float AirJumpGraceTime = 0.1f;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
