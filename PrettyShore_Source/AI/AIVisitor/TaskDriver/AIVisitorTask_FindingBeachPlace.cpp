#include "AIVisitorTask_FindingBeachPlace.h"

#include "Team9Assemble/AI/AIVisitor/AIVisitor_Controller.h"

void AIVisitorTask_FindingBeachPlace::Enter() {
	Owner->SetDebugText.Broadcast("Finding Beach Place");
	Owner->DoingTask = "Finding Beach Place";
}

void AIVisitorTask_FindingBeachPlace::OnTick(float DeltaTime) {}
void AIVisitorTask_FindingBeachPlace::Exit() {}
void AIVisitorTask_FindingBeachPlace::Reset() {}
