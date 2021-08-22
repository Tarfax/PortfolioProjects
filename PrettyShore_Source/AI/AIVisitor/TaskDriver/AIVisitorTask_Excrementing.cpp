#include "AIVisitorTask_Excrementing.h"

#include "Team9Assemble/AI/AIVisitor/AIVisitor_Controller.h"

void AIVisitorTask_Excrementing::Enter() {
	Owner->SetDebugText.Broadcast("Excrementing");
	Owner->DoingTask = "Excrementing";
	Owner->OnBeginExcrementingEvent();
	Owner->OnBeginExcrementing.Broadcast(Owner);
	
	if (Owner->StartInteractWithBuilding() == true) {
		BuildingMoodModifier = Owner->InteractingBuilding->GetBuildingMoodModifier();
	}
	else {
		Owner->FailedToExecuteTask(E_AIVisitor_TaskType::Excrementing, 30.0f);
	}
	Reset();
}

void AIVisitorTask_Excrementing::OnTick(float DeltaTime) {
	Timer -= DeltaTime;
	if (Timer <= 0.0f || Need->Toilet < 0.002f) {
		Owner->MakeDecision();
		Timer = 2.0f;
	}
}

void AIVisitorTask_Excrementing::Exit() {
	Mood->TaskEnergy -= 1;
	
	Owner->StopInteractWithBuilding();
	
	Owner->OnEndExcrementingEvent();
	Owner->OnEndExcrementing.Broadcast(Owner);
}

void AIVisitorTask_Excrementing::Reset() {
	Timer = FMath::RandRange(TaskData->ExcrementingMinTime, TaskData->ExcrementingMaxTime);
}
