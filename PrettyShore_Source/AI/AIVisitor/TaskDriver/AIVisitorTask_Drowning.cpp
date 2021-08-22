#include "AIVisitorTask_Drowning.h"

#include "Team9Assemble/AI/AIVisitor/AIVisitor_Controller.h"

void AIVisitorTask_Drowning::Enter() {
	Owner->SetDebugText.Broadcast("I'm drowning! Senpai, notice me!");
	Owner->DoingTask = "Drowning";
	Owner->StopMovement();
	Reset();
}

void AIVisitorTask_Drowning::OnTick(float DeltaTime) {
	Timer-= DeltaTime;
	if (Timer <= 0.0f) {
		Owner->Die();
		Timer = 2.0f;
	}
}

void AIVisitorTask_Drowning::Exit() {

}

void AIVisitorTask_Drowning::Reset() {
	Timer = FMath::RandRange(TaskData->DrowningMinTime, TaskData->DrowningMaxTime);
}
