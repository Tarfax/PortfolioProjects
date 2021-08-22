#pragma once
#include "GameFramework/Actor.h"
#include "World_Time.generated.h"

class U_Time_Economy_DataAsset;

//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNotificationPing);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEndOfDayPing);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDaytimeHasBegan);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNighttimeHasBegan);


USTRUCT(BlueprintType)
struct FCurrentTime
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	float days;
	UPROPERTY(BlueprintReadOnly)
	float houers;
	UPROPERTY(BlueprintReadOnly)
	float minutes;
};

UCLASS()
class AWorld_Time : public AActor
{
	GENERATED_BODY()
public:
	AWorld_Time();
	//AWorld_Time(U_Time_Economy_DataAsset* TimeEconomyData);

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	FCurrentTime GetCurrentHours();

	UFUNCTION()
	float GetCurrentTimeNormalized();

	UPROPERTY(BlueprintReadOnly)
	U_Time_Economy_DataAsset* Data;

	UPROPERTY(BlueprintAssignable)
	//FNotificationPing NotificationPing;
	FEndOfDayPing EndOfDayPing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DayTimeCounter; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StartTime;

	UPROPERTY(BlueprintReadWrite)
	int32 AmountOfDaysSinceStartOfGame;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsDay;
	UPROPERTY(BlueprintAssignable)
	FDaytimeHasBegan OnDaytimeHasBegan;
	UPROPERTY(BlueprintAssignable)
	FNighttimeHasBegan OnNighttimeHasBegan;

private:

	int32 NotificationsPerDay;
	int32 NotificationsPerDayRemaining;
};
