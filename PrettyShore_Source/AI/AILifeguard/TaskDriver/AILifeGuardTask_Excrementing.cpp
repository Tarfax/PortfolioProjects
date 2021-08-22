#include "AILifeGuardTask_Excrementing.h"
#include "Team9Assemble/AI/AILifeguard/AILifeGuard_Controller.h"

void AILifeGuardTask_Excrementing::Enter() {
	Owner->SetDebugText.Broadcast("Excrementing");
	Owner->DoingTask = "Excrementing";
	Owner->OnBeginExcrementing.Broadcast(Owner);
	Owner->OnBeginExcrementingEvent();
	
	if (Owner->StartInteractWithBuilding() == true) {
		BuildingMoodModifier = Owner->InteractingBuilding->GetBuildingMoodModifier();
	}
	else {
		Owner->FailedToExecuteTask(E_AILifeGuard_TaskType::Excrementing, 30.0f);
	}
	Reset();
}

void AILifeGuardTask_Excrementing::OnTick(float DeltaTime) {
	// UE_LOG(LogTemp, Log, TEXT("EXCREMENTING ONTICK"));
	
	Timer -= DeltaTime;
	if (Timer <= 0.0f || Need->Toilet <= 0.005f) {
		Owner->MakeDecision();
		Timer = 2.0f;
	}
}

void AILifeGuardTask_Excrementing::Exit() {
	Owner->StopInteractWithBuilding();
	Owner->OnEndExcrementing.Broadcast(Owner);
	Owner->OnEndExcrementingEvent();
}

void AILifeGuardTask_Excrementing::Reset() {
	Timer = FMath::RandRange(TaskData->ExcrementingMinTime, TaskData->ExcrementingMaxTime);
}
