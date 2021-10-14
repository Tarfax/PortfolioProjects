// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GP_RestartComp.h"
#include "Components/ActorComponent.h"
#include "Containers/Map.h"
#include "GP4/GP_Puzzles/GP_DynamicObject.h"
#include "GP_DynamicObjectHandlerComponent.generated.h"

USTRUCT()
struct FGP_PriorityGroup {
	GENERATED_BODY()
	UPROPERTY()
	int priority;
	UPROPERTY()
	TArray<UGP_RestartComp*> Objects;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GP4_API UGP_DynamicObjectHandlerComponent : public UActorComponent {
	GENERATED_BODY()

public:
	UGP_DynamicObjectHandlerComponent();

protected:
	virtual void BeginPlay() override;
	UPROPERTY(Transient)
	TArray<UGP_RestartComp*> RequestedObjects;
	UPROPERTY()
	int CurrentKey = 0;
	UPROPERTY()
	TMap<int, FGP_PriorityGroup> PriorityMap;
public:

	
	UFUNCTION()
	void ReceiveAnnounce(UGP_RestartComp* Object);
	UFUNCTION()
	void ReceiveUnAnnounce(UGP_RestartComp* Object);

	//LOAD STUFF
	UFUNCTION(BlueprintCallable)
	void RequestLoad();
	UFUNCTION()
	void LoadPriorityGroup(int PriorityKey);
	void CheckIfCallbackComplete();
	UFUNCTION()
	void LoadCompleteCallback(UGP_RestartComp* DynamicObject);
	UFUNCTION()
	void HandleAllObjectsLoaded();
	UPROPERTY(BlueprintAssignable, Category="Events")
	FLoadUnloadDelegate OnAllObjectsLoaded;
	bool bIsLoading;

	//UNLOAD STUFF
	bool bWithReset = false;
	UFUNCTION(BlueprintCallable)
	void RequestUnload(bool bWithResetAfterUnload);
	UFUNCTION()
	void UnloadPriorityGroup(int PriorityKey);
	UFUNCTION()
	void UnloadCompleteCallback(UGP_RestartComp* DynamicObject);
	UFUNCTION()
	void HandleAllObjectsUnloaded();
	UPROPERTY(BlueprintAssignable, Category="Events")
	FLoadUnloadDelegate OnAllObjectsUnloaded;
	bool bIsUnloading;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UMatineeCameraShake> ResetLevelShakeClass;
	//RESET STUFF
	UFUNCTION()
	void ResetDynamicObjects();

	UFUNCTION()
	void HandleAllObjectsReset();

	UPROPERTY(BlueprintAssignable, Category="Events")
	FLoadUnloadDelegate OnAllObjectsReset;
};
