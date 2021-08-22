#include "AIVisitorTask_AtHospital.h"
#include "Team9Assemble/AI/AILifeguard/AILifeGuard_Controller.h"
#include "Team9Assemble/AI/AIVisitor/AIVisitor_Controller.h"

void AIVisitorTask_AtHospital::Enter() {
	// Owner->SetDebugText.Broadcast("Recovering");	
	Owner->SetDebugText.Broadcast("Senpai touched me <33. I can die now.");
	Owner->DoingTask = "At Hospital";
	Owner->OnBeginHospital.Broadcast(Owner);
	Owner->OnBeginHospitalEvent();
	
	UE_LOG(LogTemp, Log, TEXT("AT HOSPITAL"));
	
	if (Owner->StartInteractWithBuilding() == true) {
		BuildingMoodModifier = Owner->InteractingBuilding->GetBuildingMoodModifier();
	}
	Reset();
}

void AIVisitorTask_AtHospital::OnTick(float DeltaTime) {
	Timer -= DeltaTime;

	if (Timer <= 0.0f) {
		Owner->MakeDecision();
		Timer = 6.0f;
	}
}

void AIVisitorTask_AtHospital::Exit() {
	Owner->StopInteractWithBuilding();	
	Owner->LifeGuardRescuer = nullptr;
	Owner->OnEndHospital.Broadcast(Owner);
	Owner->OnEndHospitalEvent();
	// Mood->TaskEnergy -= 10;
}

void AIVisitorTask_AtHospital::Reset() {
	Timer = TaskData->AtHospitalTime;
	UE_LOG(LogTemp, Log, TEXT("HOSPITAL RESET %f"), Timer);


}
