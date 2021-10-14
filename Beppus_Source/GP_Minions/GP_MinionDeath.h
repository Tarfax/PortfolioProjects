// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GP_MinionMasterMindBigBrainController.h"
#include "GameFramework/Actor.h"
#include "GP_MinionDeath.generated.h"

class UDestructibleComponent;
class UGP_RestartComp;

UCLASS()
class GP4_API AGP_MinionDeath : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGP_MinionDeath();
	UPROPERTY(EditAnywhere, BlueprintAssignable, BlueprintCallable)
	FGeneralPurposeDelegate MinionDeathCompleteDelegate;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bHasBegunDeath = false;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
    UFUNCTION(BlueprintCallable)
	void BeginMinionDeath(AGP_Minion* Minion);
	UFUNCTION(BlueprintImplementableEvent)
	void OnBeginMinionDeath(AGP_Minion* Minion);
	UFUNCTION(BlueprintCallable)
	void DestroyDestructible();
	UFUNCTION(BlueprintCallable)
	void EndMinionDeath();
	UFUNCTION(BlueprintImplementableEvent)
	void OnDestroyDestructible();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(ExposeOnSpawn=true))
	AGP_Minion* MinionToKill;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* SceneComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UDestructibleComponent* DestructibleMesh;

	// UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	// UGP_RestartComp* RestartComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool UseCustomImpulse;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CustomImpulse;
};
