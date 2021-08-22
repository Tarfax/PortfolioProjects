// I like sand

#pragma once

#include "CoreMinimal.h"

#include "Team9Assemble/AI/AILifeguard/AILifeGuard_Controller.h"
#include "Team9Assemble/AI/Interfaces/AI_Workable.h"
#include "UObject/Object.h"
#include "BeachWork.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EBeachWorkType : uint8 {
	Nothing = 0,
	PickupGarbage = 1 UMETA(DisplayName = "Picking up garbage"),
	CleanBuilding = 2 UMETA(DisplayName = "Cleaning building"),
	LeaveRestock = 3 UMETA(DisplayName = "Delivering stock"),
	FetchStock= 4 UMETA(DisplayName = "Picking up stock"),
	RemoveTowel = 5 UMETA(DisplayName = "Removing towels"),
};


UCLASS(BlueprintType, Blueprintable)
class UBeachWork : public UObject {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float Timer;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	EBeachWorkType WorkType;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FTransform WorkLocation;
	IAI_Workable* BeachWorkObject;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	AAILifeGuard_Controller* WorkOwner = nullptr;

	void Cancel() { WorkOwner = nullptr; }
};

