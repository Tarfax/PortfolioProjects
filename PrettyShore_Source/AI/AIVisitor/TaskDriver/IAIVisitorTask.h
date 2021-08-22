#pragma once
#include "Team9Assemble/AI/AIVisitor/AIVisitor.h"
#include "Team9Assemble/Buildings/BuildingBase.h"
#include "Team9Assemble/Buildings/BuildingData.h"


class IAIVisitorTask {
public:
	IAIVisitorTask(AAIVisitor_Controller* Owner,
	               U_AIVistor_TaskData* TaskData,
	               F_AIVisitor_Mood* Mood,
	               F_AIVisitor_Need* Need)
		: Owner(Owner), TaskData(TaskData), Mood(Mood),
		  Need(Need) { }

	virtual ~IAIVisitorTask() { }

	virtual void Enter() = 0;

	void Tick(float DeltaTime) {
		NeedChange(DeltaTime);
		OnTick(DeltaTime);
	}

protected:
	virtual void OnTick(float DeltaTime) = 0;
public:
	virtual void Exit() = 0;
	virtual void Reset() = 0;

	void NeedChange(float DeltaTime) {
		Need->ChangeBodyTemperature( (DeltaTime * (TaskData->BodyTemperatureNormalRate + 
			(TaskData->BodyTemperatureNormalRate * TaskModifier.BodyTemperature) +
			(TaskData->BodyTemperatureNormalRate * BuildingMoodModifier.AIVisitor_BodyTemperature))));
		Need->ChangeToilet(DeltaTime * ((TaskData->ToiletNormalRate + 
			(TaskData->ToiletNormalRate * TaskModifier.Toilet) +
			(TaskData->ToiletNormalRate * BuildingMoodModifier.AIVisitor_Toilet))));
		Need->ChangeThirst(DeltaTime * ((TaskData->ThirstNormalRate + 
			(TaskData->ThirstNormalRate * TaskModifier.Thirst) +
			(TaskData->ThirstNormalRate * BuildingMoodModifier.AIVisitor_Thirst))));
		Need->ChangeHunger(DeltaTime * ((TaskData->HungerNormalRate + 
			(TaskData->HungerNormalRate * TaskModifier.Hunger) +
			(TaskData->HungerNormalRate * BuildingMoodModifier.AIVisitor_Hunger))));
	}

	float Timer;

	class AAIVisitor_Controller* Owner;
	U_AIVistor_TaskData* TaskData;
	F_AIVisitor_NeedModifier TaskModifier;

	// U_AIVistor_Need_NormalRate_DataAsset* NeedNormalRate_DataAsset;
	FBuildingMoodModifier BuildingMoodModifier;

	F_AIVisitor_Mood* Mood;
	F_AIVisitor_Need* Need;
};
