﻿#pragma once
#include "IAILifeGuardTask.h"

class AILifeGuardTask_DoNothing : public IAILifeGuardTask{
public:
	AILifeGuardTask_DoNothing(AAILifeGuard_Controller* Owner, U_AILifeGuard_TaskData* TaskData,
		F_AILifeGuard_Mood* Mood,
		F_AILifeGuard_Need* Need, F_AILifeGuard_Info* Info)
		: IAILifeGuardTask(Owner, TaskData, Mood, Need, Info) {

		TaskModifier = TaskData->WalkingModifierRate;
	}


	void Enter( ) override;
	void OnTick(float DeltaTime) override;
	void Exit() override;
	void Reset();
};
