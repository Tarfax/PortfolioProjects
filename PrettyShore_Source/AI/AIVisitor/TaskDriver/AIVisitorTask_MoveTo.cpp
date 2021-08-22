#include "AIVisitorTask_MoveTo.h"
#include "Team9Assemble/AI/AIVisitor/AIVisitor_Controller.h"

void AIVisitorTask_MoveTo::Enter() {
	Owner->SetDebugText.Broadcast("Moving to a task");
	Owner->DoingTask = "Move To";
	if (Owner->IsInWater == true) {
		Owner->BeginTransitionOutOfWater();
	}
	Reset();
}

void AIVisitorTask_MoveTo::OnTick(float DeltaTime) {
	Timer -= DeltaTime;
	if (Timer <= 0.0f) {
		Owner->MakeDecision();
		Timer = 120.0f;
	}

	if (Owner->IsInWater == true) {
		Need->ChangeBodyTemperature( (DeltaTime * (TaskData->BodyTemperatureNormalRate + 
	(TaskData->BodyTemperatureNormalRate * Owner->TaskData->BathingModifierRate.BodyTemperature) +
	(TaskData->BodyTemperatureNormalRate * BuildingMoodModifier.AIVisitor_BodyTemperature))));
	}
}

void AIVisitorTask_MoveTo::Exit() { }

void AIVisitorTask_MoveTo::Reset() {
	Timer = 120.0f;
}
