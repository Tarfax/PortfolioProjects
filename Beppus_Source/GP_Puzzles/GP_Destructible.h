// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//Dsiables a deprication warning that is annoying
#pragma warning(disable : 4996)

#include "CoreMinimal.h"

#include "GP_DestructibleMesh.h"
#include "GP_DynamicObject.h"
#include "GP_Destructible.generated.h"

class UDestructibleComponent;
class UBoxComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExploded);



UCLASS()
class GP4_API AGP_Destructible : public AGP_DynamicObject {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGP_Destructible();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void Explode(float BaseDamage, FVector HitLocation, float DamageRadius, float ImpulseStrength = 10000, bool FullDamage = true);
	UFUNCTION(BlueprintImplementableEvent)
	void OnExplode();
	UFUNCTION(BlueprintImplementableEvent)
	void OnDestroyDestructible();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* SceneComponent;
	// UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	// UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UDestructibleComponent* DestructibleMeshComp;

	UPROPERTY(BlueprintAssignable)
	FOnExploded OnExploded;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool UseCustomImpulse;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CustomImpulse;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool IsDestroyed;	

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGP_DestructibleMesh> DestructibleMeshClass;
};
