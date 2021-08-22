#include "AIVisitorTask_Dead.h"

#include "Team9Assemble/AI/AIBeach_Observer.h"

void AIVisitorTask_Dead::Enter() {
	Owner->SetDebugText.Broadcast("Dead");
	Owner->DoingTask = "Dead";

	Owner->IsAlive = false;
	Owner->OnDeathEvent();
	Owner->OnDeath.Broadcast(Owner);
	Owner->AIObserver->NotifyOnVisitorDeath();
	Owner->PerformingTask = E_AIVisitor_TaskType::Dead;
}

void AIVisitorTask_Dead::OnTick(float DeltaTime) {
	Timer -= DeltaTime;
	if (Timer <= DeltaTime) {
		if (Owner->IsBeingRescued == true) {
			if (Owner->LifeGuardRescuer != nullptr) {
				if (Owner->LifeGuardRescuer->RescuingVisitor == nullptr) {
					Owner->IsBeingRescued = false;
				} 
			} else {
				Owner->IsBeingRescued = false;
			}
		}
		Reset();
	}
}

void AIVisitorTask_Dead::Exit() { }

void AIVisitorTask_Dead::Reset() {
	Timer = 60.0f;
}
