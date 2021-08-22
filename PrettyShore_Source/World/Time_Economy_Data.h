#pragma once
#include "Time_Economy_Data.generated.h"

UCLASS()
class U_Time_Economy_DataAsset : public UDataAsset
{
	GENERATED_BODY()
public:

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Economy")
	int32 PlayerStartSum = 25000;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Economy")
	int32 PlayerSalary = 2000;

	/** Used to have the World_Time prompt other systems to do something more than once per day. Any number less than 1 equals to 1. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Time")
	int32 NotificationsPerDay = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Time")
	int32 DayLengthInSeconds = 60;
};
