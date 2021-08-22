#include "AILifeGuardTask_Saving.h"
#include "Team9Assemble/AI/AILifeguard/AILifeGuard_Controller.h"
#include "Team9Assemble/AI/AIVisitor/AIVisitor_Controller.h"

void AILifeGuardTask_Saving::Enter() {
	Owner->SetDebugText.Broadcast("Saving an asshole");
	Owner->DoingTask = "Saving";
	Owner->OnBeginSavingEvent();
	Owner->OnBeginSaving.Broadcast(Owner);
	Owner->GoingToSave();
	
	UE_LOG(LogTemp, Log, TEXT("LIFEGUARD HAS ENTERED SAVING STATE"));
	
}

void AILifeGuardTask_Saving::OnTick(float DeltaTime) {
	Timer += DeltaTime;
	if (Owner->RescuingVisitor == nullptr && Timer > 5.0f) {
		Owner->MakeDecision();
		Reset();
	}
}

void AILifeGuardTask_Saving::Exit() {
	Owner->OnEndSaving.Broadcast(Owner);
	Owner->OnEndSavingEvent();
	UE_LOG(LogTemp, Log, TEXT("LIFEGUARD HAS EXITED SAVING STATE"));

}

void AILifeGuardTask_Saving::Reset() {
	Timer = 0.0f;
}
