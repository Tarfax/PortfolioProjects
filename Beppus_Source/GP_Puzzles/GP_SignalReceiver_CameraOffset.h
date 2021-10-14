// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GP_SignalReceiver.h"
#include "GP_SignalReceiver_CameraOffset.generated.h"

UCLASS()
class AGP_SignalReceiver_CameraOffset : public AGP_SignalReceiver {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGP_SignalReceiver_CameraOffset();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetCameraOffset(FVector Location, float Distance, bool UseRotateToThisActor);
};
