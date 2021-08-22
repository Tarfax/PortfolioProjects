// I like sand

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BeachGameState.generated.h"

class AAI_BeachArea;
class AAI_WaterArea;
class UAIBeach_Observer;
class UBuilding_Observer;
class UTileMap;
class U_Time_Economy_DataAsset;
class AWorld_Time;
class AWorld_Light;
class AEconomySystem;
class ARatingSystem;
class UAITrash_Observer;
class AWorld_LightInterpolation;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnLogUpdate, FString, Message, FLinearColor, Color, AActor*, Sender);



UCLASS()
class TEAM9ASSEMBLE_API ABeachGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ABeachGameState();
	virtual void BeginPlay() override;
	virtual void HandleBeginPlay() override;

	void SetWorldTime(AWorld_Time* worldTime);
	// void SetWorldLight(AWorld_Light* worldLight);
	void SetEconomySystem(AEconomySystem* economySystem);
	void CheckReferenceSetupComplete();
	void DoCompleteSetupForMembers();

	UFUNCTION(BlueprintCallable)
	void SendLogMessage(FString Message, FLinearColor Color, AActor* Sender);

	private:
	UPROPERTY(BlueprintAssignable)
	FOnLogUpdate OnLogUpdate;
	
public:
	UPROPERTY(BlueprintReadWrite, Category="Locators")
	AAI_BeachArea* BeachArea;
	UPROPERTY(BlueprintReadWrite, Category="Locators")
	AAI_WaterArea* WaterArea;
	UPROPERTY(BlueprintReadWrite, Category="Locators")
	ARatingSystem* RatingSystem;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Locators")
	UBuilding_Observer* BuildingObserver;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Classes")
	TSubclassOf<UBuilding_Observer> BuildingObserver_Class;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Locators")
	UAIBeach_Observer* AIObserver;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Classes")
	TSubclassOf<UAIBeach_Observer> AIObserver_Class;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Locators")
	UTileMap* TileMap;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Classes")
	TSubclassOf<UTileMap> TileMap_Class;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Locators")
	UAITrash_Observer* TrashObserver;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Classes")
	TSubclassOf<UAITrash_Observer> TrashObserver_Class;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Time & Economy Data")
	U_Time_Economy_DataAsset* TimeEconomyData;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Systems")
	AWorld_Time* WorldTime;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Systems")
	AWorld_Light* WorldLight;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Systems")
	AWorld_LightInterpolation* WorldLightInterpolation;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Systems")
	AEconomySystem* EconomySystem;

	
};

