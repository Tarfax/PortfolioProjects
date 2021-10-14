// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GP_DynamicObject.generated.h"

class UGP_RestartComp;
UCLASS()
class GP4_API AGP_DynamicObject : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGP_DynamicObject();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Level Handling")
	UGP_RestartComp* RestartComp;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Tick( float DeltaTime ) override;
	
	UPROPERTY(BlueprintReadWrite, Category="Level Animation")
	FVector StartAnimationLocation;
	UPROPERTY(BlueprintReadWrite,Category="Level Animation")
	FVector EndAnimationLocation;
	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category="Level Animation")
	bool AnimateIn;
	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category="Level Animation")
	float AnimationEnterExp;
	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category="Level Animation")
	bool AnimateOut;
	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category="Level Animation")
	float AnimationExitExp;
	UPROPERTY(BlueprintReadWrite, Category="Level Animation")
	float AnimationTime;
	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category="Level Animation")
	float RandomMin;
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category="Level Animation")
	float RandomMax;
	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category="Level Animation")
	float TravelOffsetZMin;
	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category="Level Animation")
	float TravelOffsetZMax;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Level Animation")
	float TravelOffset = 2000.f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category="Level Design")
	bool CenterX;
};
