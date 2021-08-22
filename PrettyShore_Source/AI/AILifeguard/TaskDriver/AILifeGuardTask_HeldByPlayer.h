#pragma once
#include "IAILifeGuardTask.h"

class AAI_Trash;

class AILifeGuardTask_HeldByPlayer  : public IAILifeGuardTask {
public:
	AILifeGuardTask_HeldByPlayer(AAILifeGuard_Controller* Owner,
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
	
	void CheckForTask();
	bool SetSavingTask();
	bool SetBeachWorkTask();

	
	bool CanSave;
	bool HasWork;
	AAIVisitor_Controller* Drowner;
	AAI_Trash* Trash;
	ABuildingBase* Building;
	
	E_AILifeGuard_TaskType Task = E_AILifeGuard_TaskType::Nothing;


};
