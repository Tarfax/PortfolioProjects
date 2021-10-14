// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GP_WorldLightInterpolation.h"
#include "GameFramework/Actor.h"
#include "GP_LightData.generated.h"

UCLASS()
class GP4_API AGP_LightData : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGP_LightData();
 
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FLightComponents LightComponents;

	UFUNCTION(BlueprintCallable, CallInEditor)
	void DeactivateLight();

	
	UFUNCTION(BlueprintCallable, CallInEditor)
	void ActivateLight();
	
};
