#include "AILifeGuardTask_DoNothing.h"
#include "Team9Assemble/AI/AILifeguard/AILifeGuard_Controller.h"

void AILifeGuardTask_DoNothing::Enter() {
	Owner->SetDebugText.Broadcast("Moving");
	Owner->DoingTask = "MoveTo";
	Reset();

}

void AILifeGuardTask_DoNothing::OnTick(float DeltaTime) {
	Timer-= DeltaTime;
	if (Timer <= 0.0f) {
		Reset();
		Owner->MakeDecision();
	}
}

void AILifeGuardTask_DoNothing::Exit() { }

void AILifeGuardTask_DoNothing::Reset() {
	Timer = 30.0f;
}
