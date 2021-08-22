﻿#pragma once
#include "IAILifeGuardTask.h"

class AILifeGuardTask_Excrementing : public IAILifeGuardTask {
public:
	AILifeGuardTask_Excrementing(AAILifeGuard_Controller* Owner, U_AILifeGuard_TaskData* TaskData,
		F_AILifeGuard_Mood* Mood,
		F_AILifeGuard_Need* Need, F_AILifeGuard_Info* Info)
		: IAILifeGuardTask(Owner, TaskData, Mood, Need, Info) {

		this->TaskModifier = TaskData->ExcrementingModifierRate;
	}


	void Enter( ) override;
	void OnTick(float DeltaTime) override;
	void Exit() override;
	void Reset() override;
};
