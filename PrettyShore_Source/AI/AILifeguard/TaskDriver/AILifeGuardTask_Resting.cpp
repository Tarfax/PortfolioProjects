#include "AILifeGuardTask_Resting.h"
#include "Team9Assemble/AI/AILifeguard/AILifeGuard_Controller.h"

void AILifeGuardTask_Resting::Enter() {
	Owner->SetDebugText.Broadcast("Resting");
	Owner->DoingTask = "Resting";
	Owner->OnBeginResting.Broadcast(Owner);
	Owner->OnBeginRestingEvent();
	if (Owner->StartInteractWithBuilding() == true) {
		BuildingMoodModifier = Owner->InteractingBuilding->GetBuildingMoodModifier();
	}
	else {
		Owner->FailedToExecuteTask(E_AILifeGuard_TaskType::Resting, 30.0);
	}
	Reset();
}

void AILifeGuardTask_Resting::OnTick(float DeltaTime) {
	Timer -= DeltaTime;
	// UE_LOG(LogTemp, Log, TEXT("RESTING OnTick"));
	if (Timer < TaskData->RestingMinTime/2) {
		Reset();
		bool NeedRecreation = (Need->Recreation > 0.95f && (Owner->IsTaskLocked(E_AILifeGuard_TaskType::Drinking) == false || Owner->IsTaskLocked(E_AILifeGuard_TaskType::Eating) == false));
		bool NeedToilet = (Need->Toilet > 0.95f && Owner->IsTaskLocked(E_AILifeGuard_TaskType::Excrementing) == false);
		if (NeedRecreation || NeedToilet) {
			Owner->MakeDecision();
		}
	}
	
	if (Timer <= 0.0f || Need->Rest < 0.002f) {
		Owner->MakeDecision();
		Reset();
	}
}

void AILifeGuardTask_Resting::Exit() {
	Owner->StopInteractWithBuilding();
	Owner->OnEndResting.Broadcast(Owner);
	Owner->OnEndRestingEvent();
}

void AILifeGuardTask_Resting::Reset() {
	Timer = FMath::RandRange(TaskData->RestingMinTime, TaskData->RestingMaxTime);
}

