// I like sand

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIVisitor/AIVisitor.h"
#include "AILifeguard/AILifeGuard.h"
#include "AIBeach_Controller_Base.generated.h"

class ABuildingBase;
class AAI_BeachArea;
class AAI_BeachCharacter;
class UAIBeach_Observer;
// class ABuildingBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetDebugText, FString, DebugText);

UCLASS(Abstract)
class TEAM9ASSEMBLE_API AAIBeach_Controller_Base : public AAIController {
	GENERATED_BODY()

public:
	AAIBeach_Controller_Base();
	void SetDecorationValue();

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	virtual void Start() { }

	// virtual void BeginPlay() override;
	// virtual void Tick(float DeltaTime) override;

public: //Inherentstuff
	virtual void PresaveRuntimeCurvesIntoRichCurves() {}

	UFUNCTION(BlueprintCallable)
	virtual void MakeDecision() {}

	// UFUNCTION(BlueprintCallable)
	// virtual void FailedToExecuteTask() {}

	bool ReservBuilding(ABuildingBase* Building);


	bool StartInteractWithBuilding() ;

	void StopInteractWithBuilding() ;

	UPROPERTY(BlueprintAssignable)
	FSetDebugText SetDebugText;

	UPROPERTY(BlueprintReadWrite, Category="AI Debug")
	FString DebugText;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="AI Task")
	FString DoingTask;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAIBeach_Observer* AIObserver;

	UPROPERTY(BlueprintReadOnly, Category="AIVisitor ReadOnly")
	AAI_BeachCharacter* AI_BeachCharacter;

	UPROPERTY(EditDefaultsOnly, Category="AI Default")
	UBehaviorTree* BTAsset;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="AIVisitor ReadOnly")
	AAI_BeachArea* BeachArea;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="AIVisitor Water")
	bool IsInWater;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ABuildingBase* InteractingBuilding;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ABuildingBase* ReservedBuilding;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Decoration")
	int DecorationValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Decoration")
	float DecorationSearchRadius = 1000.0f;

	float DecorationValueTimer = 30.0f;
	float DecorationValueTime = 30.0f;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="AI Building")
	bool IsInteractingWithBuilding;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="AI Building")
	bool IsHeadingForBuilding;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="AI Movement")
	float NormalMoveSpeed = 220;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="AI Movement")
	float RunMoveSpeed = 650;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="AI Movement")
	float SlowMoveSpeed = 120;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="AI Movement")
	float LastMoveSpeed = 220;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="AI Movement")
	float SwimMoveSpeed = 80;

	TArray<TEnumAsByte<EObjectTypeQuery>> OverlapObjectTypes;
	UPROPERTY(EditDefaultsOnly)
	TArray<TEnumAsByte<EObjectTypeQuery>> OverlapTaskTypes;
	
	UFUNCTION(BlueprintCallable)
	void OnMovingStateChanged(bool NewState);
	UFUNCTION(BlueprintImplementableEvent)
	void OnMovingStateChangedEvent(bool NewState);

	UFUNCTION(BlueprintCallable)
	void OnIsInWaterChanged(bool NewState);
	UFUNCTION(BlueprintImplementableEvent)
	void OnIsInWaterChangedEvent(bool NewState);

	UFUNCTION(BlueprintImplementableEvent)
	void SetNewMoveSpeed(float NewMoveSpeed);

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool IsWet;
	float WetTimer;

	UPROPERTY()
	UBlackboardComponent* BlackBoardAccessor;
};
