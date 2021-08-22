#include "AILifeGuardTask_Baywatch.h"
#include "Team9Assemble/AI/AILifeguard/AILifeGuard_Controller.h"
#include "Team9Assemble/AI/AILifeguard/SensingComponents/VisionSensing.h"

void AILifeGuardTask_Baywatch::Enter() {
	Owner->SetDebugText.Broadcast("Baywatching");
	Owner->DoingTask = "Baywatch";
	Owner->OnBeginWatch.Broadcast(Owner);
	Owner->OnBeginWatchEvent();
	VisionSensing = Owner->VisionSensing;


	if (Owner->StartInteractWithBuilding() == true) {
		ExtraVision = Owner->InteractingBuilding->BuildingData->WatchTowerVisionRange;
		BuildingMoodModifier = Owner->InteractingBuilding->GetBuildingMoodModifier();
	}

	VisionSensing->VisionLength = Vision + ExtraVision;
	VisionSensing->DebugVision = true;

	Reset();
}

void AILifeGuardTask_Baywatch::OnTick(float DeltaTime) {
	if (HasVisitorToSave == true) {
		UE_LOG(LogTemp, Log, TEXT("HAS VISITOR TO SAVE STUCK?"));
		return;
	}

	DrownTimer -= DeltaTime;
	Timer -= DeltaTime;
	
	if (DrownTimer <= 0.0f) {
		HasVisitorToSave = Owner->CheckForDrowners(ExtraVision);
		DrownTimer = 2.5f;
		if (HasVisitorToSave == true) {
			UE_LOG(LogTemp, Log, TEXT("HAS VISITOR TO SAVE STUCK IN TIMER??"));

			return;
		}

		// UE_LOG(LogTemp, Warning, TEXT("DrownTimer<0.0f:: %f"), DrownTimer);

	}

	// VisionUpdateTimer -= DeltaTime;
	// if (VisionUpdateTimer <= 0.0f) {
	// 	Vision = Owner->Stats.Vision;
	// 	if (Owner->InteractingBuilding != nullptr) {
	// 		ExtraVision = Owner->InteractingBuilding->BuildingData->WatchTowerVisionRange;
	// 		BuildingMoodModifier = Owner->InteractingBuilding->GetBuildingMoodModifier();
	// 	}
	// 	VisionSensing->VisionLength = Vision + ExtraVision;
	//
	// 	VisionUpdateTimer = 2.0f;
	// }

	const bool NeedResting = (Need->Rest > 0.95f && Owner->IsTaskLocked(E_AILifeGuard_TaskType::Resting) == false);
	const bool NeedRecreation = (Need->Recreation > 0.55f && (Owner->IsTaskLocked(E_AILifeGuard_TaskType::Drinking) == false
		|| Owner->IsTaskLocked(E_AILifeGuard_TaskType::Eating) == false));
	const bool NeedToilet = (Need->Toilet > 0.85f && Owner->IsTaskLocked(E_AILifeGuard_TaskType::Excrementing) == false);
	if (Timer <= TaskData->BeachTaskMinTime / 2 && ( NeedResting || NeedRecreation || NeedToilet)) {
		Owner->MakeDecision();
		Reset();
	}

	if (Timer <= 0.0f) {
		Owner->MakeDecision();
		Reset();
	}
}

void AILifeGuardTask_Baywatch::Exit() {
	VisionSensing->VisionLength = Vision;
	VisionSensing->DebugVision = false;
	Owner->StopInteractWithBuilding();

	Owner->OnEndWatch.Broadcast(Owner);
	Owner->OnEndWatchEvent();

}

void AILifeGuardTask_Baywatch::Reset() {
	Timer = FMath::RandRange(Owner->TaskData->BaywatchMinTime, Owner->TaskData->BaywatchMaxTime);
	DrownTimer = 2.5f;
	HasVisitorToSave = false;
	// UE_LOG(LogTemp, Warning, TEXT("Batwatch_Task:: Reset Timer: %f"), Timer);


}
