// I like sand


#include "AILifeGuard_Controller.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TaskDriver/IAILifeGuardTask.h"
#include "Team9Assemble/AI/AIBeach_Observer.h"
#include "Team9Assemble/AI/AI_BeachCharacter.h"
#include "Team9Assemble/World/BeachGameState.h"
#include "Team9Assemble/Buildings/BuildingBase.h"
#include "Team9Assemble/AI/Models/BeachWork.h"

AAILifeGuard_Controller::AAILifeGuard_Controller() {
	PrimaryActorTick.bCanEverTick = true;
	VisionSensing = CreateDefaultSubobject<UVisionSensing>(TEXT("VisionSensing"));
}

// #if WITH_EDITOR
void AAILifeGuard_Controller::PresaveRuntimeCurvesIntoRichCurves() {
	ToiletCurve.EditorCurveData.CompressCurve(ToiletRuntimeCurve);
	RestCurve.EditorCurveData.CompressCurve(RecreationRuntimeCurve);
	RecreationCurve.EditorCurveData.CompressCurve(RestRuntimeCurve);
}

// #endif

void AAILifeGuard_Controller::BeginPlay() {
	Super::BeginPlay();

	PresaveRuntimeCurvesIntoRichCurves();

	TaskEvaluationResults.Add(E_AILifeGuard_TaskType::Baywatch, BaywatchThreshold);
	TaskEvaluationResults.Add(E_AILifeGuard_TaskType::Excrementing, Need.Toilet);
	TaskEvaluationResults.Add(E_AILifeGuard_TaskType::Resting, Need.Rest);
	TaskEvaluationResults.Add(E_AILifeGuard_TaskType::Eating, Need.Recreation);
}

void AAILifeGuard_Controller::OnPossess(APawn* InPawn) {
	SwimMoveSpeed = 220.0f;

	Super::OnPossess(InPawn);
	Start();
}

void AAILifeGuard_Controller::Start() {
	ABeachGameState* GameState = Cast<ABeachGameState>(GetWorld()->GetGameState());
	if (BeachArea == nullptr) {
		BeachArea = GameState->BeachArea;
	}

	if (AIObserver == nullptr) {
		AIObserver = GameState->AIObserver;
	}

	if (BTAsset == nullptr) {
		BTAsset = AIObserver->AILifeGuard_BehaviourTree;
	}

	if (TaskData == nullptr) {
		TaskData = AIObserver->AILifeGuard_TaskData;
		// UE_LOG(LogTemp, Warning, TEXT("FAILED TO INITIALIZE NeedTaskTimer_DataAsset!"));

	}

	AI_BeachCharacter = Cast<AAI_BeachCharacter>(GetPawn());
	if (AI_BeachCharacter != nullptr) {

		FString FirstName = LifeguardNames->FirstNames[FMath::RandRange(0, LifeguardNames->FirstNames.Num() - 1)];
		FString SecondName = LifeguardNames->LastNames[FMath::RandRange(0, LifeguardNames->LastNames.Num() - 1)];
		Info.Name = FirstName + " " + SecondName;
		Info.Age = FMath::RandRange(18, 65);

		// UE_LOG(LogTemp, Log, TEXT("Added new lifeguard, AI_BeachCharacter != nullptr"));


		AI_BeachCharacter->IsLifeGuard = true;
		VisionSensing->AILifeGuard = this;
		VisionSensing->DebugVision = false;
		AI_BeachCharacter->Vision = VisionSensing;

		Need.MaxRestValue = Stats.EnergyMaxLevel;
		Need.Salary = 0.5f;

		if (RunBehaviorTree(BTAsset) == false) {
			// 	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Could not initialize behaviour tree"));
		}
		BlackBoardAccessor = Blackboard;

		Blackboard->SetValueAsObject("SelfActor", this);

		CanBaywatch = true;
		CanRoam = true;
		CanRescue = true;
		CanDoBeachWork = true;
		
		PerformingTask = E_AILifeGuard_TaskType::HasSpawned;
		SetNewTaskInBehaviourTree(PerformingTask);
		hasStarted = true;
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("DID NOT new lifeguard, AI_BeachCharacter != nullptr"));

	}


}


void AAILifeGuard_Controller::TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction) {
	Super::TickActor(DeltaTime, TickType, ThisTickFunction);
	if (hasStarted == true) {

		if (TickType == ELevelTick::LEVELTICK_All) {
			if (SwitchToNewTask == true) {
				SwitchTask();
			}

			if (RunningTask != nullptr) {
				RunningTask->Tick(DeltaTime);
			}

			if (TasksAreUnderLockdown == true) {
				bool AreAnyStateLocked = false;
				for (auto& StateWrapper : TaskWrappers) {
					if (StateWrapper.IsLocked == true) {
						StateWrapper.Timer -= DeltaTime;
						AreAnyStateLocked = true;
						// UE_LOG(LogTemp, Log, TEXT("AAIVisitor_Controller::TickActor: StateWrapper deltaTime: %f"), StateWrapper.Timer);
						if (StateWrapper.Timer <= 0.0f) {
							StateWrapper.IsLocked = false;
							StateWrapper.FailedCount = 0;
						}
					}
				}

				if (AreAnyStateLocked == false) {
					TasksAreUnderLockdown = false;
				}
			}

			if (IsGoingForASwim == true) {
				StopRunSlowly();
			}
			if (IsGoingOutOfWater == true) {
				StartGainingMovement();
			}

			EvaluateMoodTimer -= DeltaTime;
			if (EvaluateMoodTimer <= 0.0f) {
				EvaluateMorale();
				EvaluateMoodTimer = EvaluateMoodTime;
			}

		}
	}
}


void AAILifeGuard_Controller::EvaluateMorale() {
	//Check how much dirt there is in the level.

	float SalaryMorale = MoraleEvaluationCurve.SalaryMoraleCurve.GetRichCurve()->Eval(Need.Salary);
	float RecreationMorale = MoraleEvaluationCurve.RecreationMoraleCurve.GetRichCurve()->Eval(Need.Recreation);
	float RestMorale = MoraleEvaluationCurve.RestMoraleCurve.GetRichCurve()->Eval(Need.Rest);
	float ToiletMorale = MoraleEvaluationCurve.ToiletMoraleCurve.GetRichCurve()->Eval(Need.Toilet);

	float allNeed = SalaryMorale + RecreationMorale + RestMorale + ToiletMorale;

	float avarage = allNeed / 4.0f;
	Mood.Morale = avarage;
	if (Mood.Morale > 0.75f) {
		OnHighMorale.Broadcast(this);
		OnHighMoraleEvent();
	}
	else if (Mood.Morale > 0.35f) {
		OnNormalMorale.Broadcast(this);
		OnNormalMoraleEvent();
	}
	else {
		OnLowMorale.Broadcast(this);
		OnLowMoraleEvent();
	}
}

void AAILifeGuard_Controller::ReceiveSalary(int Amount) {
	if (Amount >= Info.Salary) {
		Need.ChangeSalary(0.1f);
	}
	else {
		Need.ChangeSalary(-0.1f);
	}

	if (Need.Salary < 0.1) {
		SetModifier(E_AILifeGuard_ModifierType::ImNotPaid);
	}
}

void AAILifeGuard_Controller::EnterWorld() {
	AIObserver->NewLifeGuard(this);
}

void AAILifeGuard_Controller::Die() {
	OnDying.Broadcast(this);
	OnDyingEvent();
}

void AAILifeGuard_Controller::DestroyLifeGuard() {
	if (RunningTask != nullptr) {
		RunningTask->Exit();
	}
	delete RunningTask;
	AIObserver->RemoveLifeGuard(this);
	AI_BeachCharacter->Destroy();
	CancelBeachWork();
	StopInteractWithBuilding();
	CancelSaveVisitor();
	Destroy();
}

void AAILifeGuard_Controller::MakeDecision() {
	const E_AILifeGuard_TaskType OldState = PerformingTask;
	UE_LOG(LogTemp, Log, TEXT("LIFEGUARD IS MAKING DECISION"));

	// if (TaskData->StopTask_RestCurve.GetRichCurve()->Eval(Need.Rest) > 0.9f) {
	// 	PerformingTask = E_AILifeGuard_TaskType::Resting;
	// }
	// else {
	PerformingTask = CheckForHighestNeedValue();
	// }

	if (OldState == PerformingTask) {
		if (RunningTask != nullptr) {
			RunningTask->Reset();
		}
		// UE_LOG(LogTemp, Log, TEXT("REPEAT OF TASK LIFEGUARD"));
		return;
	}

	if (IsHeadingForBuilding == true) {
		ReservedBuilding->CancelIncoming(this);
		ReservedBuilding = nullptr;
		IsHeadingForBuilding = false;
	}
	// UE_LOG(LogTemp, Log, TEXT("LIFEGUARD NEW DECISION HAS BEEN MADE!"));

	SetNewTaskInBehaviourTree(PerformingTask);
}

E_AILifeGuard_TaskType AAILifeGuard_Controller::CheckForHighestNeedValue() {
	float NeedValue = BaywatchThreshold;
	E_AILifeGuard_TaskType NewTaskToPerform = E_AILifeGuard_TaskType::Nothing;

	if (IsTaskLocked(E_AILifeGuard_TaskType::Baywatch) == false) {
		if (CanBaywatch == true) {
			NewTaskToPerform = E_AILifeGuard_TaskType::Baywatch;
		}
	}
	else if (IsTaskLocked(E_AILifeGuard_TaskType::BeachWork) == false) {
		if (CanDoBeachWork == true) {
			NewTaskToPerform = E_AILifeGuard_TaskType::BeachWork;
		}
	}
	else {
		// UE_LOG(LogTemp, Log, TEXT("LIFEGUARD BAYWATCH TASK IS LOCKED"));

	}

	const float RecreationEval = RecreationCurve.GetRichCurve()->Eval(Need.Recreation);
	TaskEvaluationResults[E_AILifeGuard_TaskType::Eating] = RecreationEval;
	if (RecreationEval > NeedValue) {
		NeedValue = RecreationEval;
		float Random = FMath::RandRange(0.0f, 1.0f);
		if (Random > 0.5f && IsTaskLocked(E_AILifeGuard_TaskType::Drinking) == false) {
			NewTaskToPerform = E_AILifeGuard_TaskType::Drinking;
		}
		else if (IsTaskLocked(E_AILifeGuard_TaskType::Eating) == false) {
			NewTaskToPerform = E_AILifeGuard_TaskType::Eating;
		}
	}

	if (IsTaskLocked(E_AILifeGuard_TaskType::Resting) == false) {
		const float RestEval = RestCurve.GetRichCurve()->Eval(Need.Rest);
		TaskEvaluationResults[E_AILifeGuard_TaskType::Resting] = RestEval;
		if (RestEval > NeedValue) {

			NeedValue = RestEval;
			NewTaskToPerform = E_AILifeGuard_TaskType::Resting;
		}
	}

	if (IsTaskLocked(E_AILifeGuard_TaskType::Excrementing) == false) {
		const float ToiletEval = ToiletCurve.GetRichCurve()->Eval(Need.Toilet);
		TaskEvaluationResults[E_AILifeGuard_TaskType::Excrementing] = ToiletEval;
		if (ToiletEval > NeedValue) {
			NeedValue = ToiletEval;
			NewTaskToPerform = E_AILifeGuard_TaskType::Excrementing;
		}
	}

	if (NewTaskToPerform == E_AILifeGuard_TaskType::Nothing) {
		if (CanRoam == true) {
			NewTaskToPerform = E_AILifeGuard_TaskType::Roaming;
		}
	}
	// UE_LOG(LogTemp, Log, TEXT("TASK DECISION: %s!"), *GetBehaviourEnumAsString(NewTaskToPerform).ToString());

	return NewTaskToPerform;
}

bool AAILifeGuard_Controller::WrapperContains(E_AILifeGuard_TaskType ContainsState) {
	for (int i = 0; i < TaskWrappers.Num(); i++) {
		if (TaskWrappers[i].LockedState == ContainsState) {
			return true;
		}
	}
	return false;
}

F_AILifeGuard_TaskDriverWrapper* AAILifeGuard_Controller::GetWrapper(E_AILifeGuard_TaskType StateToGet) {
	for (int i = 0; i < TaskWrappers.Num(); i++) {
		if (TaskWrappers[i].LockedState == StateToGet) {
			return &TaskWrappers[i];
		}
	}
	return nullptr;
}

bool AAILifeGuard_Controller::IsTaskLocked(E_AILifeGuard_TaskType StateToCheck) {
	for (int i = 0; i < TaskWrappers.Num(); i++) {
		if (TaskWrappers[i].LockedState == StateToCheck && TaskWrappers[i].IsLocked == true) {
			return true;
		}
	}
	return false;
}

void AAILifeGuard_Controller::SetModifier(E_AILifeGuard_ModifierType ModifierType) {
	F_AILifeGuard_Modifier* Modifier = GetModifier(ModifierType);
	if (Modifier != nullptr) {
		Modifier->Multiplier++;
		Modifier->Timer += 30.0f;
		return;
	}

	F_AILifeGuard_Modifier Mod;

	switch (ModifierType) {
	case E_AILifeGuard_ModifierType::DeathOnBeach:
		Mod.ModifierType = ModifierType;
		Mod.Timer = 300.0f;
		Mod.TooltipText = "Morale -20%";
		Mod.Description = "Someone died";
		Mod.MoodModifier.Morale = -0.2f;
		break;
	case E_AILifeGuard_ModifierType::NoAvailableToilet:
		Mod.ModifierType = ModifierType;
		Mod.Timer = 120.0f;
		Mod.TooltipText = "Morale -20%";

		Mod.Description = "No available toilets";
		Mod.MoodModifier.Morale = -0.2f;
		break;
	case E_AILifeGuard_ModifierType::NoAvailableFoodStall:
		Mod.ModifierType = ModifierType;
		Mod.Timer = 120.0f;
		Mod.Description = "No place to eat";		
		Mod.TooltipText = "Morale -10%";
		Mod.MoodModifier.Morale = -0.1f;
		break;
	case E_AILifeGuard_ModifierType::NoAvailableDrinkStall:
		Mod.ModifierType = ModifierType;
		Mod.Timer = 120.0f;
		Mod.Description = "No place to get a drink";
		Mod.TooltipText = "Morale -10%";

		Mod.MoodModifier.Morale = -0.1f;
		break;
	case E_AILifeGuard_ModifierType::SavedSomeone:
		Mod.ModifierType = ModifierType;
		Mod.Timer = 600.0f;
		Mod.Description = "I saved someone!";
		Mod.IsBuff = true;
		Mod.TooltipText = "Morale +25%";

		Mod.MoodModifier.Morale = 0.25f;
		break;
	case E_AILifeGuard_ModifierType::AllNeedsMet:
		Mod.ModifierType = ModifierType;
		Mod.Timer = 60.0f;
		Mod.IsBuff = true;
		Mod.Description = "All needs have been met!";
		Mod.TooltipText = "Morale +40%";
		Mod.MoodModifier.Morale = 0.4f;
		break;
	case E_AILifeGuard_ModifierType::NoAvailableHospitals:
		Mod.ModifierType = ModifierType;
		Mod.Timer = 60.0f;
		Mod.Description = "No Hospitals to take the drowners";
		Mod.TooltipText = "Morale -10%";

		Mod.MoodModifier.Morale = -0.1f;
		break;
	case E_AILifeGuard_ModifierType::ImNotPaid:
		Mod.ModifierType = ModifierType;
		Mod.Timer = 360.0f;
		Mod.Description = "Where is my salary?";
		Mod.TooltipText = "Morale -10%";

		Mod.MoodModifier.Morale = -1.0f;
		break;
	case E_AILifeGuard_ModifierType::NoAvailableStaffRoom:
		Mod.ModifierType = ModifierType;
		Mod.Timer = 260.0f;
		Mod.Description = "I need to rest";
		Mod.TooltipText = "Morale -30%";
		Mod.MoodModifier.Morale = -0.3f;
		break;
	default: ;
	}

	// GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red,
	//                                  TEXT("AILifeGuard::SetModifier: Added a modifier"));
	Modifiers.Add(Mod);
}

void AAILifeGuard_Controller::HandleTaskFailure(E_AILifeGuard_TaskType TaskType) {
	switch (TaskType) {
	case E_AILifeGuard_TaskType::Drinking:
		SetModifier(E_AILifeGuard_ModifierType::NoAvailableDrinkStall);
		break;
	case E_AILifeGuard_TaskType::Eating:
		SetModifier(E_AILifeGuard_ModifierType::NoAvailableFoodStall);
		break;
	case E_AILifeGuard_TaskType::Excrementing:
		SetModifier(E_AILifeGuard_ModifierType::NoAvailableToilet);
		break;
	case E_AILifeGuard_TaskType::Resting:
		SetModifier(E_AILifeGuard_ModifierType::NoAvailableStaffRoom);
		break;

	case E_AILifeGuard_TaskType::Nothing: break;
	case E_AILifeGuard_TaskType::Baywatch:
		break;
	case E_AILifeGuard_TaskType::Roaming: break;
	case E_AILifeGuard_TaskType::Saving:
		SetModifier(E_AILifeGuard_ModifierType::NoAvailableHospitals);
		CancelSaveVisitor();
		break;
	case E_AILifeGuard_TaskType::Training: break;
	case E_AILifeGuard_TaskType::BeachWork: break;
	case E_AILifeGuard_TaskType::MoveTo: break;
	default: ;
	}
}

F_AILifeGuard_Modifier* AAILifeGuard_Controller::GetModifier(E_AILifeGuard_ModifierType ModifierType) {
	for (int i = 0; i < Modifiers.Num(); i++) {
		if (Modifiers[i].ModifierType == ModifierType) {
			return &Modifiers[i];
		}
	}
	return nullptr;
}

void AAILifeGuard_Controller::SetTaskOnCooldown(E_AILifeGuard_TaskType Task, float Time) {
	TasksAreUnderLockdown = true;
	if (WrapperContains(Task) == false) {
		F_AILifeGuard_TaskDriverWrapper Wrapper;
		Wrapper.Timer = FMath::RandRange(Time, Time + Time);
		Wrapper.IsLocked = true;
		Wrapper.LockedState = Task;
		TaskWrappers.Add(Wrapper);
	}
	else {
		F_AILifeGuard_TaskDriverWrapper* Wrapper = GetWrapper(Task);
		Wrapper->IsLocked = true;
		Wrapper->Timer += Time;
		Wrapper->FailedCount++;
		if (Wrapper->LockedState == E_AILifeGuard_TaskType::Resting && Wrapper->FailedCount > 10) {
			// UE_LOG(LogTemp, Warning, TEXT("SOMEONE IS CRANKY!!!!"));
		}
	}
}

void AAILifeGuard_Controller::FailedToExecuteTask(E_AILifeGuard_TaskType Task, float Time) {
	SetTaskOnCooldown(Task, Time);
	HandleTaskFailure(Task);

	// UE_LOG(LogTemp, Warning, TEXT("AAIVisitor_LifeGuard::FailedToExecuteTask: Failed to execute task: %s"),
	//        *GetBehaviourEnumAsString(Task).ToString());

	MakeDecision();
}

void AAILifeGuard_Controller::SomeoneIsDrowning(bool IsDrowning) {
	IsSomeoneDrowning = IsDrowning;
	Blackboard->SetValue<UBlackboardKeyType_Bool>(Blackboard->GetKeyID("IsSomeoneDrowning"), IsDrowning);
	if (InteractingBuilding != nullptr) {
		if (InteractingBuilding->BuildingData->BuildingType == E_BuildingType::WatchTower) {
			CheckForDrowners(InteractingBuilding->BuildingData->WatchTowerVisionRange);
		}
	}
}


bool AAILifeGuard_Controller::CheckForDrowners(float ExtraVision) {
	if (CanRescue == true) {
		if (IsSomeoneDrowning == true) {
			FVector SelfLocation = AI_BeachCharacter->GetActorLocation();
			GetDrownerLocation(ExtraVision, SelfLocation);
			if (RescuingVisitor != nullptr) {
				return true;
			}
		}
		return CheckForDeadPeople();
	}
	return false;
}


void AAILifeGuard_Controller::GetDrownerLocation(float ExtraVision, FVector SelfLocation) {
	for (AAIVisitor_Controller* Visitor : AIObserver->GetDrowners()) {
		if (Visitor->IsBeingRescued == false && Visitor->IsAlive == true) {
			FVector PositionToTest = Visitor->AI_BeachCharacter->GetActorLocation();
			FVector Direction = FVector::ForwardVector;
			float VisionRange = Stats.Vision + ExtraVision;

			if (VisionSensing->IsAIVisitorVisible(PositionToTest, SelfLocation, Direction, VisionRange) == true) {
				StartRescuingVisitor(Visitor);
				return;
			}
		}
	}
}

void AAILifeGuard_Controller::StartRescuingVisitor(AAIVisitor_Controller* Visitor, bool IsCommandFromPlayer) {
	if (CanRescue == false) {
		return;
	}
	if (IsCommandFromPlayer == true && RescuingVisitor != nullptr) {
		return;
	}

	if (Visitor->IsBeingRescued == false) {
		Visitor->IsBeingRescued = true;
		AIObserver->StartSaveDrowner(Visitor, this);

		StopMovement();

		Blackboard->SetValue<UBlackboardKeyType_Object>(Blackboard->GetKeyID("DrowningVisitor"), Visitor);
		RescuingVisitor = Visitor;
		PerformingTask = E_AILifeGuard_TaskType::Saving;
		SetNewTaskInBehaviourTree(PerformingTask);
		CancelBeachWork();
		StopInteractWithBuilding();
		CanBePickedUp = false;
	}
}

bool AAILifeGuard_Controller::CheckForDeadPeople() {
	if (IsTaskLocked(E_AILifeGuard_TaskType::Saving) == false && AIObserver->DeadVisitors.Num() > 0) {
		for (AAIVisitor_Controller* DeadVisitor : AIObserver->DeadVisitors) {
			if (DeadVisitor->IsBeingRescued == false) {
				StartRescuingVisitor(DeadVisitor);
				return true;
			}
		}
	}
	return false;
}

void AAILifeGuard_Controller::FinishedSavingVisitor() {
	OnEndRescue.Broadcast(this);
	OnEndRescueEvent();

	AIObserver->FinishSaveDrowner(RescuingVisitor);
	if (RescuingVisitor == nullptr) {
		// UE_LOG(LogTemp, Log, TEXT("Why is rescuing visitor nullptr?"));
		RescuingVisitor = nullptr;
		MakeDecision();
		return;
	}
	else {
		// UE_LOG(LogTemp, Log, TEXT("Rescuing visitor is not nullptr!!!"));
	}

	if (RescuingVisitor->IsAlive == true) {
		SetModifier(E_AILifeGuard_ModifierType::SavedSomeone);
	}
	// InteractingBuilding->Enter(this);
	// InteractingBuilding->Leave(this);
	RescuingVisitor = nullptr;
	// UE_LOG(LogTemp, Log, TEXT("LIFEGUARD HAS FINISHED SAVING SOMEONE AND IS MAKING DECISION"));
	CanBePickedUp = true;
	MakeDecision();
}

void AAILifeGuard_Controller::AssignBeachWork(UBeachWork* beachWork) {
	AssignedBeachWork = beachWork;
	beachWork->WorkOwner = this;
}

void AAILifeGuard_Controller::FinishBeachWork() {
	if (AssignedBeachWork != nullptr) {
		AssignedBeachWork->BeachWorkObject->FinishWork(AssignedBeachWork);
		AssignedBeachWork->WorkOwner = nullptr;
	}
	BeachWorkDone++;
	AssignedBeachWork = nullptr;
}

void AAILifeGuard_Controller::CancelBeachWork() {
	if (AssignedBeachWork != nullptr) {
		AssignedBeachWork->Cancel();
		Blackboard->SetValue<UBlackboardKeyType_Object>(Blackboard->GetKeyID("BeachWork"), nullptr);
	}
	AssignedBeachWork = nullptr;
}

void AAILifeGuard_Controller::SwitchTask() {
	if (RunningTask != nullptr) {
		RunningTask->Exit();
		delete RunningTask;
	}

	RunningTask = NextTask;
	RunningTask->Enter();
	NextTask = nullptr;
	SwitchToNewTask = false;
}

void AAILifeGuard_Controller::CancelSaveVisitor() {
	if (RescuingVisitor != nullptr) {
		AIObserver->CancelSaveDrowner(RescuingVisitor);
		RescuingVisitor->IsBeingRescued = false;
		RescuingVisitor = nullptr;
	}
}

void AAILifeGuard_Controller::GoingToSave() {

	AI_BeachCharacter->GetCharacterMovement()->MaxWalkSpeed = RunMoveSpeed;
	LastMoveSpeed = AI_BeachCharacter->GetCharacterMovement()->MaxWalkSpeed;
	IsGoingForASwim = true;
}

void AAILifeGuard_Controller::StopRunSlowly() {

	const float z = AI_BeachCharacter->GetActorLocation().Z - 90.0f;

	if (z < -20.0f) {
		const float zClamped = FMath::Clamp(z, -70.0f, 0.0f);
		AI_BeachCharacter->GetCharacterMovement()->MaxWalkSpeed =
			FMath::Lerp(LastMoveSpeed, SwimMoveSpeed + 10, zClamped / -70.0f);
	}

	if (z < -80.0f) {
		IsInWater = true;
		IsGoingForASwim = false;
		OnIsInWaterChanged(IsInWater);
		OnBeginRescue.Broadcast(this);
		OnBeginRescueEvent();
	}
}

void AAILifeGuard_Controller::GoingOutOfWater() {
	OnBeginGoingOutOfWaterEvent();
	OnBeginGoingOutOfWater.Broadcast(this);
	LastMoveSpeed = AI_BeachCharacter->GetCharacterMovement()->MaxWalkSpeed;
	IsGoingOutOfWater = true;
}

void AAILifeGuard_Controller::StartGainingMovement() {
	float z = AI_BeachCharacter->GetActorLocation().Z - 90.0f;

	// UE_LOG(LogTemp, Log, TEXT("StartGainingMOvement LIFEGUARD"));

	if (IsInWater == false) {
		float zClamped = FMath::Clamp(z, -84.0f, 0.0f);
		AI_BeachCharacter->GetCharacterMovement()->MaxWalkSpeed =
			FMath::Lerp(NormalMoveSpeed, LastMoveSpeed, zClamped / -84.0f);
	}

	if (IsInWater == true && z > -80) {
		IsInWater = false;
		OnIsInWaterChanged(IsInWater);
	}
	if (z > 0) {
		IsGoingOutOfWater = false;
	}
}

bool AAILifeGuard_Controller::SetPlayerControlled() {
	if (CanBePickedUp == true) {
		StopMovement();
		AI_BeachCharacter->MovementComponent->StopMovementImmediately();
		PerformingTask = E_AILifeGuard_TaskType::HeldByPlayer;
		SetNewTaskInBehaviourTree(PerformingTask);
		CancelBeachWork();
		StopInteractWithBuilding();
		IsPlayerControlled = true;
		return true;
	}
	return false;
}


void AAILifeGuard_Controller::SetNewTaskInBehaviourTree(E_AILifeGuard_TaskType newTask) const {
	Blackboard->SetValue<UBlackboardKeyType_Enum>(Blackboard->GetKeyID("BehaviourState"),
	                                              static_cast<UBlackboardKeyType_Enum::FDataType>(newTask));
}

void AAILifeGuard_Controller::TaskPermissionHasChanged() {
	if (CanBaywatch == false) {
		if (PerformingTask == E_AILifeGuard_TaskType::Baywatch) {
			MakeDecision();
		}
	}

	if (CanRoam == false) {
		if (PerformingTask == E_AILifeGuard_TaskType::Roaming) {
			MakeDecision();
		}
	}

	if (CanDoBeachWork == false) {
		if (PerformingTask == E_AILifeGuard_TaskType::BeachWork) {
			MakeDecision();
		}
	}
}
