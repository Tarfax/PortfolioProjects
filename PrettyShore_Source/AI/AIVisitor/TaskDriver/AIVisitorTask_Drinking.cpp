#include "AIVisitorTask_Drinking.h"

#include "Team9Assemble/AI/AIVisitor/AIVisitor_Controller.h"

void AIVisitorTask_Drinking::Enter() {
	Owner->SetDebugText.Broadcast("Drinking");
	Owner->DoingTask = "Drinking";
	Owner->OnBeginDrinkingEvent();
	Owner->OnBeginDrinking.Broadcast(Owner);
	

	if (Owner->StartInteractWithBuilding() == true) {
		BuildingMoodModifier = Owner->InteractingBuilding->GetBuildingMoodModifier();
	}
	else {
		Owner->FailedToExecuteTask(E_AIVisitor_TaskType::Drinking, 30.0f);
	}
	Reset();
}

void AIVisitorTask_Drinking::OnTick(float DeltaTime) {
	Timer -= DeltaTime;
	if (Timer <= 0.0f) {
		Owner->MakeDecision();
		Timer = 2.0f;
	}

	if (Need->Thirst <= 0.005f) {
		Owner->MakeDecision();
	}
}

void AIVisitorTask_Drinking::Exit() {
	Owner->Mood.ChangeStamina(-0.1);
	Mood->TaskEnergy -= 1;
	
	Owner->StopInteractWithBuilding();
	
	Owner->OnEndDrinkingEvent();
	Owner->OnEndDrinking.Broadcast(Owner);
}

void AIVisitorTask_Drinking::Reset() {
	Timer = FMath::RandRange(TaskData->DrinkingMinTime,
						TaskData->DrinkingMaxTime);
}
