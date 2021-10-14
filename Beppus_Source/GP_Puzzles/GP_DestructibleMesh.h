// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GP_DestructibleMesh.generated.h"

class UDestructibleComponent;

UCLASS()
class GP4_API AGP_DestructibleMesh : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGP_DestructibleMesh();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UDestructibleComponent* DestructibleMeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* SceneComponent;

	UFUNCTION(BlueprintCallable)
	void Explode(float BaseDamage, FVector HitLocation, float DamageRadius, float ImpulseStrength = 10000, bool FullDamage = true);

};
