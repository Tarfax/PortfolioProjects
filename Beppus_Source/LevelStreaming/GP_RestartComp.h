// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GP_RestartComp.generated.h"

class AGP4GameModeBase;
class AGP_DynamicObject;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLoadUnloadDelegate);
UCLASS(ClassGroup=("Restart"), meta=(BlueprintSpawnableComponent))
class GP4_API UGP_RestartComp : public UActorComponent {
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGP_RestartComp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void EndPlay( const EEndPlayReason::Type EndPlayReason ) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Resets")
	float DelayTime = 0.5f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Resets")
	bool bUseTimer = true;

public:
	UPROPERTY()
	AGP4GameModeBase* GameMode;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Resets")
	int Priority;
	
	void AnnounceSelf();
	void UnAnnounceSelf();

	UPROPERTY()
	FTimerHandle TimerHandle;
	
	//Load
	void HandleLoadRequest();
	UFUNCTION(BlueprintCallable, Category="Events")
	void LoadComplete();
	UPROPERTY(BlueprintAssignable, Category="Events")
	FLoadUnloadDelegate OnLoadRequest;
	UPROPERTY(BlueprintAssignable, Category="Events")
	FLoadUnloadDelegate OnLoadComplete;

	//Unload
	void HandleUnloadRequest();
	UFUNCTION(BlueprintCallable, Category="Events")
	void UnloadComplete();
	UPROPERTY(BlueprintAssignable, Category="Events")
	FLoadUnloadDelegate OnUnloadRequest;
	UPROPERTY(BlueprintAssignable, Category="Events")
	FLoadUnloadDelegate OnUnloadComplete;
	

};
