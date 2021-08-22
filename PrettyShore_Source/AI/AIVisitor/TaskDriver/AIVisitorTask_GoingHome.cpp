#include "AIVisitorTask_GoingHome.h"

#include "Team9Assemble/AI/AIVisitor/AIVisitor_Controller.h"

void AIVisitorTask_GoingHome::Enter() {
	Owner->SetDebugText.Broadcast("Going Home");
	Owner->DoingTask = "Going Home";

	UE_LOG(LogTemp, Log, TEXT("AI WILL NOW GO HOME!"));
		
	if (Owner->Home != nullptr) {
		Owner->Home->Enter(Owner);
		Owner->Home->Leave(Owner);
	}

}

void AIVisitorTask_GoingHome::OnTick(float DeltaTime) {}

void AIVisitorTask_GoingHome::Exit() {}
void AIVisitorTask_GoingHome::Reset() {}
