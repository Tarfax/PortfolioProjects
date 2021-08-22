// I like sand

#pragma once

#include "CoreMinimal.h"
#include "Team9Assemble/AI/AIBeach_Controller_Base.h"
#include "Team9Assemble/AI/BehaviourTree/Tasks/BTTask_Helper.h"
#include "AIVisitor_Controller.generated.h"

class IAIVisitorTask;
class AIVisitorTask_MoveTo;
class AAI_WaterArea;
class AAILifeGuard_Controller;
class ADummyBuilding;
class ABuilding_AIHome;
class UTile;


UCLASS(BlueprintType)
class AAIVisitor_Controller : public AAIBeach_Controller_Base {
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAIVisitorEvent, AAIVisitor_Controller*, AIController);

public: // Virtuals
	AAIVisitor_Controller();
	virtual void BeginPlay() override;
	virtual void Start() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void DestroyVisitor();

public: // Functions
	void PresaveRuntimeCurvesIntoRichCurves() override;

	UFUNCTION(BlueprintCallable)
	void EvaluateHappiness();
	void EvaluateStamina();

	void MakeDecision() override;
	void FailedToExecuteTask(E_AIVisitor_TaskType TaskType, float Time);

	// void FailedToExecuteTask(float Time);

	UFUNCTION()
	ABuildingBase* SetupBeachHomeArea(UTile* Tile, ABuildingBase* Building);

	UFUNCTION(BlueprintCallable)
	bool StartSunning();

	UFUNCTION(BlueprintCallable)
	void StopSunning();

	UFUNCTION(BlueprintCallable)
	void HomeWasDestroyed();

	UFUNCTION(BlueprintCallable)
	void CheckProbabilityOfDrowning(bool InstantDrown = false);

	UFUNCTION(BlueprintCallable)
	void SavedFromDrowning(AAILifeGuard_Controller* LifeGuard);

	UFUNCTION(BlueprintCallable)
	void LeaveAtBuilding(ABuildingBase* Building);

	UFUNCTION(BlueprintCallable)
	void Die();

	UFUNCTION(BlueprintCallable)
	void BeginTransitionIntoWater();

	UFUNCTION(BlueprintCallable)
	void TransitionIntoWater();
	void BeginTransitionOutOfWater();
	void TransitionOutOfWater();

	UFUNCTION(BlueprintImplementableEvent)
	void SetAnimation(EAI_CustomAnimations NewAnimation);

	/*** State Machine ***/
	template <typename T, typename = std::enable_if_t<std::is_base_of<IAIVisitorTask, T>::value>>
	UFUNCTION(BlueprintCallable)
	void ChangeToState() {
		SwitchToNewTask = true;
		NextTask = new T(this, TaskData, &Mood, &Need);
	}

	void SwitchTask();
	void SetNewTaskInBehaviourTree(E_AIVisitor_TaskType NewBlackboardState) const;
	E_AIVisitor_TaskType CheckForHighestNeedValue();

	FText GetBehaviourEnumAsString(E_AIVisitor_TaskType EnumValue) {
		const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("E_AIVisitor_TaskType"), true);
		if (!EnumPtr) return NSLOCTEXT("Invalid", "Invalid", "Invalid");
		return EnumPtr->GetDisplayNameTextByIndex((int32)EnumValue);
	}

	bool IsTaskLocked(E_AIVisitor_TaskType Task);
	bool WrapperContains(E_AIVisitor_TaskType Task);
	F_AIVisitor_TaskDriverWrapper* GetWrapper(E_AIVisitor_TaskType Task);

public: //Variables

	bool IsGoingForASwim;
	bool IsGoingOutOfWater;

	/*** Curves ***/
	UPROPERTY()
	FCompressedRichCurve BodyTemperatureRuntimeCurve;
	UPROPERTY()
	FCompressedRichCurve ToiletRuntimeCurve;
	UPROPERTY()
	FCompressedRichCurve HungerRuntimeCurve;
	UPROPERTY()
	FCompressedRichCurve ThirstRuntimeCurve;
	UPROPERTY()
	FCompressedRichCurve DrownRuntimeCurve;

	// #if WITH_EDITORONLY_DATA
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="AI Decision Curves")
	float SunningThreshold;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="AI Decision Curves")
	FRuntimeFloatCurve BodyTemperatureCurve;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="AI Decision Curves")
	FRuntimeFloatCurve ToiletCurve;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="AI Decision Curves")
	FRuntimeFloatCurve HungerCurve;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="AI Decision Curves")
	FRuntimeFloatCurve ThirstCurve;
	UPROPERTY()
	FRuntimeFloatCurve DrownCurve;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="AI Happiness")
	F_AIVisitor_HappinessCurves HappinessCurves;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="AI Happiness")
	float EvaluateHappinessTime = 10.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="AI Happiness")
	float EvaluateMoodTimer;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="AI Happiness")
	int TaskEnergyMin = 20;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="AI Happiness")
	int TaskEnergyMax = 30;
	// #endif

	/*** State Machine ***/
	IAIVisitorTask* RunningTask;
	IAIVisitorTask* NextTask;
	bool SwitchToNewTask;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="AI Task")
	E_AIVisitor_TaskType PerformingTask;

	UPROPERTY(VisibleAnywhere, Category="AI Task", AdvancedDisplay, meta = (DisplayName ="Tasks On Cooldown"))
	TArray<F_AIVisitor_TaskDriverWrapper> TaskWrappers;
	bool TasksAreUnderLockdown;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="AI Task", AdvancedDisplay)
	float TaskTimerLockdownTimer;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="AI Task")
	float TaskTimerLockdownTime = 10.0f;

	/*** AI Variables ***/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="AIVisitor ReadOnly")
	AAI_WaterArea* WaterArea;


	UPROPERTY(EditDefaultsOnly, Category="AI Home")
	TSubclassOf<ADummyBuilding> DummyHomeBlueprint;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="AI Home")
	ABuildingBase* Home;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="AI Home")
	bool HasHome;
	bool HasSunbench;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI Task", meta = (DisplayAfter="DoingTask"))
	F_AIVisitor_Mood Mood;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI Task", meta = (DisplayAfter="DoingTask"))
	F_AIVisitor_Need Need;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI Task", meta = (DisplayAfter="DoingTask"))
	F_AIVisitor_Info Info;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="AIVisitor DataAssets")
	U_AIVistor_TaskData* TaskData;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="AI Task", AdvancedDisplay)
	TMap<E_AIVisitor_TaskType, float> TaskEvaluationResults;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="AIVisitor")
	bool IsAlive;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="AIVisitor Water")
	bool IsDrowning;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="AIVisitor Water")
	bool IsBeingRescued;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="AIVisitor Water")
	AAILifeGuard_Controller* LifeGuardRescuer;

	float StartTimer = 2.0f;
	bool HasStarted;


	UFUNCTION(BlueprintCallable, Category="AI BUFF/DEBUFF")
	void SetModifier(E_AIVisitor_ModifierType ModifierType);
	void SetDebuffAfterTaskFailed(E_AIVisitor_TaskType TaskType);
	F_AIVisitor_Modifier* GetModifier(E_AIVisitor_ModifierType ModifierType);
	void SetTaskOnCooldown(E_AIVisitor_TaskType TaskType, float Time);

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="AIVisitor Mood")
	TArray<F_AIVisitor_Modifier> Modifiers;

	UPROPERTY(EditDefaultsOnly)
	U_AINameGenerator* VisitorNames;

	//Callbacks
	UFUNCTION(BlueprintImplementableEvent)
	void OnBeginDrownEvent();
	UPROPERTY(BlueprintAssignable)
	FAIVisitorEvent OnBeginDrown;
	UFUNCTION(BlueprintImplementableEvent)
	void OnEndDrownEvent();
	UPROPERTY(BlueprintAssignable)
	FAIVisitorEvent OnEndDrown;

	UFUNCTION(BlueprintImplementableEvent)
	void OnDeathEvent();
	UPROPERTY(BlueprintAssignable)
	FAIVisitorEvent OnDeath;

	UFUNCTION(BlueprintImplementableEvent)
	void OnVeryUnhappyEvent();
	UPROPERTY(BlueprintAssignable)
	FAIVisitorEvent OnVeryUnhappy;

	UFUNCTION(BlueprintImplementableEvent)
	void OnBeginSunningEvent();
	UPROPERTY(BlueprintAssignable)
	FAIVisitorEvent OnBeginSunning;

	UFUNCTION(BlueprintImplementableEvent)
	void OnEndSunningEvent();
	UPROPERTY(BlueprintAssignable)
	FAIVisitorEvent OnEndSunning;

	UFUNCTION(BlueprintImplementableEvent)
	void OnBeginSunningBenchEvent();
	UPROPERTY(BlueprintAssignable)
	FAIVisitorEvent OnBeginSunningBench;

	UFUNCTION(BlueprintImplementableEvent)
	void OnEndSunningBenchEvent();
	UPROPERTY(BlueprintAssignable)
	FAIVisitorEvent OnEndSunningBench;

	UFUNCTION(BlueprintImplementableEvent)
	void OnBeginDrinkingEvent();
	UPROPERTY(BlueprintAssignable)
	FAIVisitorEvent OnBeginDrinking;

	UFUNCTION(BlueprintImplementableEvent)
	void OnEndDrinkingEvent();
	UPROPERTY(BlueprintAssignable)
	FAIVisitorEvent OnEndDrinking;

	UFUNCTION(BlueprintImplementableEvent)
	void OnBeginBathingEvent();
	UPROPERTY(BlueprintAssignable)
	FAIVisitorEvent OnBeginBathing;

	UFUNCTION(BlueprintImplementableEvent)
	void OnEndBathingEvent();
	UPROPERTY(BlueprintAssignable)
	FAIVisitorEvent OnEndBathing;

	UFUNCTION(BlueprintImplementableEvent)
	void OnBeginEatingEvent();
	UPROPERTY(BlueprintAssignable)
	FAIVisitorEvent OnBeginEating;

	UFUNCTION(BlueprintImplementableEvent)
	void OnEndEatingEvent();
	UPROPERTY(BlueprintAssignable)
	FAIVisitorEvent OnEndEating;

	UFUNCTION(BlueprintImplementableEvent)
	void OnBeginExcrementingEvent();
	UPROPERTY(BlueprintAssignable)
	FAIVisitorEvent OnBeginExcrementing;

	UFUNCTION(BlueprintImplementableEvent)
	void OnEndExcrementingEvent();
	UPROPERTY(BlueprintAssignable)
	FAIVisitorEvent OnEndExcrementing;

	UFUNCTION(BlueprintImplementableEvent)
	void OnBeginBeingSavedEvent();
	UPROPERTY(BlueprintAssignable)
	FAIVisitorEvent OnBeginBeingSaved;

	UFUNCTION(BlueprintImplementableEvent)
	void OnEndBeingSavedEvent();
	UPROPERTY(BlueprintAssignable)
	FAIVisitorEvent OnEndBeingSaved;

	UFUNCTION(BlueprintImplementableEvent)
	void OnBeginBeingTransportedEvent();
	UPROPERTY(BlueprintAssignable)
	FAIVisitorEvent OnBeginBeingTransported;

	UFUNCTION(BlueprintImplementableEvent)
	void OnBeginHospitalEvent();
	UPROPERTY(BlueprintAssignable)
	FAIVisitorEvent OnBeginHospital;

	UFUNCTION(BlueprintImplementableEvent)
	void OnEndHospitalEvent();
	UPROPERTY(BlueprintAssignable)
	FAIVisitorEvent OnEndHospital;

	UFUNCTION(BlueprintImplementableEvent)
	void OnBeginGoingForBathEvent();
	UPROPERTY(BlueprintAssignable)
	FAIVisitorEvent OnBeginGoingForBath;

	UFUNCTION(BlueprintImplementableEvent)
	void OnBeginGoingOutOfWaterEvent();
	UPROPERTY(BlueprintAssignable)
	FAIVisitorEvent OnBeginGoingOutOfWater;

};
