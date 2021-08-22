#pragma once
#include "IAIVisitorTask.h"

class AIVisitorTask_AtHospital : public IAIVisitorTask {
public:
	AIVisitorTask_AtHospital(AAIVisitor_Controller* Owner, U_AIVistor_TaskData* TaskData,
                        F_AIVisitor_Mood* Mood,
                        F_AIVisitor_Need* Need)
: IAIVisitorTask(Owner, TaskData, Mood, Need) {
		
		TaskModifier = TaskData->AtHospitalModifierRate;
	}

	void Enter() override;
	void OnTick(float DeltaTime) override;
	void Exit() override;
	void Reset() override;
	
};
