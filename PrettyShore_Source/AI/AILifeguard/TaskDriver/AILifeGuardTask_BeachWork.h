#pragma once
#include "IAILifeGuardTask.h"

class AILifeGuardTask_BeachWork : public IAILifeGuardTask {
	
public:
	AILifeGuardTask_BeachWork(AAILifeGuard_Controller* Owner, U_AILifeGuard_TaskData* TaskData,
		F_AILifeGuard_Mood* Mood, F_AILifeGuard_Need* Need, F_AILifeGuard_Info* Info)
		: IAILifeGuardTask(Owner, TaskData, Mood, Need, Info) {
		TaskModifier = TaskData->BeachTaskModifierRate;
	}

	void Enter() override;
protected:
	void OnTick(float DeltaTime) override;
public:
	void Exit() override;
	void Reset() override;

	float DrownTimer;
};

