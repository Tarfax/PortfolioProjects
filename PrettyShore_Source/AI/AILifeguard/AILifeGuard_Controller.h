// I like sand

#pragma once

#include "CoreMinimal.h"
#include "Team9Assemble/AI/AIBeach_Controller_Base.h"
#include "AILifeGuard_Controller.generated.h"

class UVisionSensing;
class IAILifeGuardTask;
class AAIVisitor_Controller;
class UBeachWork;

UCLASS(BlueprintType)
class AAILifeGuard_Controller : public AAIBeach_Controller_Base {
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAILifeGuardEvent, AAILifeGuard_Controller*, AIController);

public: //Virtuals
	AAILifeGuard_Controller();
	virtual void BeginPlay() override;
	virtual void Start() override;
	virtual void TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction) override;
	virtual void OnPossess(APawn* InPawn) override;
public: //Functions
	void PresaveRuntimeCurvesIntoRichCurves() override;
	void MakeDecision() override;
	E_AILifeGuard_TaskType CheckForHighestNeedValue();
	bool WrapperContains(E_AILifeGuard_TaskType ContainsState);
	F_AILifeGuard_TaskDriverWrapper* GetWrapper(E_AILifeGuard_TaskType StateToGet);
	bool IsTaskLocked(E_AILifeGuard_TaskType StateToCheck);
	UFUNCTION(BlueprintCallable, Category="AI BUFF/DEBUFF")
	void SetModifier(E_AILifeGuard_ModifierType ModifierType);
	void HandleTaskFailure(E_AILifeGuard_TaskType TaskType);
	F_AILifeGuard_Modifier* GetModifier(E_AILifeGuard_ModifierType ModifierType);
	void SetTaskOnCooldown(E_AILifeGuard_TaskType Task, float Time);
	void FailedToExecuteTask(E_AILifeGuard_TaskType, float Time);

	UFUNCTION(BlueprintCallable)
	void SomeoneIsDrowning(bool IsDrowning);
	void GetDrownerLocation(float ExtraVision, FVector SelfLocation);
	
	UFUNCTION(BlueprintCallable)
	void StartRescuingVisitor(AAIVisitor_Controller* Visitor, bool IsCommandFromPlayer = false);

	bool CheckForDrowners(float ExtraVision);
	bool CheckForDeadPeople();
	void FinishedSavingVisitor();
	void AssignBeachWork(UBeachWork* BeachWork);
	void FinishBeachWork();
	void CancelBeachWork();

	void EvaluateMorale();
	void ReceiveSalary(int Amount);
	
	UFUNCTION(BlueprintCallable)
	void EnterWorld();
	UFUNCTION(BlueprintCallable)
	void Die();
	UFUNCTION(BlueprintCallable)
	void DestroyLifeGuard();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void RegretHire();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void DropLifeguardInWorld();

	/*** State Machine ***/
	template <typename T, typename = std::enable_if_t<std::is_base_of<IAILifeGuardTask, T>::value>>
	UFUNCTION(BlueprintCallable)
	void ChangeToState() {
		SwitchToNewTask = true;
		NextTask = new T(this, TaskData, &Mood, &Need, &Info);
	}

	void SwitchTask();
	void CancelSaveVisitor();
	void GoingToSave();
	void StopRunSlowly();
	UFUNCTION(BlueprintCallable)
	void GoingOutOfWater();
	void StartGainingMovement();
	UFUNCTION(BlueprintCallable)
	bool SetPlayerControlled();

	void SetNewTaskInBehaviourTree(E_AILifeGuard_TaskType newTask) const;

	FText GetBehaviourEnumAsString(E_AILifeGuard_TaskType EnumValue) {
		const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("E_AILifeGuard_TaskType"), true);
		if (!EnumPtr) return NSLOCTEXT("Invalid", "Invalid", "Invalid");
		return EnumPtr->GetDisplayNameTextByIndex((int32)EnumValue);
	}

public: //Variables
	UPROPERTY(BlueprintReadWrite)
	bool CanBaywatch;
	UPROPERTY(BlueprintReadWrite)
	bool CanRoam;
	UPROPERTY(BlueprintReadWrite)
	bool CanRescue;
	UPROPERTY(BlueprintReadWrite)
	bool CanDoBeachWork;
	UFUNCTION(BlueprintCallable)
	void TaskPermissionHasChanged();
	
	UPROPERTY(EditDefaultsOnly)
	U_AINameGenerator* LifeguardNames;
	
	bool IsGoingForASwim;
	bool IsGoingOutOfWater;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool IsPlayerControlled;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool CanBePickedUp;

	UPROPERTY(VisibleAnywhere)
	UVisionSensing* VisionSensing;

	/*** State Machine ***/
	IAILifeGuardTask* RunningTask;
	IAILifeGuardTask* NextTask;
	bool SwitchToNewTask;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="AI Task")
	E_AILifeGuard_TaskType PerformingTask;

	UPROPERTY(VisibleAnywhere, Category="AI Task", AdvancedDisplay, meta = (DisplayName ="Tasks On Cooldown"))
	TArray<F_AILifeGuard_TaskDriverWrapper> TaskWrappers;

	bool TasksAreUnderLockdown;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="AI Task", AdvancedDisplay)
	float TaskTimerLockdownTimer;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="AI Task")
	float TaskTimerLockdownTime = 10.0f;

	/*** AI Variables ***/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="AI Task")
	F_AILifeGuard_Mood Mood;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="AI Task")
	F_AILifeGuard_Need Need;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="AI Task")
	F_AILifeGuard_Stats Stats;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="AI Task")
	F_AILifeGuard_Info Info;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="AI Mood")
	float EvaluateMoodTime = 10.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="AI Mood")
	float EvaluateMoodTimer;

	/*** Curves ***/
	UPROPERTY()
	FCompressedRichCurve RestRuntimeCurve;
	UPROPERTY()
	FCompressedRichCurve RecreationRuntimeCurve;
	UPROPERTY()
	FCompressedRichCurve ToiletRuntimeCurve;

	// #if WITH_EDITORONLY_DATA
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="AI Decision Curves")
	float BaywatchThreshold = 0.6f;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="AI Decision Curves")
	FRuntimeFloatCurve RestCurve;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="AI Decision Curves")
	FRuntimeFloatCurve RecreationCurve;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="AI Decision Curves")
	FRuntimeFloatCurve ToiletCurve;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Morale Evaluation")
	F_AILifeGuard_MoraleCurves MoraleEvaluationCurve;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Rest Drain,  ")
	FRuntimeFloatCurve MoraleRestDrainCurve;
	// #endif

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="AI Task", AdvancedDisplay)
	TMap<E_AILifeGuard_TaskType, float> TaskEvaluationResults;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="AILifeGuard DataAssets")
	U_AILifeGuard_TaskData* TaskData;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	AAIVisitor_Controller* RescuingVisitor;
	UPROPERTY(VisibleAnywhere)
	bool IsSomeoneDrowning = false;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="AILifeGuard Mood")
	TArray<F_AILifeGuard_Modifier> Modifiers;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBeachWork* AssignedBeachWork;
	int BeachWorkDone;

	bool hasStarted = false;

	//Callbacks
	UFUNCTION(BlueprintImplementableEvent)
	void OnBeginWatchEvent();
	UPROPERTY(BlueprintAssignable)
	FAILifeGuardEvent OnBeginWatch;

	UFUNCTION(BlueprintImplementableEvent)
	void OnEndWatchEvent();
	UPROPERTY(BlueprintAssignable)
	FAILifeGuardEvent OnEndWatch;

	UFUNCTION(BlueprintImplementableEvent)
	void OnLowMoraleEvent();
	UPROPERTY(BlueprintAssignable)
	FAILifeGuardEvent OnLowMorale;

	UFUNCTION(BlueprintImplementableEvent)
	void OnNormalMoraleEvent();
	UPROPERTY(BlueprintAssignable)
	FAILifeGuardEvent OnNormalMorale;

	UFUNCTION(BlueprintImplementableEvent)
	void OnHighMoraleEvent();
	UPROPERTY(BlueprintAssignable)
	FAILifeGuardEvent OnHighMorale;
	//
	// UFUNCTION(BlueprintImplementableEvent)
	// void OnEndSunningEvent();
	// UPROPERTY(BlueprintAssignable)
	// FAILifeGuardEvent OnEndSunning;

	UFUNCTION(BlueprintImplementableEvent)
	void OnBeginDrinkingEvent();
	UPROPERTY(BlueprintAssignable)
	FAILifeGuardEvent OnBeginDrinking;

	UFUNCTION(BlueprintImplementableEvent)
	void OnEndDrinkingEvent();
	UPROPERTY(BlueprintAssignable)
	FAILifeGuardEvent OnEndDrinking;

	UFUNCTION(BlueprintImplementableEvent)
	void OnBeginSavingEvent();
	UPROPERTY(BlueprintAssignable)
	FAILifeGuardEvent OnBeginSaving;

	UFUNCTION(BlueprintImplementableEvent)
	void OnEndSavingEvent();
	UPROPERTY(BlueprintAssignable)
	FAILifeGuardEvent OnEndSaving;

	UFUNCTION(BlueprintImplementableEvent)
	void OnBeginRescueEvent();
	UPROPERTY(BlueprintAssignable)
	FAILifeGuardEvent OnBeginRescue;

	UFUNCTION(BlueprintImplementableEvent)
	void OnEndRescueEvent();
	UPROPERTY(BlueprintAssignable)
	FAILifeGuardEvent OnEndRescue;

	UFUNCTION(BlueprintImplementableEvent)
	void OnBeginEatingEvent();
	UPROPERTY(BlueprintAssignable)
	FAILifeGuardEvent OnBeginEating;

	UFUNCTION(BlueprintImplementableEvent)
	void OnEndEatingEvent();
	UPROPERTY(BlueprintAssignable)
	FAILifeGuardEvent OnEndEating;

	UFUNCTION(BlueprintImplementableEvent)
	void OnBeginExcrementingEvent();
	UPROPERTY(BlueprintAssignable)
	FAILifeGuardEvent OnBeginExcrementing;

	UFUNCTION(BlueprintImplementableEvent)
	void OnEndExcrementingEvent();
	UPROPERTY(BlueprintAssignable)
	FAILifeGuardEvent OnEndExcrementing;

	UFUNCTION(BlueprintImplementableEvent)
	void OnBeginBeachTaskEvent();
	UPROPERTY(BlueprintAssignable)
	FAILifeGuardEvent OnBeginBeachTask;

	UFUNCTION(BlueprintImplementableEvent)
	void OnEndBeachTaskEvent();
	UPROPERTY(BlueprintAssignable)
	FAILifeGuardEvent OnEndBeachTask;

	UFUNCTION(BlueprintImplementableEvent)
	void OnBeginRestingEvent();
	UPROPERTY(BlueprintAssignable)
	FAILifeGuardEvent OnBeginResting;

	UFUNCTION(BlueprintImplementableEvent)
	void OnEndRestingEvent();
	UPROPERTY(BlueprintAssignable)
	FAILifeGuardEvent OnEndResting;

	UFUNCTION(BlueprintImplementableEvent)
	void OnBeginGoingForBathEvent();
	UPROPERTY(BlueprintAssignable)
	FAILifeGuardEvent OnBeginGoingForBath;

	UFUNCTION(BlueprintImplementableEvent)
	void OnBeginGoingOutOfWaterEvent();
	UPROPERTY(BlueprintAssignable)
	FAILifeGuardEvent OnBeginGoingOutOfWater;

	
	UFUNCTION(BlueprintImplementableEvent)
	void OnDyingEvent();
	UPROPERTY(BlueprintAssignable)
	FAILifeGuardEvent OnDying;
};
