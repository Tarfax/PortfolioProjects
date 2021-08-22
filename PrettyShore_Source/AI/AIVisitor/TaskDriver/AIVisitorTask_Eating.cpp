#include "AIVisitorTask_Eating.h"
#include "Team9Assemble/AI/AIVisitor/AIVisitor_Controller.h"

void AIVisitorTask_Eating::Enter() {
	Owner->SetDebugText.Broadcast("Eating");
	Owner->DoingTask = "Eating";
	Owner->OnBeginEatingEvent();
	Owner->OnBeginEating.Broadcast(Owner);
	
	
	if (Owner->StartInteractWithBuilding() == true) {
		BuildingMoodModifier = Owner->InteractingBuilding->GetBuildingMoodModifier();
	}
	else {
		Owner->FailedToExecuteTask(E_AIVisitor_TaskType::Eating, 30.0f);
	}
	Reset();
}

void AIVisitorTask_Eating::OnTick(float DeltaTime) {
	Timer -= DeltaTime;
	if (Timer <= 0.0f  || Need->Hunger <= 0.005) {
		Owner->MakeDecision();
		Timer = 2.0f;
	}
}

void AIVisitorTask_Eating::Exit() {
	Owner->Mood.ChangeStamina(-0.15f);
	Mood->TaskEnergy -= 1;
	// Spara byggnaden lokalt, så när vi ska gå hem så är inte StopInteract
	

	Owner->StopInteractWithBuilding();
	
	
	Owner->OnEndEatingEvent();
	Owner->OnEndEating.Broadcast(Owner);
}

void AIVisitorTask_Eating::Reset() {
	Timer = FMath::RandRange(TaskData->EatingMinTime, TaskData->EatingMaxTime);
}

