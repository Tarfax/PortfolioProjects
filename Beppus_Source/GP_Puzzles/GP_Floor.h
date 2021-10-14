// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GP_DynamicObject.h"

#include "GameFramework/Actor.h"
#include "GP_Floor.generated.h"

UCLASS()
class GP4_API AGP_Floor : public AGP_DynamicObject {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGP_Floor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick( float DeltaTime ) override;



};
