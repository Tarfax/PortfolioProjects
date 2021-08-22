// I like sand

#pragma once

#include "CoreMinimal.h"
#include "AIVisitor.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct F_AIVisitor_NeedModifier {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BodyTemperature = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Thirst = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Hunger = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Toilet = 1.0f;

	void ClearAll() {
		BodyTemperature = 0.0f;
		Thirst = 0.0f;
		Hunger = 0.0f;
		Toilet = 0.0f;
	}
};

USTRUCT(BlueprintType, Blueprintable)
struct F_AIVisitor_MoodModifier {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Happiness = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Stamina = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TaskEnergy = 0;
};


UENUM(BlueprintType, Blueprintable)
enum class E_AIVisitor_TaskType : uint8 {
	Nothing = 0 UMETA(DisplayName = "Nothing"),
	FindingPlace = 1 UMETA(DisplayName = "Finding a good spot"),
	Sunning = 2 UMETA(DisplayName = "Sunbathing"),
	Bathing = 3 UMETA(DisplayName = "Swimming"),
	Drinking= 4 UMETA(DisplayName = "Having a drink"),
	Eating = 5 UMETA(DisplayName = "Grabbing a bite"),
	Excrementing = 6 UMETA(DisplayName = "Taking a shit"),
	Drowning = 7 UMETA(DisplayName = "Drowning"),
	BeingSaved = 8 UMETA(DisplayName = "Being rescued"),
	Hospital = 9 UMETA(DisplayName = "Recovering"),
	GoingHome = 10 UMETA(DisplayName = "I'm done, going home"),
	Walking = 11 UMETA(DisplayName = "Walking"),
	Dead = 12 UMETA(DisplayName = "Dead"),
	Transported = 13 UMETA(DisplayName = "Transported"),
};

UENUM(BlueprintType, Blueprintable)
enum class E_AIVisitor_ModifierType : uint8 {
	DeathOnBeach = 0 UMETA(DisplayName = "Someone drowned"),
	NoAvailableToilet = 1 UMETA(DisplayName = "No toilets"),
	NoAvailableFoodStall = 2 UMETA(DisplayName = "No food"),
	NoAvailableDrinkStall = 3 UMETA(DisplayName = "No drinks"),
	NoAvailableSunbench= 4 UMETA(DisplayName = "No sun benches"),
	AlmostDrowned = 5 UMETA(DisplayName = "Almost drowned"),
	AllNeedsMet = 6 UMETA(DisplayName = "I like this place"),
};


UENUM(BlueprintType)
enum class E_AIVisitor_PerkType : uint8 {
	SlowPoke UMETA(DisplayName = "SlowPoke"),
	PreferBars UMETA(DisplayName = "PreferBars"),
};

UENUM(BlueprintType)
enum class E_AIVisitor_NeedType : uint8 {
	BodyTemperature UMETA(DisplayName = "BodyTemperature"),
	Thirst UMETA(DisplayName = "Thirst"),
	Hunger UMETA(DisplayName = "Hunger"),
	Toilet UMETA(DisplayName = "Toilet"),
};

UCLASS(Blueprintable)
class U_AIVistor_TaskData : public UDataAsset {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Normal Need Rates")
	float BodyTemperatureNormalRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sunning Task")
	F_AIVisitor_NeedModifier SunningModifierRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sunning Task")
	float SunningMinTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sunning Task")
	float SunningMaxTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Bathing Task")
	F_AIVisitor_NeedModifier BathingModifierRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Bathing Task")
	float BathingMinTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Bathing Task")
	float BathingMaxTime;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Normal Need Rates")
	float ThirstNormalRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Drinking Task")
	F_AIVisitor_NeedModifier DrinkingModifierRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Drinking Task")
	float DrinkingMinTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Drinking Task")
	float DrinkingMaxTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Normal Need Rates")
	float HungerNormalRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Eating Task")
	F_AIVisitor_NeedModifier EatingModifierRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Eating Task")
	float EatingMinTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Eating Task")
	float EatingMaxTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Normal Need Rates")
	float ToiletNormalRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Excrementing Task")
	F_AIVisitor_NeedModifier ExcrementingModifierRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Excrementing Task")
	float ExcrementingMinTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Excrementing Task")
	float ExcrementingMaxTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Walking/In Between State")
	F_AIVisitor_NeedModifier WalkingModifierRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Drowning Task")
	F_AIVisitor_NeedModifier DrowningModifierRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Drowning Task")
	float StartCheckToDrownTimer = 8;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Drowning Task")
	float CheckToDrownMin = 4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Drowning Task")
	float CheckToDrownMax = 8;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Drowning Task")
	float DrowningMinTime = 12;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Drowning Task")
	float DrowningMaxTime = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Drowning Task", AdvancedDisplay)
	FRuntimeFloatCurve ProbabilityOfDrowningCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AtHospital Task")
	F_AIVisitor_NeedModifier AtHospitalModifierRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AtHospital Task")
	float AtHospitalTime;
};

USTRUCT(BlueprintType)
struct F_AIVisitor_Mood {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Happiness;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Stamina;

	void ChangeStamina(float changeValue) {
		Stamina += changeValue;
		Stamina = FMath::Clamp(Stamina, 0.0f, 1.0f);
	};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TaskEnergy;
};

USTRUCT(BlueprintType)
struct F_AIVisitor_Need {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BodyTemperature;

	void ChangeBodyTemperature(float changeValue) {
		BodyTemperature += changeValue;
		BodyTemperature = FMath::Clamp(BodyTemperature, 0.0f, 1.0f);
	};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Thirst;

	void ChangeThirst(float changeValue) {
		Thirst += changeValue;
		Thirst = FMath::Clamp(Thirst, 0.0f, 1.0f);
	};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Hunger;

	void ChangeHunger(float changeValue) {
		Hunger += changeValue;
		Hunger = FMath::Clamp(Hunger, 0.0f, 1.0f);
	};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Toilet;

	void ChangeToilet(float changeValue) {
		Toilet += changeValue;
		Toilet = FMath::Clamp(Toilet, 0.0f, 1.0f);
	};
};

USTRUCT(BlueprintType)
struct F_AIVisitor_Info {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Age;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Physic;
};


USTRUCT(BlueprintType)
struct F_AIVisitor_Perk {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	E_AIVisitor_PerkType Perk;
};


USTRUCT(BlueprintType)
struct F_AIVisitor_HappinessCurves {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Body Temperature")
	FRuntimeFloatCurve BodyTemperatureHappinessCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Thirst")
	FRuntimeFloatCurve ThirstHappinessCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hunger")
	FRuntimeFloatCurve HungerHappinessCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Toilet")
	FRuntimeFloatCurve ToiletHappinessCurve;
};


USTRUCT(BlueprintType)
struct F_AIVisitor_TaskDriverWrapper {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float Timer;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool IsLocked;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	E_AIVisitor_TaskType LockedState;
};

USTRUCT(BlueprintType)
struct F_AIVisitor_Modifier {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int Multiplier = 1;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool IsBuff = false;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float Timer;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FString Description;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FString TooltipText;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	E_AIVisitor_ModifierType ModifierType;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	F_AIVisitor_NeedModifier NeedModifier;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	F_AIVisitor_MoodModifier MoodModifier;
};


UCLASS(Blueprintable)
class U_AINameGenerator : public UDataAsset {
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FString> FirstNames;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FString> LastNames;
};