#include "AIVisitorTask_Sunning.h"
#include "Team9Assemble/AI/AIVisitor/AIVisitor.h"
#include "Team9Assemble/AI/AIVisitor/AIVisitor_Controller.h"
#include "Team9Assemble/Buildings/Building_AIHome.h"


void AIVisitorTask_Sunning::Enter() {
	Owner->SetDebugText.Broadcast("Sunning");
	Owner->DoingTask = "Sunning";


	
	if (Owner->StartSunning() == true) {
		BuildingMoodModifier = Owner->Home->GetBuildingMoodModifier();

		if (Owner->HasSunbench == true) {
			Owner->OnBeginSunningBenchEvent();
			Owner->OnBeginSunningBench.Broadcast(Owner);
		} else {
			Owner->OnBeginSunningEvent();
			Owner->OnBeginSunning.Broadcast(Owner);
		}
	}
	else {
		Owner->MakeDecision();
		return;
	}

	Reset();
}

void AIVisitorTask_Sunning::OnTick(float DeltaTime) {
	Timer -= DeltaTime;
	if (Timer <= 0.0f || Need->BodyTemperature > 0.98f
		//  || Need->Toilet > 0.98f || Need->Hunger > 0.98f || Need->Thirst > 0.98f
		) {
		Owner->MakeDecision();
		Timer = 2.0f;
	}


	if (DrainStaminaTimer <= 0.0f) {
		Mood->Stamina += 0.01;
		DrainStaminaTimer = 10.0f;
	}
}

void AIVisitorTask_Sunning::Exit() {
	Mood->TaskEnergy -= 1;

	Owner->StopSunning();

	if (Owner->HasSunbench == true) {
		Owner->OnEndSunningBenchEvent();
		Owner->OnEndSunningBench.Broadcast(Owner);
	} else {
		Owner->OnEndSunningEvent();
		Owner->OnEndSunning.Broadcast(Owner);
	}
	
}

void AIVisitorTask_Sunning::Reset() {
	Timer = FMath::RandRange(TaskData->SunningMinTime, TaskData->SunningMaxTime);
	DrainStaminaTimer = 10.0f;
}
