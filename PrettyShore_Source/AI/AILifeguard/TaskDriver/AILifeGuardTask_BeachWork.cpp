#include "AILifeGuardTask_BeachWork.h"

#include "Team9Assemble/AI/Models/BeachWork.h"
#include "Team9Assemble/AI/AILifeguard/SensingComponents/VisionSensing.h"

void AILifeGuardTask_BeachWork::Enter() {
	Owner->SetDebugText.Broadcast("Doing A Beach's Work");
	Owner->DoingTask = "Beach Work";

	Owner->BeachWorkDone = 0;

	DrownTimer = 2.5f;
	Reset();

	Owner->OnBeginBeachTask.Broadcast(Owner);
	Owner->OnBeginBeachTaskEvent();
}

void AILifeGuardTask_BeachWork::OnTick(float DeltaTime) {
	Timer -= DeltaTime;
	bool NeedResting = (Need->Rest > 0.95f && Owner->IsTaskLocked(E_AILifeGuard_TaskType::Resting) == false);
	bool NeedRecreation = (Need->Recreation > 0.55 && (Owner->IsTaskLocked(E_AILifeGuard_TaskType::Drinking) == false
		|| Owner->IsTaskLocked(E_AILifeGuard_TaskType::Eating) == false));
	bool NeedToilet = (Need->Toilet > 0.85f && Owner->IsTaskLocked(E_AILifeGuard_TaskType::Excrementing) == false);

	if (Timer <= TaskData->BeachTaskMinTime / 2 && (NeedResting || NeedRecreation || NeedToilet)) {
		if (Owner->BeachWorkDone > 0) {
			Owner->MakeDecision();
		}
		Reset();
	}

	if (Timer <= 0.0f) {
		if (Owner->BeachWorkDone > 4) {
			Owner->MakeDecision();
		}
		Reset();
	}

	DrownTimer -= DeltaTime;
	if (DrownTimer <= 0.0f) {
		Owner->CheckForDrowners(Owner->VisionSensing->VisionLength);
		DrownTimer = 2.5f;
	}
}

void AILifeGuardTask_BeachWork::Exit() {
	Owner->OnEndBeachTask.Broadcast(Owner);
	Owner->OnEndBeachTaskEvent();
	Owner->CancelBeachWork();
}

void AILifeGuardTask_BeachWork::Reset() {
	Timer = FMath::RandRange(TaskData->BeachTaskMinTime, TaskData->BeachTaskMaxTime);
}
