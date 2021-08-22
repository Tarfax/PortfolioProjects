#include "AILifeGuardTask_Training.h"
#include "Team9Assemble/AI/AILifeguard/AILifeGuard_Controller.h"

void AILifeGuardTask_Training::Enter() {
	Owner->SetDebugText.Broadcast("Training");
	Owner->DoingTask = "Training";
	
	
	if (Owner->StartInteractWithBuilding() == true) {
		BuildingMoodModifier = Owner->InteractingBuilding->GetBuildingMoodModifier();
	}
}

void AILifeGuardTask_Training::OnTick(float DeltaTime) {}

void AILifeGuardTask_Training::Exit() {
	Owner->StopInteractWithBuilding();
}

void AILifeGuardTask_Training::Reset() {
	
}
