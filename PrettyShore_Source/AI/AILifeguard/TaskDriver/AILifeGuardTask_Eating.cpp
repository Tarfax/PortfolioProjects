#include "AILifeGuardTask_Eating.h"
#include "Team9Assemble/AI/AILifeguard/AILifeGuard_Controller.h"

void AILifeGuardTask_Eating::Enter() {
	Owner->SetDebugText.Broadcast("Eating");
	Owner->DoingTask = "Eating";
	Owner->OnBeginEating.Broadcast(Owner);
	Owner->OnBeginEatingEvent();

	if (Owner->StartInteractWithBuilding() == true) {
		BuildingMoodModifier = Owner->InteractingBuilding->GetBuildingMoodModifier();
	}
	else {
		Owner->FailedToExecuteTask(E_AILifeGuard_TaskType::Eating, 30.0f);
	}

	Reset();
}

void AILifeGuardTask_Eating::OnTick(float DeltaTime) {
	Timer -= DeltaTime;

	if (Timer < TaskData->EatingMinTime/2) {
		bool NeedToilet = (Need->Toilet > 0.85f && Owner->IsTaskLocked(E_AILifeGuard_TaskType::Excrementing) == false);
		if (NeedToilet) {
			Owner->MakeDecision();
			Reset();
		}
	}
	
	if (Timer <= 0.0f || Need->Recreation <= 0.005f) {
		Owner->MakeDecision();
		Timer = 2.0f;
	}
}

void AILifeGuardTask_Eating::Exit() {
	Owner->StopInteractWithBuilding();
	Owner->OnEndEating.Broadcast(Owner);
	Owner->OnEndEatingEvent();
}

void AILifeGuardTask_Eating::Reset() {
	Timer = FMath::RandRange(TaskData->EatingMinTime, TaskData->EatingMaxTime);
}
