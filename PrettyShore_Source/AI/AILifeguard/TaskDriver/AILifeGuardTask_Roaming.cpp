#include "AILifeGuardTask_Roaming.h"

void AILifeGuardTask_Roaming::Enter() {
	Owner->SetDebugText.Broadcast("Roaming");
	Owner->DoingTask = "Roaming";
	Owner->OnBeginWatch.Broadcast(Owner);
	Owner->OnBeginWatchEvent();
	Reset();
}

void AILifeGuardTask_Roaming::OnTick(float DeltaTime) {
	DrownTimer -= DeltaTime;

	if (DrownTimer <= 0.0f) {
		Owner->CheckForDrowners(1000.0f);
		DrownTimer = 2.5f;
	}

	Timer -= DeltaTime;
	bool NeedResting = (Need->Rest > 0.95f && Owner->IsTaskLocked(E_AILifeGuard_TaskType::Resting) == false);
	bool NeedRecreation = (Need->Recreation > 0.55f && (Owner->IsTaskLocked(E_AILifeGuard_TaskType::Drinking) == false
		|| Owner->IsTaskLocked(E_AILifeGuard_TaskType::Eating) == false));
	bool NeedToilet = (Need->Toilet > 0.85f && Owner->IsTaskLocked(E_AILifeGuard_TaskType::Excrementing) == false);
	if (Timer <= TaskData->BeachTaskMinTime / 2 && ( NeedResting || NeedRecreation || NeedToilet)) {
		Owner->MakeDecision();
		Reset();
	}

	if (Timer <= 0.0f) {
		Owner->MakeDecision();
		Timer = 2.0f;
	}
}

void AILifeGuardTask_Roaming::Exit() {
	Owner->OnEndWatch.Broadcast(Owner);
	Owner->OnEndWatchEvent();
}


void AILifeGuardTask_Roaming::Reset() {
	Timer = FMath::RandRange(Owner->TaskData->BaywatchMinTime, Owner->TaskData->BaywatchMaxTime);
	DrownTimer = 2.5f;
}
