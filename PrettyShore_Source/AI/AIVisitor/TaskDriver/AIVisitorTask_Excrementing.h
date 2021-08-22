#pragma once
#include "IAIVisitorTask.h"

class AIVisitorTask_Excrementing : public IAIVisitorTask {
public:
	AIVisitorTask_Excrementing(AAIVisitor_Controller* Owner, U_AIVistor_TaskData* TaskData,
	                            F_AIVisitor_Mood* Mood,
	                            F_AIVisitor_Need* Need)
		: IAIVisitorTask(Owner, TaskData, Mood, Need) {

		TaskModifier = TaskData->ExcrementingModifierRate;
	}

	void Enter() override;
	void OnTick(float DeltaTime) override;
	void Exit() override;
	void Reset() override;

};
