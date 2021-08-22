#pragma once
#include "EconomySystem.h"
#include "RatingData.h"

#include "Team9Assemble/AI/AITrash_Observer.h"
#include "Team9Assemble/Buildings/Building_Observer.h"

#include "RatingSystem.generated.h"

UCLASS(Blueprintable, BlueprintType)
class ARatingSystem : public AActor {
	GENERATED_BODY()
public:
	ARatingSystem();
	virtual void BeginPlay() override;
	void CalculateHappiness(float Percentage);
	bool CheckRequirements(int EarningsThreshold, bool RequireEarnings, float HappinessThreshold, bool RequireHappiness, int DecorThreshold, bool RequireDecor, 	TArray<TSubclassOf<ABuildingBase>> BuildingRequirements, bool RequireBuildings, int CurrentLifeguardCount, bool RequireLifeguard);
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category="Rating System", BlueprintPure)
	float GetHappinessGoal();
	UFUNCTION(BlueprintCallable, Category="Rating System", BlueprintPure)
	int GetEarningGoal();
	UFUNCTION(BlueprintCallable, Category="Rating System", BlueprintPure)
	int GetDecorGoal();
	UFUNCTION(BlueprintCallable, Category="Rating System", BlueprintPure)
	int GetLifeguardGoal();

	UFUNCTION(BlueprintCallable, Category="Rating System", BlueprintPure)
	bool ShowHappiness();
	UFUNCTION(BlueprintCallable, Category="Rating System", BlueprintPure)
	bool ShowEarnings();
	UFUNCTION(BlueprintCallable, Category="Rating System", BlueprintPure)
	bool ShowDecor();
	UFUNCTION(BlueprintCallable, Category="Rating System", BlueprintPure)
	bool ShowBuildables();
	UFUNCTION(BlueprintCallable, Category="Rating System", BlueprintPure)
	bool ShowLifeguards();

	UFUNCTION(BlueprintCallable, Category="Rating System", BlueprintPure)
	TArray<TSubclassOf<ABuildingBase>> GetBuildingRequirements();

	UPROPERTY(VisibleAnywhere, Category="References")
	UAITrash_Observer* TrashObserver;

	UPROPERTY(VisibleAnywhere, Category="References")
	UAIBeach_Observer* AIObserver;

	UPROPERTY(VisibleAnywhere, Category="References")
	UBuilding_Observer* BuildingObserver;

	UPROPERTY(VisibleAnywhere)
	float Timer;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Rating System")
	int VisitorCount;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Rating System")
	int Earned;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Rating System")
	int DecorValue;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Rating System")
	float AvarageHappiness;

	UFUNCTION(BlueprintCallable, Category="Rating System", BlueprintPure)
	int GetBeachStarLevel();

	UFUNCTION(BlueprintCallable)
	void AddEarnings(int Earnings);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Rating System")
	URatingData* RatingData;

private:
	uint8 CurrentRating = 1;
};
