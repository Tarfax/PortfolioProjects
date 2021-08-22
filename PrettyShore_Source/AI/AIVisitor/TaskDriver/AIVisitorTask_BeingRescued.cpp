#include "AIVisitorTask_BeingRescued.h"


#include "Team9Assemble/AI/AI_BeachCharacter.h"
#include "Team9Assemble/AI/AILifeguard/AILifeGuard_Controller.h"
#include "Team9Assemble/AI/AIVisitor/AIVisitor_Controller.h"

void AIVisitorTask_BeingRescued::Enter() {
	Owner->SetDebugText.Broadcast("Being rescued by senpai <33");
	Owner->DoingTask = "Being Rescued";
	Owner->OnBeginBeingSaved.Broadcast(Owner);
	Owner->OnBeginBeingSavedEvent();
	Reset();
}	


void AIVisitorTask_BeingRescued::OnTick(float DeltaTime) {
	if (Owner->LifeGuardRescuer != nullptr) {
		FTransform LifeguardTransform = Owner->LifeGuardRescuer->AI_BeachCharacter->GetActorTransform();
		const FVector PositionOffset =  LifeguardTransform.GetLocation() +  ( LifeguardTransform.GetLocation().ForwardVector * -50 );
		LifeguardTransform.SetLocation(PositionOffset);
		Owner->AI_BeachCharacter->SetActorTransform(LifeguardTransform);
	}
	
	Timer -= DeltaTime;
	if (Timer < 0.0f) {
		Owner->MakeDecision();
		Timer = 5.0f;
	}
}

void AIVisitorTask_BeingRescued::Exit() {
	Owner->SetDebugText.Broadcast("Senpai touched me <33. I can die now.");
	Owner->OnEndBeingSaved.Broadcast(Owner);
	Owner->OnEndBeingSavedEvent();
}

void AIVisitorTask_BeingRescued::Reset() {
	Timer = 120.0f;
}
