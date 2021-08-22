#include "AILifeGuardTask_Drinking.h"
#include "Team9Assemble/AI/AILifeguard/AILifeGuard_Controller.h"

void AILifeGuardTask_Drinking::Enter() {
	Owner->SetDebugText.Broadcast("Drinking");
	Owner->DoingTask = "Drinking";
	Owner->OnBeginDrinking.Broadcast(Owner);
	Owner->OnBeginDrinkingEvent();

	if (Owner->StartInteractWithBuilding() == true) {
		BuildingMoodModifier = Owner->InteractingBuilding->GetBuildingMoodModifier();
	}
	else {
		Owner->FailedToExecuteTask(E_AILifeGuard_TaskType::Drinking, 30.0);
	}
	Reset();
}

void AILifeGuardTask_Drinking::OnTick(float DeltaTime) {
	Timer -= DeltaTime;

	if (Timer < TaskData->DrinkingMinTime / 2) {
		bool NeedToilet = (Need->Toilet > 0.85f && Owner->IsTaskLocked(E_AILifeGuard_TaskType::Excrementing) == false);
		if (NeedToilet) {
			Owner->MakeDecision();
			Reset();
		}
	}

	if (Timer <= 0.0f || Need->Recreation < 0.005f) {
		Owner->MakeDecision();
		Timer = 2.0f;
	}
}

void AILifeGuardTask_Drinking::Exit() {
	Owner->StopInteractWithBuilding();
	Owner->OnEndDrinking.Broadcast(Owner);
	Owner->OnEndDrinkingEvent();
}

void AILifeGuardTask_Drinking::Reset() {
	Timer = FMath::RandRange(TaskData->DrinkingMinTime, TaskData->DrinkingMaxTime);
}
