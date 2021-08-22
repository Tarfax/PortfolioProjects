#pragma once
#include "IAILifeGuardTask.h"

class AILifeGuardTask_Baywatch : public IAILifeGuardTask {
public:
	AILifeGuardTask_Baywatch(AAILifeGuard_Controller* Owner,
	                         U_AILifeGuard_TaskData* TaskData,
	                         F_AILifeGuard_Mood* Mood,
	                         F_AILifeGuard_Need* Need, F_AILifeGuard_Info* Info)
		: IAILifeGuardTask(Owner, TaskData, Mood, Need, Info) {

		TaskModifier = TaskData->BaywatchModifierRate;
	}

	void Enter() override;
	void OnTick(float DeltaTime) override;
	void Exit() override;
	void Reset() override;

	float DrownTimer;

	float VisionUpdateTimer;

	float ExtraVision = 0.0f;
	float Vision = 0.0f;

	bool HasVisitorToSave = false;

	class UVisionSensing* VisionSensing;
};
