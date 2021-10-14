// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GP_DynamicObject.h"
#include "GP_SignalSender.generated.h"

class AGP_SignalReceiver;
class AActor;

UCLASS()
class GP4_API AGP_SignalSender : public AGP_DynamicObject {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGP_SignalSender();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//
	// UFUNCTION(BlueprintImplementableEvent)
	// void OnStartInteracting();

	UFUNCTION(BlueprintCallable, CallInEditor)
	void Interact(TArray<AActor*> Actors);

	UFUNCTION(BlueprintImplementableEvent)
	void OnInteract();
	
	UFUNCTION(BlueprintCallable, CallInEditor)
	void Break();

	UFUNCTION(BlueprintImplementableEvent)
	void OnBreak();
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TArray<AGP_SignalReceiver*> SignalReceivers;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool IsActive;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool IsBroken;
};
