#include "AIVisitorTask_TransportingDead.h"
#include "Team9Assemble/AI/AI_BeachCharacter.h"
#include "Team9Assemble/AI/AILifeguard/AILifeGuard_Controller.h"
#include "Team9Assemble/AI/AIVisitor/AIVisitor_Controller.h"

void AIVisitorTask_TransportingDead::Enter() {
	Owner->SetDebugText.Broadcast("Senpai touched me <33. But it was to late. =(");
	Owner->DoingTask = "Transported Dead";
	Owner->OnBeginBeingTransported.Broadcast(Owner);
	Owner->OnBeginBeingTransportedEvent();
}

void AIVisitorTask_TransportingDead::OnTick(float DeltaTime) {
	if (Owner->LifeGuardRescuer != nullptr) {
		FTransform LifeguardTransform = Owner->LifeGuardRescuer->AI_BeachCharacter->GetActorTransform();
		const FVector PositionOffset =  LifeguardTransform.GetLocation() +  ( LifeguardTransform.GetLocation().ForwardVector * -50 );
		LifeguardTransform.SetLocation(PositionOffset);
		Owner->AI_BeachCharacter->SetActorTransform(LifeguardTransform);
	}
}

void AIVisitorTask_TransportingDead::Exit() {}

void AIVisitorTask_TransportingDead::Reset() {}
