#include "AIVisitor_Controller.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TaskDriver/AIVisitorTask_MoveTo.h"
#include "Team9Assemble/AI/AIBeach_Observer.h"
#include "Team9Assemble/AI/AI_BeachCharacter.h"
#include "Team9Assemble/Buildings/DummyBuilding.h"
#include "Team9Assemble/TileSystem/Tile.h"
#include "Team9Assemble/World/BeachGameState.h"

AAIVisitor_Controller::AAIVisitor_Controller() {
	PrimaryActorTick.bCanEverTick = true;
}

void AAIVisitor_Controller::PresaveRuntimeCurvesIntoRichCurves() {
	BodyTemperatureCurve.EditorCurveData.CompressCurve(BodyTemperatureRuntimeCurve);
	ToiletCurve.EditorCurveData.CompressCurve(ToiletRuntimeCurve);
	HungerCurve.EditorCurveData.CompressCurve(HungerRuntimeCurve);
	ThirstCurve.EditorCurveData.CompressCurve(ThirstRuntimeCurve);

	DrownCurve = TaskData->ProbabilityOfDrowningCurve;
	DrownCurve.EditorCurveData.CompressCurve(DrownRuntimeCurve);
}

void AAIVisitor_Controller::BeginPlay() {
	Super::BeginPlay();
	HasStarted = false;

	PresaveRuntimeCurvesIntoRichCurves();

	EvaluateMoodTimer = EvaluateHappinessTime;

	TaskEvaluationResults.Add(E_AIVisitor_TaskType::Sunning, SunningThreshold);
	TaskEvaluationResults.Add(E_AIVisitor_TaskType::Bathing, Need.BodyTemperature);
	TaskEvaluationResults.Add(E_AIVisitor_TaskType::Drinking, Need.Thirst);
	TaskEvaluationResults.Add(E_AIVisitor_TaskType::Eating, Need.Hunger);
	TaskEvaluationResults.Add(E_AIVisitor_TaskType::Excrementing, Need.Toilet);
}

void AAIVisitor_Controller::OnPossess(APawn* InPawn) {
	Super::OnPossess(InPawn);
	Start();
}

void AAIVisitor_Controller::Start() {
	ABeachGameState* GameState = Cast<ABeachGameState>(GetWorld()->GetGameState());

	if (WaterArea == nullptr) {
		WaterArea = GameState->WaterArea;
	}

	if (BeachArea == nullptr) {
		BeachArea = GameState->BeachArea;
	}

	if (AIObserver == nullptr) {
		AIObserver = GameState->AIObserver;
	}

	if (BTAsset == nullptr) {
		BTAsset = AIObserver->AIVisitor_BehaviourTree;
	}

	if (TaskData == nullptr) {
		TaskData = AIObserver->AIVisitor_TaskData;
		UE_LOG(LogTemp, Warning, TEXT("FAILED TO INITIALIZE NeedTaskTimer_DataAsset!"));
	}

	AI_BeachCharacter = Cast<AAI_BeachCharacter>(GetPawn());
	if (AI_BeachCharacter != nullptr) {

		FString FirstName = VisitorNames->FirstNames[FMath::RandRange(0, VisitorNames->FirstNames.Num() - 1)];
		FString SecondName = VisitorNames->LastNames[FMath::RandRange(0, VisitorNames->LastNames.Num() - 1)];
		Info.Name = FirstName + " " + SecondName;
		Info.Age = FMath::RandRange(8, 86);

		AIObserver->NewVisitor(this);
		AI_BeachCharacter->IsLifeGuard = false;

		if (RunBehaviorTree(BTAsset) == false) {
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red,
			                                 TEXT("AIVisitor::Start: Could not initialize behaviour tree"));
		}
		IsAlive = true;
		Blackboard->SetValue<UBlackboardKeyType_Bool>(Blackboard->GetKeyID("IsAlive"), IsAlive);
		Blackboard->SetValueAsObject("SelfActor", this);

		Need.BodyTemperature = FMath::RandRange(0.0f, 0.6f);
		Need.Thirst = FMath::RandRange(0.0f, 0.6f);
		Need.Hunger = FMath::RandRange(0.0f, 0.6f);
		Need.Toilet = FMath::RandRange(0.0f, 0.6f);

		EvaluateHappiness();
		Mood.Stamina = 1.0f;
		Mood.TaskEnergy = FMath::RandRange(TaskEnergyMin, TaskEnergyMax);

		MakeDecision();
		HasStarted = true;
	}

}


void AAIVisitor_Controller::TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction) {
	Super::TickActor(DeltaTime, TickType, ThisTickFunction);
	if (HasStarted == true) {

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
						if (StateWrapper.Timer <= 0.0f) {
							StateWrapper.IsLocked = false;
						}
					}
				}

				if (AreAnyStateLocked == false) {
					TasksAreUnderLockdown = false;
				}
			}

			if (IsGoingForASwim == true) {
				TransitionIntoWater();
			}
			if (IsInWater == true) {
				TransitionOutOfWater();
			}

			EvaluateMoodTimer -= DeltaTime;
			if (EvaluateMoodTimer <= 0.0f) {
				EvaluateHappiness();
				EvaluateStamina();
				EvaluateMoodTimer = EvaluateHappinessTime;
			}

			TArray<F_AIVisitor_Modifier> KeepModifiers;
			bool RemovedModifier = false;
			if (Modifiers.Num() > 0) {
				TArray<F_AIVisitor_Modifier>::TIterator Iterator = Modifiers.CreateIterator();
				for (TArray<F_AIVisitor_Modifier>::TIterator; Iterator; ++Iterator) {
					Iterator->Timer -= DeltaTime;
					if (Iterator->Timer <= 0.0f) {
						RemovedModifier = true;
					}
					else {
						KeepModifiers.Add(*Iterator);
					}
				}
			}

			if (RemovedModifier == true) {
				Modifiers.Reset();
				for (int i = 0; i < KeepModifiers.Num(); i++) {
					Modifiers.Add(KeepModifiers[i]);
				}
				EvaluateHappiness();
			}

			if (IsWet == true) {
				WetTimer -= DeltaTime;
				if (WetTimer <= 0.0f) {
					IsWet = false;
					AI_BeachCharacter->IsWet = false;
				}
			}

		}
	}
}

void AAIVisitor_Controller::DestroyVisitor() {
	if (RunningTask != nullptr) {
		RunningTask->Exit();
	}
	delete RunningTask;
	AIObserver->RemoveVisitor(this);
	if (IsAlive == false) {
		AIObserver->RemoveDrowner(this);
	}
	AI_BeachCharacter->Destroy();
	if (Home != nullptr) {
		Home->LeaveHome(this);
		Home = nullptr;
	}
	StopInteractWithBuilding();
	Destroy();
}

void AAIVisitor_Controller::EvaluateHappiness() {

	float BodyTemperatureHappiness = HappinessCurves.BodyTemperatureHappinessCurve.GetRichCurve()->Eval(
		Need.BodyTemperature);
	float ThirstHappiness = HappinessCurves.ThirstHappinessCurve.GetRichCurve()->Eval(Need.Thirst);
	float HungerHappiness = HappinessCurves.HungerHappinessCurve.GetRichCurve()->Eval(Need.Hunger);
	float ToiletHappiness = HappinessCurves.ToiletHappinessCurve.GetRichCurve()->Eval(Need.Toilet);

	float allNeed = BodyTemperatureHappiness + ThirstHappiness + HungerHappiness + ToiletHappiness;

	float avarage = allNeed / 4.0f;

	for (int i = 0; i < Modifiers.Num(); i++) {
		avarage += Modifiers[i].MoodModifier.Happiness;
	}

	Mood.Happiness = avarage;

	if (Mood.Happiness < 0.15f) {
		OnVeryUnhappyEvent();
		OnVeryUnhappy.Broadcast(this);
	}
}

void AAIVisitor_Controller::EvaluateStamina() {
	if (Modifiers.Num() > 0) {
		TArray<F_AIVisitor_Modifier>::TIterator Iterator = Modifiers.CreateIterator();
		for (TArray<F_AIVisitor_Modifier>::TIterator; Iterator; ++Iterator) {
			Mood.ChangeStamina(Iterator->MoodModifier.Stamina);
		}
	}

	if (Need.Hunger > 0.95f) {
		Mood.ChangeStamina(-0.1f);
	}

	if (Need.BodyTemperature > 0.95f) {
		Mood.ChangeStamina(-0.1f);
	}
}

void AAIVisitor_Controller::SetNewTaskInBehaviourTree(E_AIVisitor_TaskType NewBlackboardState) const {
	Blackboard->SetValue<UBlackboardKeyType_Enum>(Blackboard->GetKeyID("BehaviourState"),
	                                              static_cast<UBlackboardKeyType_Enum::FDataType>(NewBlackboardState));

}


void AAIVisitor_Controller::MakeDecision() {
	if (Modifiers.Num() > 0) {
		TArray<F_AIVisitor_Modifier>::TIterator Iterator = Modifiers.CreateIterator();
		for (TArray<F_AIVisitor_Modifier>::TIterator; Iterator; ++Iterator) {
			Mood.TaskEnergy += Iterator->MoodModifier.TaskEnergy;
		}
	}

	E_AIVisitor_TaskType OldState = PerformingTask;
	if (Mood.TaskEnergy <= 0) {
		PerformingTask = E_AIVisitor_TaskType::GoingHome;
	}
	else if (HasHome == false) {
		PerformingTask = E_AIVisitor_TaskType::FindingPlace;
	}
	else {
		PerformingTask = CheckForHighestNeedValue();
	}

	if (OldState == PerformingTask) {
		if (RunningTask != nullptr) {

			RunningTask->Reset();
		}
		return;
	}

	if (IsHeadingForBuilding == true) {
		if (ReservedBuilding != nullptr) {
			ReservedBuilding->CancelIncoming(this);
		}
		ReservedBuilding = nullptr;
		IsHeadingForBuilding = false;
	}


	if (Mood.Happiness < 0.1) {
		Mood.TaskEnergy -= 3;
	}
	
	SetNewTaskInBehaviourTree(PerformingTask);
}

E_AIVisitor_TaskType AAIVisitor_Controller::CheckForHighestNeedValue() {
	float NeedValue = SunningThreshold;
	E_AIVisitor_TaskType NewTaskToPerform = E_AIVisitor_TaskType::Sunning;

	const float BodyTemperatureEval = BodyTemperatureCurve.GetRichCurve()->Eval(Need.BodyTemperature);
	TaskEvaluationResults[E_AIVisitor_TaskType::Bathing] = BodyTemperatureEval;
	if (BodyTemperatureEval > NeedValue) {
		if (IsTaskLocked(E_AIVisitor_TaskType::Bathing) == false) {
			NeedValue = BodyTemperatureEval;
			NewTaskToPerform = E_AIVisitor_TaskType::Bathing;
		}
	}

	const float ThirstEval = ThirstCurve.GetRichCurve()->Eval(Need.Thirst);
	TaskEvaluationResults[E_AIVisitor_TaskType::Drinking] = ThirstEval;
	if (ThirstEval > NeedValue) {
		if (IsTaskLocked(E_AIVisitor_TaskType::Drinking) == false) {
			NeedValue = ThirstEval;
			NewTaskToPerform = E_AIVisitor_TaskType::Drinking;
		}
	}


	const float HungerEval = HungerCurve.GetRichCurve()->Eval(Need.Hunger);
	TaskEvaluationResults[E_AIVisitor_TaskType::Eating] = HungerEval;
	if (HungerEval > NeedValue) {
		if (IsTaskLocked(E_AIVisitor_TaskType::Eating) == false) {
			NeedValue = HungerEval;
			NewTaskToPerform = E_AIVisitor_TaskType::Eating;
		}
	}

	const float ToiletEval = ToiletCurve.GetRichCurve()->Eval(Need.Toilet);
	TaskEvaluationResults[E_AIVisitor_TaskType::Excrementing] = ToiletEval;
	if (ToiletEval > NeedValue) {
		if (IsTaskLocked(E_AIVisitor_TaskType::Excrementing) == false) {
			//NeedValue = ToiletEval; todo If more tasks are coming.
			NewTaskToPerform = E_AIVisitor_TaskType::Excrementing;
		}
	}


	return NewTaskToPerform;
}

bool AAIVisitor_Controller::IsTaskLocked(E_AIVisitor_TaskType StateToCheck) {
	for (int i = 0; i < TaskWrappers.Num(); i++) {
		if (TaskWrappers[i].LockedState == StateToCheck && TaskWrappers[i].IsLocked == true) {
			return true;
		}
	}
	return false;
}

bool AAIVisitor_Controller::WrapperContains(E_AIVisitor_TaskType ContainsState) {
	for (int i = 0; i < TaskWrappers.Num(); i++) {
		if (TaskWrappers[i].LockedState == ContainsState) {
			return true;
		}
	}
	return false;
}

F_AIVisitor_TaskDriverWrapper* AAIVisitor_Controller::GetWrapper(E_AIVisitor_TaskType StateToGet) {
	for (int i = 0; i < TaskWrappers.Num(); i++) {
		if (TaskWrappers[i].LockedState == StateToGet) {
			return &TaskWrappers[i];
		}
	}
	return nullptr;
}

void AAIVisitor_Controller::SetModifier(E_AIVisitor_ModifierType ModifierType) {
	F_AIVisitor_Modifier* Modifier = GetModifier(ModifierType);
	if (Modifier != nullptr) {
		Modifier->Multiplier++;
		Modifier->Timer += 30.0f;
		return;
	}

	F_AIVisitor_Modifier Mod;

	switch (ModifierType) {
	case E_AIVisitor_ModifierType::DeathOnBeach:
		Mod.ModifierType = ModifierType;
		Mod.Timer = 300.0f;
		Mod.Description = "Someone died";
		Mod.TooltipText = "Happiness -20%";
		Mod.MoodModifier.Happiness = -0.2f;
		Mod.MoodModifier.TaskEnergy = -10;
		FailedToExecuteTask(E_AIVisitor_TaskType::Bathing, Mod.Timer);
		break;
	case E_AIVisitor_ModifierType::NoAvailableToilet:
		Mod.ModifierType = ModifierType;
		Mod.Timer = 60.0f;
		Mod.Description = "No available toilets";
		Mod.TooltipText = "Happiness -20%";
		Mod.MoodModifier.Happiness = -0.2f;
		Mod.MoodModifier.TaskEnergy = -3;
		break;
	case E_AIVisitor_ModifierType::NoAvailableFoodStall:
		Mod.ModifierType = ModifierType;
		Mod.Timer = 60.0f;
		Mod.Description = "No place to eat";
		Mod.TooltipText = "Happiness -10%";
		Mod.MoodModifier.Happiness = -0.1f;
		Mod.MoodModifier.TaskEnergy = -2;
		break;
	case E_AIVisitor_ModifierType::NoAvailableDrinkStall:
		Mod.ModifierType = ModifierType;
		Mod.Timer = 60.0f;
		Mod.Description = "No place to get a drink";
		Mod.TooltipText = "Happiness -10%";
		Mod.MoodModifier.Happiness = -0.1f;
		Mod.MoodModifier.TaskEnergy = -2;
		break;
	case E_AIVisitor_ModifierType::NoAvailableSunbench:
		Mod.ModifierType = ModifierType;
		Mod.Timer = 360.0f;
		Mod.TooltipText = "Happiness -10%";
		Mod.Description = "Couldn't find a nice sun bench";
		Mod.MoodModifier.Happiness = -0.1f;
		break;
	case E_AIVisitor_ModifierType::AlmostDrowned:
		Mod.ModifierType = ModifierType;
		Mod.Timer = 1200.0f;
		Mod.Description = "Someone saved me!";
		Mod.TooltipText = "Happiness +25%";
		Mod.IsBuff = true;
		Mod.MoodModifier.Happiness = 0.25f;
		Mod.MoodModifier.TaskEnergy = -3;
		FailedToExecuteTask(E_AIVisitor_TaskType::Bathing, Mod.Timer);
		break;
	case E_AIVisitor_ModifierType::AllNeedsMet:
		Mod.ModifierType = ModifierType;
		Mod.Timer = 120.0f;
		Mod.Description = "All needs have been met!";
		Mod.TooltipText = "Happiness +40%";
		Mod.MoodModifier.Happiness = 0.4f;
		Mod.MoodModifier.TaskEnergy = 1;
		Mod.IsBuff = true;
		break;
	default: ;
	}

	Modifiers.Add(Mod);
}

void AAIVisitor_Controller::SetDebuffAfterTaskFailed(E_AIVisitor_TaskType TaskType) {
	switch (TaskType) {
	case E_AIVisitor_TaskType::Drinking:
		SetModifier(E_AIVisitor_ModifierType::NoAvailableDrinkStall);
		break;
	case E_AIVisitor_TaskType::Eating:
		SetModifier(E_AIVisitor_ModifierType::NoAvailableFoodStall);
		break;
	case E_AIVisitor_TaskType::Excrementing:
		SetModifier(E_AIVisitor_ModifierType::NoAvailableToilet);
		break;

	case E_AIVisitor_TaskType::Nothing: break;
	case E_AIVisitor_TaskType::FindingPlace: break;
	case E_AIVisitor_TaskType::Sunning: break;
	case E_AIVisitor_TaskType::Bathing: break;
	case E_AIVisitor_TaskType::Drowning: break;
	case E_AIVisitor_TaskType::BeingSaved: break;
	case E_AIVisitor_TaskType::Hospital: break;
	case E_AIVisitor_TaskType::GoingHome: break;
	case E_AIVisitor_TaskType::Walking: break;
	case E_AIVisitor_TaskType::Dead: break;
	default: ;
	}
}

F_AIVisitor_Modifier* AAIVisitor_Controller::GetModifier(E_AIVisitor_ModifierType ModifierType) {
	for (int i = 0; i < Modifiers.Num(); i++) {
		if (Modifiers[i].ModifierType == ModifierType) {
			return &Modifiers[i];
		}
	}
	return nullptr;
}


void AAIVisitor_Controller::SetTaskOnCooldown(E_AIVisitor_TaskType TaskType, float Time) {
	TasksAreUnderLockdown = true;
	if (WrapperContains(TaskType) == false) {
		F_AIVisitor_TaskDriverWrapper Wrapper;
		Wrapper.Timer = FMath::RandRange(Time, Time + Time);
		Wrapper.IsLocked = true;
		Wrapper.LockedState = TaskType;
		TaskWrappers.Add(Wrapper);
	}
	else {
		F_AIVisitor_TaskDriverWrapper* Wrapper = GetWrapper(TaskType);
		Wrapper->IsLocked = true;
		Wrapper->Timer += Time;
	}
}

void AAIVisitor_Controller::FailedToExecuteTask(E_AIVisitor_TaskType TaskType, float Time) {

	SetTaskOnCooldown(TaskType, Time);
	SetDebuffAfterTaskFailed(TaskType);

	if (IsDrowning == true || IsBeingRescued == true) {
		return;
	}
	MakeDecision();
}

ABuildingBase* AAIVisitor_Controller::SetupBeachHomeArea(UTile* Tile, ABuildingBase* Building) {
	ADummyBuilding* Dummy = nullptr;
	HasSunbench = false;
	if (Building != nullptr && Building->BuildingData->BuildingType == E_BuildingType::VisitorHome) {
		Home = Building;
		HasSunbench = true;
		Blackboard->SetValueAsBool("HasHomePosition", true);
	}
	else {
		if (Tile->CanReservAsHome() == false) {
			return nullptr;
		}

		//TODO Fix home location being either something from the Player or its own. Maybe make a class of it?
		Dummy = GetWorld()->SpawnActor<ADummyBuilding>(DummyHomeBlueprint, Tile->GetLocation(),
		                                               FRotator::ZeroRotator);


		Home = Dummy->TryPlaceDummyBuilding(true, true);
		if (Home == nullptr) {
			return nullptr;
		}
		SetModifier(E_AIVisitor_ModifierType::NoAvailableSunbench);
	}

	if (Home != nullptr) {
		if (Dummy != nullptr) {
			Dummy->Delete();
		}
		HasHome = true;
		Blackboard->SetValueAsBool("HasHomePosition", true);

		Home->EnterHome(this);
		MakeDecision();
	}
	return Home;

}

bool AAIVisitor_Controller::StartSunning() {
	if (HasHome == false) {
		return false;
	}

	Home->Enter(this);

	return true;
}

void AAIVisitor_Controller::StopSunning() {
	if (HasHome == false) {
		MakeDecision();
		return;
	}
	Home->Leave(this);
}

void AAIVisitor_Controller::HomeWasDestroyed() {
	Blackboard->SetValueAsBool("HasHomePosition", false);
	HasHome = false;
	Home = nullptr;
}

void AAIVisitor_Controller::SwitchTask() {
	if (RunningTask != nullptr) {
		RunningTask->Exit();
		delete RunningTask;
	}

	RunningTask = NextTask;
	RunningTask->Enter();
	NextTask = nullptr;
	SwitchToNewTask = false;

}

void AAIVisitor_Controller::CheckProbabilityOfDrowning(bool InstantDrown) {
	bool ShouldDrown = false;
	if (InstantDrown == true) {
		ShouldDrown = true;
	}
	
	const float DrownEval = DrownCurve.GetRichCurve()->Eval(Mood.Stamina);
	const float RandomNumber = FMath::RandRange(0.0f, 1.0f);

	if (RandomNumber < DrownEval) {
		ShouldDrown = true;
	}

	if (ShouldDrown == true) {
		PerformingTask = E_AIVisitor_TaskType::Drowning;
		SetNewTaskInBehaviourTree(PerformingTask);
		AIObserver->AddDrowner(this);
		IsDrowning = true;
		OnBeginDrownEvent();
		OnBeginDrown.Broadcast(this);
	}
}

void AAIVisitor_Controller::SavedFromDrowning(AAILifeGuard_Controller* LifeGuard) {
	this->LifeGuardRescuer = LifeGuard;

	IsDrowning = false;

	if (IsAlive == true) {
		PerformingTask = E_AIVisitor_TaskType::BeingSaved;
		SetNewTaskInBehaviourTree(PerformingTask);
	}
	else {
		PerformingTask = E_AIVisitor_TaskType::Transported;
		SetNewTaskInBehaviourTree(PerformingTask);
	}

	OnEndDrown.Broadcast(this);
	OnEndDrownEvent();
}

void AAIVisitor_Controller::LeaveAtBuilding(ABuildingBase* Building) {
	ReservedBuilding = Building;
	Blackboard->SetValue<UBlackboardKeyType_Object>(Blackboard->GetKeyID("Building"), InteractingBuilding);
	if (IsAlive == true) {
		IsBeingRescued = false;
		SetModifier(E_AIVisitor_ModifierType::AlmostDrowned);
		PerformingTask = E_AIVisitor_TaskType::Hospital;
		SetNewTaskInBehaviourTree(PerformingTask);
		return;
	}
	AIObserver->RemoveDeadVisitor(this);
	DestroyVisitor();
}

void AAIVisitor_Controller::Die() {
	IsDrowning = false;
	OnEndDrown.Broadcast(this);
	OnEndDrownEvent();
	AIObserver->RemoveVisitor(this);
	AIObserver->AddDeadVisitor(this);

	Need.Hunger = 1;
	Need.Thirst = 1;
	Need.Toilet = 1;
	Need.BodyTemperature = 1;

	SetNewTaskInBehaviourTree(E_AIVisitor_TaskType::Dead);
	Blackboard->SetValue<UBlackboardKeyType_Bool>(Blackboard->GetKeyID("IsAlive"), IsAlive);
}

void AAIVisitor_Controller::BeginTransitionIntoWater() {
	if (IsGoingForASwim == false) {
		OnBeginGoingForBathEvent();
		OnBeginGoingForBath.Broadcast(this);
		LastMoveSpeed = AI_BeachCharacter->GetCharacterMovement()->MaxWalkSpeed;
		IsGoingForASwim = true;
	}
}

void AAIVisitor_Controller::TransitionIntoWater() {
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
	}
}

void AAIVisitor_Controller::BeginTransitionOutOfWater() {
	if (IsGoingOutOfWater == false) {
		OnBeginGoingOutOfWaterEvent();
		OnBeginGoingOutOfWater.Broadcast(this);
		LastMoveSpeed = AI_BeachCharacter->GetCharacterMovement()->MaxWalkSpeed;
		IsGoingOutOfWater = true;
	}
}

void AAIVisitor_Controller::TransitionOutOfWater() {
	float z = AI_BeachCharacter->GetActorLocation().Z - 90.0f;

	if (IsInWater == false) {
		float zClamped = FMath::Clamp(z, -84.0f, 0.0f);
		AI_BeachCharacter->GetCharacterMovement()->MaxWalkSpeed =
			FMath::Lerp(NormalMoveSpeed, LastMoveSpeed, zClamped / -84.0f);
	}

	if (IsInWater == true && z > -80) {
		IsInWater = false;
		IsWet = true;
		WetTimer = 10.0f;
		AI_BeachCharacter->IsWet = true;
		OnIsInWaterChanged(IsInWater);
	}
	if (z > 0) {
		IsGoingOutOfWater = false;
	}
}
