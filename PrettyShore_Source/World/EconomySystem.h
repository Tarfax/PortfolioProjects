#pragma once
#include "GameFramework/Actor.h"
#include "EconomySystem.generated.h"

class U_Time_Economy_DataAsset;
class AEconomySystem;
class ABeachGameState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndOfDayUpdate, AEconomySystem*, EconomySystem);

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNotificationUpdate, AEconomySystem*, EconomySystem);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUiUpdate, AEconomySystem*, EconomySystem, bool, bIsEndOfDay);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUiUpdate, FString, MessageToLog);

UCLASS()
class AEconomySystem : public AActor
{
	GENERATED_BODY()
public:
	AEconomySystem();

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintAssignable)
	FOnEndOfDayUpdate OnEndOfDayUpdate;

	//UPROPERTY(BlueprintAssignable)
	//FOnNotificationUpdate OnNotificationUpdate;

	UPROPERTY(BlueprintAssignable)
	FOnUiUpdate OnUiUpdate;

	//void ReceiveEconomicUpdate(int32 EconomicChange);
	
	//UFUNCTION(BlueprintCallable)
	//void PurchaseBuilding(int32 BuildingCost);

	UFUNCTION(BlueprintCallable)
	void AddToIncome(int32 Amount, AActor* Sender);

	UFUNCTION(BlueprintCallable)
	void AddToExpenses(int32 Amount, AActor* Sender);

	UFUNCTION(BlueprintCallable)
	void AddToUpkeep(int32 Amount);
	
	void CompleteSetup();

	UFUNCTION(BlueprintCallable)
	int32 GetBankAmount() const {
		return PlayerBankTotal;
	}

	UFUNCTION(BlueprintCallable)
	int32 GetBankDayBefore() const{
		return PlayerBankDayBefore;
	}

	UFUNCTION(BlueprintCallable)
	int32 GetBankDiff() const{
		return PlayerBankDifferenceFromDayBefore;
	}

	UFUNCTION(BlueprintCallable)
	int32 GetLatestDiff() const{
		return LatestDiff;
	}

	UFUNCTION(BlueprintCallable)
	int32 GetTotalIncome() const{
		return TotalIncome;
	}

	UFUNCTION(BlueprintCallable)
	int32 GetTotalExpenditure() const{
		return TotalExpenditure;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PlayerBankTotal;

	UFUNCTION(BlueprintPure)
	int GetLifeguardSalary();
	UFUNCTION(BlueprintPure)
	int GetUpKeepCost();

private:
	UFUNCTION()
	void EndOfDayUpdate();

	//UFUNCTION()
	//void NotificationUpdate();

	void UpdateUI(bool bIsEndOfDay);


	UPROPERTY()
	U_Time_Economy_DataAsset* Data;

	UPROPERTY()
	ABeachGameState* GameState;

	int32 PlayerBankDayBefore;
	int32 PlayerBankDifferenceFromDayBefore;
	int32 PlayerSalary;
	int32 UpkeepAmount;
	int32 LatestDiff;
	int32 TotalIncome;
	int32 TotalExpenditure;
};
