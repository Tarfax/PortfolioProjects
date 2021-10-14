// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "GP_Pawn_Base.generated.h"

UCLASS()
class AGP_Pawn_Base : public APawn {
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGP_Pawn_Base();

// protected:
	// // Called when the game starts or when spawned
	// virtual void BeginPlay() override;

// public:
	// // Called every frame
	// virtual void Tick(float DeltaTime) override;
	//
	// // Called to bind functionality to input
	// virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void HandleUpInput(const float Value);
	virtual void HandleRightInput(const float Value);
	virtual void HandleSpaceInput();
	virtual void HandleZoomInput(float Value);


};
