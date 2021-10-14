// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "GP4/GP_Minions/GP_Minion.h"

#include "GP_LedgePlatform.generated.h"

class USplineComponent;
class AGP_Minion;

UCLASS()
class GP4_API AGP_LedgePlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGP_LedgePlatform();
	UPROPERTY(EditAnywhere)
	USplineComponent* LedgeSpline;
	UPROPERTY(EditAnywhere)
	USceneComponent* LedgePoint; 

	float ClimbSpeed;
	float CourseCorrectionDistance;
	FVector CourseCorrection;
	bool IsClimbing = false;
	bool IsMovingOntoSpline = false;
	AGP_Minion* MinionToClimb;
	FVector StartPos;

	float CurrentClimbTime;

	void BeginLedgeClimb(AGP_Minion*);
	AGP_LedgePlatform* CheckForPossibleClimb(AGP_Minion*);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
