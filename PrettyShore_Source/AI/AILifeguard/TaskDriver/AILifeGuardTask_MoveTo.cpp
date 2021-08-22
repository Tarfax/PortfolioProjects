#include "AILifeGuardTask_MoveTo.h"
#include "Team9Assemble/AI/AILifeguard/AILifeGuard_Controller.h"
#include "Team9Assemble/AI/AILifeguard/SensingComponents/VisionSensing.h"

void AILifeGuardTask_MoveTo::Enter() {
	Owner->SetDebugText.Broadcast("Moving");
	Owner->DoingTask = "MoveTo";
	DrownTimer = 2.5f;

}

void AILifeGuardTask_MoveTo::OnTick(float DeltaTime) {
	DrownTimer-= DeltaTime;
	if (DrownTimer <= 0.0f) {
		Owner->CheckForDrowners(Owner->VisionSensing->VisionLength);
		DrownTimer = 2.5f;
	}
}

void AILifeGuardTask_MoveTo::Exit() {
	
}

void AILifeGuardTask_MoveTo::Reset() {}
