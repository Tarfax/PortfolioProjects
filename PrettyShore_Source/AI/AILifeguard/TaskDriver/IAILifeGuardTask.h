#pragma once
#include "Team9Assemble/AI/AILifeguard/AILifeGuard.h"
#include "Team9Assemble/Buildings/BuildingData.h"
#include "Team9Assemble/AI/AILifeguard/AILifeGuard_Controller.h"
#include "Team9Assemble/Buildings/BuildingBase.h"

class AAILifeGuard_Controller;

class IAILifeGuardTask {
public:
	IAILifeGuardTask(AAILifeGuard_Controller* Owner,
	                 U_AILifeGuard_TaskData* TaskData,
	                 F_AILifeGuard_Mood* Mood,
	                 F_AILifeGuard_Need* Need,
	                 F_AILifeGuard_Info* Info)
		: Owner(Owner), TaskData(TaskData), Mood(Mood), Need(Need), Info(Info) { }

	virtual ~IAILifeGuardTask() { }

	virtual void Enter() = 0;

	void Tick(float DeltaTime) {
		NeedChange(DeltaTime);
		OnTick(DeltaTime);
	}

protected:
	virtual void OnTick(float DeltaTime) = 0;
	float Timer;
public:
	virtual void Exit() = 0;
	virtual void Reset() = 0;

	void NeedChange(float DeltaTime) {
		Need->ChangeRest(DeltaTime * ((TaskData->RestNormalRate + 
				(TaskData->RestNormalRate * TaskModifier.Rest) +
				(TaskData->RestNormalRate * BuildingMoodModifier.AILifeGuard_Rest)) *
			Owner->MoraleRestDrainCurve.GetRichCurve()->Eval(Mood->Morale)));
		Need->ChangeRecreation(DeltaTime * (TaskData->RecreationNormalRate + 
			(TaskData->RecreationNormalRate * TaskModifier.Recreation) +
			(TaskData->RecreationNormalRate * BuildingMoodModifier.AILifeGuard_Recreation)));
		Need->ChangeToilet(DeltaTime * (TaskData->ToiletNormalRate + 
			(TaskData->ToiletNormalRate * TaskModifier.Toilet) +
			(TaskData->ToiletNormalRate * BuildingMoodModifier.AILifeGuard_Toilet)));
	}


	class AAILifeGuard_Controller* Owner;

	U_AILifeGuard_TaskData* TaskData;
	F_AILifeGuard_NeedModifier TaskModifier;
	FBuildingMoodModifier BuildingMoodModifier;

	struct F_AILifeGuard_Mood* Mood;
	struct F_AILifeGuard_Need* Need;
	struct F_AILifeGuard_Info* Info;
};
