// I like sand

#pragma once

#include "CoreMinimal.h"
#include "AILifeGuard.generated.h"

USTRUCT(BlueprintType)
struct F_AILifeGuard_NeedModifier {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Rest = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Recreation= 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Toilet= 1.0f;
};

USTRUCT(BlueprintType, Blueprintable)
struct F_AILifeGuard_MoodModifier {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Morale = 0.0f;
};

UENUM(BlueprintType, Blueprintable)
enum class E_AILifeGuard_TaskType : uint8 {
	Nothing = 0 UMETA(DisplayName = "Doing Nothing"),
    Baywatch  = 1 UMETA(DisplayName = "Baywatching"),
    Saving  = 2 UMETA(DisplayName = "Rescuing guest"),
    Drinking  = 3 UMETA(DisplayName = "Getting a drink"),
    Eating  = 4 UMETA(DisplayName = "Lunch!"),
    Excrementing = 5 UMETA(DisplayName = "Taking a shit"),
    Training = 6 UMETA(DisplayName = "Training"),
    BeachWork = 7 UMETA(DisplayName = "Working..."),
	Resting = 8 UMETA(DisplayName = "Resting"),
    MoveTo = 9 UMETA(DisplayName = "Walking"),
	Roaming = 10 UMETA(DisplayName = "Patrolling"),
	HasSpawned = 11 UMETA(DisplayName = "Waiting"),
	HeldByPlayer = 12 UMETA(DisplayName = "Held by player"),


};

UENUM(BlueprintType)
enum E_AILifeGuard_NeedType {
	Rest UMETA(DisplayName = "Rest"),
	Recreation UMETA(DisplayName = "Recreation"),
	Toilet UMETA(DisplayName = "Toilet"),
	Salary UMETA(DisplayName = "Salary"),
};

UENUM(BlueprintType, Blueprintable)
enum class E_AILifeGuard_ModifierType : uint8 {
	DeathOnBeach = 0 UMETA(DisplayName = "Death On Beach"),
	NoAvailableToilet = 1 UMETA(DisplayName = "No Available Toilet"),
	NoAvailableFoodStall = 2 UMETA(DisplayName = "No Available FoodStall"),
	NoAvailableDrinkStall = 3 UMETA(DisplayName = "No Available DrinkStall"),
	NoAvailableStaffRoom= 4 UMETA(DisplayName = "No Available Sunbench"),
	SavedSomeone = 5 UMETA(DisplayName = "Saved Someone"),
	AllNeedsMet = 6 UMETA(DisplayName = "All Needs Met"),
	NoAvailableHospitals = 7 UMETA(DisplayName = "No available hospitals"),
	ImNotPaid = 8 UMETA(DisplayName = "I received no salary")
};

UCLASS(Blueprintable)
class U_AILifeGuard_TaskData : public UDataAsset {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Normal Need Rates")
	float RestNormalRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Resting")
	F_AILifeGuard_NeedModifier RestingModifierRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Resting")
	float RestingMinTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Resting")
	float RestingMaxTime;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Resting", AdvancedDisplay)
	FRuntimeFloatCurve StopTask_RestCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Normal Need Rates")
	float RecreationNormalRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Baywatch/Roaming")
	F_AILifeGuard_NeedModifier BaywatchModifierRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Baywatch/Roaming")
	float BaywatchMinTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Baywatch/Roaming")
	float BaywatchMaxTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Drinking")
	F_AILifeGuard_NeedModifier DrinkingModifierRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Drinking")
	float DrinkingMinTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Drinking")
	float DrinkingMaxTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Eating")
	F_AILifeGuard_NeedModifier EatingModifierRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Eating")
	float EatingMinTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Eating")
	float EatingMaxTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Normal Need Rates")
	float ToiletNormalRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Excrementing")
	F_AILifeGuard_NeedModifier ExcrementingModifierRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Excrementing")
	float ExcrementingMinTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Excrementing")
	float ExcrementingMaxTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BeachTask")
	F_AILifeGuard_NeedModifier BeachTaskModifierRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BeachTask")
	float BeachTaskMinTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BeachTask")
	float BeachTaskMaxTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Saving")
	F_AILifeGuard_NeedModifier SavingModifierRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Walking/In Between State")
	F_AILifeGuard_NeedModifier WalkingModifierRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Training")
	F_AILifeGuard_NeedModifier TrainingModifierRate;
};

USTRUCT(BlueprintType)
struct F_AILifeGuard_Mood {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Morale;
};

USTRUCT(BlueprintType)
struct F_AILifeGuard_Stats {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed = 650;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Strength = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Vision = 3500;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EnergyMaxLevel = 100;
};

USTRUCT(BlueprintType)
struct F_AILifeGuard_Need {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Rest;
	float MaxRestValue = 1.0f;
	void ChangeRest(float changeValue) {
		Rest += changeValue;
		Rest = FMath::Clamp(Rest, 0.0f, MaxRestValue);
	};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Recreation;
	void ChangeRecreation(float changeValue) {
		Recreation += changeValue;
		Recreation = FMath::Clamp(Recreation, 0.0f,1.0f);
	};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Toilet;
	void ChangeToilet(float changeValue) {
		Toilet += changeValue;
		Toilet = FMath::Clamp(Toilet, 0.0f,1.0f);
	};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Salary;
	void ChangeSalary(float changeValue) {
		UE_LOG(LogTemp, Log, TEXT("Where si this coming from?"));
		Salary += changeValue;
		Salary = FMath::Clamp(Salary, 0.0f,1.0f);
	};
};

USTRUCT(BlueprintType)
struct F_AILifeGuard_Info {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Age;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Salary;

};

USTRUCT(BlueprintType)
struct F_AILifeGuard_MoraleCurves {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Salary")
	FRuntimeFloatCurve SalaryMoraleCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rest")
	FRuntimeFloatCurve RestMoraleCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Recreation")
	FRuntimeFloatCurve RecreationMoraleCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Toilet")
	FRuntimeFloatCurve ToiletMoraleCurve;
};

USTRUCT(BlueprintType)
struct F_AILifeGuard_TaskDriverWrapper {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float Timer;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool IsLocked;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int FailedCount;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	E_AILifeGuard_TaskType LockedState;
};

USTRUCT(BlueprintType)
struct F_AILifeGuard_Modifier {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool IsBuff;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int Multiplier = 1;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float Timer;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FString Description;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FString TooltipText;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	E_AILifeGuard_ModifierType ModifierType;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	F_AILifeGuard_NeedModifier NeedModifier;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	F_AILifeGuard_MoodModifier MoodModifier;
};
