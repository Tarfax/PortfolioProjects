#pragma once


UENUM(BlueprintType)
enum class EAIType : uint8 {
	Visitor,
	LifeGuard
};

UENUM(BlueprintType, Blueprintable)
enum class EAI_CustomAnimations : uint8 {
	StartPutDownTowel = 0,
	Decide = 1,
	HappyResult= 2,
	UnhappyResult= 3,
	RunToWater= 4,
	AtSunbench= 5,
	Saving_StartSaving = 6,
	Saving_IsBeingSaved = 7,
	PutDown_FromCarrying = 8,
	PutDown_FromBeingCarried = 9,
	
	WaitOutsideRestaurant	= 10,
	EatAtRestaurant			= 11,
	WaitOutsideIceCream		= 12,
	EatAtIceCream			= 13,
	WaitOutsideBar			= 14,
	DrinkAtBar				= 15,
	WaitOutsideStaffroom	= 16,
	Rest					= 17,
	WaitOutsideToilet		= 18,
	Excrement				= 19,
	WaitOutsideWatchtower	= 20,
	Baywatching				= 21,
	PickingUpTrash = 22,
	RemovingTowel = 23,
	CleaningBuilding = 24,
	Restocking = 25,
	HeldByPlayer = 26,
};

UENUM(BlueprintType)
enum class EAIBuildingInteractionType : uint8 {
	Waiting,
	Interacting
};

