#include "AIVisitorTask_Bathing.h"
#include "Team9Assemble/AI/AIVisitor/AIVisitor_Controller.h"

void AIVisitorTask_Bathing::Enter() {
	Owner->SetDebugText.Broadcast("Bath time!");
	Owner->DoingTask = "Bathing";
	Owner->OnBeginBathingEvent();
	Owner->OnBeginBathing.Broadcast(Owner);


	BeginCheckIfShouldDrownTimer = TaskData->StartCheckToDrownTimer;
	CheckToDrown = false;
	Reset();
}

void AIVisitorTask_Bathing::OnTick(float DeltaTime) {
	Timer -= DeltaTime;
	BeginCheckIfShouldDrownTimer -= DeltaTime;
	DrainStaminaTimer -= DeltaTime;

	if (Timer <= 0.0f || Need->BodyTemperature <= 0.005f) {
		Owner->MakeDecision();
		Timer = 2.0f;
	}

	if (DrainStaminaTimer <= 0.0f) {
		Mood->Stamina -= 0.01;
		DrainStaminaTimer = 10.0f;
	}

	if (CheckToDrown == false && BeginCheckIfShouldDrownTimer <= 0.0f) {
		CheckToDrown = true;
		CheckIfShouldDrown = FMath::RandRange(TaskData->CheckToDrownMin, TaskData->CheckToDrownMax);
	}

	if (CheckToDrown == true) {
		CheckIfShouldDrown -= DeltaTime;
		if (CheckIfShouldDrown <= 0.0f) {
			Owner->CheckProbabilityOfDrowning();
			CheckIfShouldDrown = FMath::RandRange(TaskData->CheckToDrownMin, TaskData->CheckToDrownMax);
		}
	}

}

void AIVisitorTask_Bathing::Exit() {
	Owner->OnEndBathingEvent();
	Owner->OnEndBathing.Broadcast(Owner);
	Mood->TaskEnergy -= 1;
	// Owner->GoingOutOfWater();
}

void AIVisitorTask_Bathing::Reset() {
	Timer = FMath::RandRange(TaskData->BathingMinTime,
	                         TaskData->BathingMaxTime);
	DrainStaminaTimer = 10.0f;
}
