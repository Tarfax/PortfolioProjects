// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GP_DynamicObject.h"
#include "GP_SignalReceiver.generated.h"

class AActor;

USTRUCT(BlueprintType)
struct FListOfActors {
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite)
	TArray<AActor*> Actors;
};

UCLASS()
class AGP_SignalReceiver : public AGP_DynamicObject {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGP_SignalReceiver();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void Interact(TArray<AActor*> Actors);
	UFUNCTION(BlueprintImplementableEvent)
	void OnInteract(FListOfActors Actors);

	UPROPERTY(BlueprintReadWrite)
	bool IsActive;
};
