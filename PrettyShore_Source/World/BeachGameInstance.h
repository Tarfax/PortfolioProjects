// I like sand

#pragma once

#include "CoreMinimal.h"
#include "BeachGameInstance.generated.h"

class AAI_BeachArea;
class AAI_WaterArea;
class UAIBeach_Observer;
class UBuilding_Observer;
class UTileMap;
class U_Time_Economy_DataAsset;
class AWorld_Time;
class AEconomySystem;


UCLASS()
class TEAM9ASSEMBLE_API UBeachGameInstance : public UGameInstance {
	GENERATED_BODY()

public:
	UBeachGameInstance();
	virtual void Init() override;


	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Time & Economy Data")
	// U_Time_Economy_DataAsset* TimeEconomyData;
	//
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Systems")
	// AWorld_Time* WorldTime;
	//
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Systems")
	// AEconomySystem* EconomySystem;
};
