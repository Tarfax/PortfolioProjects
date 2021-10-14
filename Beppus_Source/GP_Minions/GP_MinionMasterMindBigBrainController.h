// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "GP_MinionMasterMindBigBrainController.generated.h"

class AGP_MinionSpawnPortal;
class AGP_Minion;
class UGP_MinionCommand_Base;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCameraShakeDelegate, TSubclassOf<UMatineeCameraShake>, ShakeClass);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGeneralPurposeDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCallAllMinionsDelegate, FVector, TargetLocation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCallExplodeAllMinionsDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMinionSpawnedDelegate, AGP_Minion*, SpawnedMinion);
UCLASS(Transient)
class GP4_API AGP_MinionMasterMindBigBrainController : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGP_MinionMasterMindBigBrainController();

	UPROPERTY(BlueprintCallable, Transient)
	FCallAllMinionsDelegate AllMinionsMoveToDelegate;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Transient)
	FCameraShakeDelegate BeginCameraShakeDelegate;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Transient)
	FGeneralPurposeDelegate EndCameraShakeDelegate;

	UPROPERTY(BlueprintCallable, Transient)
	FCallExplodeAllMinionsDelegate AllMinionsExplodeDelegate;
	UPROPERTY(BlueprintReadOnly, Transient, VisibleInstanceOnly)
	TArray<AGP_Minion*> Minions;
	UPROPERTY(BlueprintReadOnly, Transient, VisibleAnywhere)
	TArray<AGP_Minion*> MinionsInGoal;
	UPROPERTY(BlueprintReadWrite, Transient, EditAnywhere)
	TArray<AGP_MinionSpawnPortal*> MinionSpawnPortals;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay( const EEndPlayReason::Type EndPlayReason ) override;
public:
	static AGP_MinionMasterMindBigBrainController* Get();
	// Called every frame
	virtual void Tick( float DeltaTime ) override;
	UFUNCTION(BlueprintCallable)
	void MinionEnteredGoal( AGP_Minion* MinionInGoal );
	UFUNCTION(BlueprintCallable)
	bool IsMinionPortalsDoneSpawning();
	UFUNCTION()
	virtual void Reset() override;

	UPROPERTY(BlueprintAssignable)
	FGeneralPurposeDelegate OnMinionEnteredGoal;

	UPROPERTY(BlueprintAssignable)
	FGeneralPurposeDelegate OnMinionDied;

	UPROPERTY(BlueprintAssignable)
	FGeneralPurposeDelegate OnAllMinionsSpawned;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnMinionSpawnedDelegate OnMinionSpawnedDelegate;
};
