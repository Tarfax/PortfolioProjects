﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GP_LevelBoundComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GP4_API UGP_LevelBoundComponent : public USceneComponent {
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGP_LevelBoundComponent();
#if WITH_EDITOR
	virtual  void PostEditComponentMove(bool bFinished) override;
#endif

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;


};
