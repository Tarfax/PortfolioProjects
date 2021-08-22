#include "World_Time.h"
#include "Time_Economy_Data.h"
#include "BeachGameState.h"
#include "Math/UnitConversion.h"
#include "Windows/LiveCoding/Private/External/LC_ClientCommandActions.h"

AWorld_Time::AWorld_Time() {
	PrimaryActorTick.bCanEverTick = true;
	// Data = Cast<UBeachGameInstance>(GetGameInstance())->TimeEconomyData;
	//
	// DayTimeCounter = Data->DayLengthInSeconds;
	// NotificationsPerDay = Data->NotificationsPerDay < 1 ? 1 : Data->NotificationsPerDay;
	// NotificationsPerDayRemaining = NotificationsPerDay;


	// UE_LOG(LogTemp, Log, TEXT("World_Time has been constructed."))
}

// AWorld_Time::AWorld_Time(U_Time_Economy_DataAsset* TimeEconomyData)
// {
// 	Data = TimeEconomyData;
//
// 	DayTimeCounter = Data->DayLengthInSeconds;
// 	NotificationsPerDay = Data->NotificationsPerDay < 1 ? 1 : Data->NotificationsPerDay;
// 	NotificationsPerDayRemaining = NotificationsPerDay;
//
// 	UE_LOG(LogTemp, Log, TEXT("World_Time has been constructed."))
// }

void AWorld_Time::BeginPlay() {
	// UE_LOG(LogTemp, Log, TEXT("World_Time: BeginPlay Start! <---"))

	Super::BeginPlay();

	auto* BeachGameState = Cast<ABeachGameState>(GetWorld()->GetGameState());
	if (BeachGameState == nullptr) {
		UE_LOG(LogTemp, Log, TEXT("World_Time.cpp; BeginPlay: GameState was a nullptr!"))
	}
	else {
		BeachGameState->SetWorldTime(this);

		Data = BeachGameState->TimeEconomyData;

		DayTimeCounter = StartTime;
		NotificationsPerDay = Data->NotificationsPerDay < 1 ? 1 : Data->NotificationsPerDay;
		NotificationsPerDayRemaining = NotificationsPerDay;

		// UE_LOG(LogTemp, Log, TEXT("World_Time: BeginPlay End! <---"))
		//UE_LOG(LogTemp, Log, TEXT("World_Time has completed its Setup! <---"))
	}
}

void AWorld_Time::Tick(const float DeltaTime) {
	Super::Tick(DeltaTime);

	DayTimeCounter += DeltaTime;
	/*
	if (NotificationsPerDayRemaining > 1)
	{
		const float DayLength = Data->DayLengthInSeconds;
		const float TimeToNotification = DayTimeCounter - (DayLength - (DayLength / NotificationsPerDayRemaining));
		if (TimeToNotification < 0.f)
		{
			NotificationsPerDayRemaining--;
			//Send Notification.
			UE_LOG(LogTemp, Log, TEXT("World_Time sending a notification ping..."))
			NotificationPing.Broadcast();
		}
	}
	*/
	FCurrentTime CurrentHours = GetCurrentHours();
	if (IsDay == false && CurrentHours.houers >= 7.35f && CurrentHours.houers <= 19.4f) {
		IsDay = true;
		OnDaytimeHasBegan.Broadcast();
		UE_LOG(LogTemp, Log, TEXT("DayTime!"));

	}
	else if (IsDay == true && CurrentHours.houers >= 19.5f) {
		IsDay = false;
		OnNighttimeHasBegan.Broadcast();
		UE_LOG(LogTemp, Log, TEXT("NightTime!"));
	}

	if (DayTimeCounter >= Data->DayLengthInSeconds) {
		DayTimeCounter -= Data->DayLengthInSeconds;
		NotificationsPerDayRemaining = NotificationsPerDay;
		AmountOfDaysSinceStartOfGame++;

		UE_LOG(LogTemp, Log, TEXT("World_Time sending a EndOfDay ping..."))
		EndOfDayPing.Broadcast();
	}
}

FCurrentTime AWorld_Time::GetCurrentHours() {
	FCurrentTime timeCapsual;

	float secondsIngame = (Data->DayLengthInSeconds);
	float minutesIngame = (((DayTimeCounter / secondsIngame) * 24) * 60);

	timeCapsual.days = AmountOfDaysSinceStartOfGame;
	timeCapsual.houers = ((DayTimeCounter / secondsIngame) * 24);
	timeCapsual.minutes = minutesIngame;

	return timeCapsual;
}

float AWorld_Time::GetCurrentTimeNormalized() {
	return DayTimeCounter / Data->DayLengthInSeconds;
}
