#pragma once
#include "Team9Assemble/AI/AIBeach_Controller_Base.h"
#include "Team9Assemble/AI/BehaviourTree/Tasks/BTTask_Helper.h"

#include "BuildingData.generated.h"

class ABuildingBase;

UENUM(BlueprintType)
enum class E_BuildingType : uint8 {
	WatchTower = 0 UMETA(DisplayName = "Watch Tower"),
	Hospital = 1 UMETA(DisplayName = "Hospital"),
	Drinkstall= 2 UMETA(DisplayName = "Drinkstall"),
	Foodstall= 3 UMETA(DisplayName = "Foodstall"),
	Toilet= 4 UMETA(DisplayName = "Toilet"),
	StaffRoom= 5 UMETA(DisplayName = "Staff Room"),
	VisitorHome= 6 UMETA(DisplayName = "Visitor Home"),
	Cosmetica= 7 UMETA(DisplayName = "Cosmetica"),
	GoingHome= 8 UMETA(DisplayName = "Going Home"),

};

UENUM(BlueprintType)
enum class EBuildingUnlock : uint8 {
	One UMETA(DisplayName = "One"),
	Two UMETA(DisplayName = "Two"),
	Three UMETA(DisplayName = "Three")
};

UENUM(BlueprintType)
enum class EConstructionFailType : uint8 {
	NotEnoughMoney UMETA(DisplayName = "Not Enough Money"),
	InvalidPlacement UMETA(DisplayName = "Invalid Placement"),
};

USTRUCT(BlueprintType)
struct FBuildingMoodModifier {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI Visitor")
	float AIVisitor_BodyTemperature = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI Visitor")
	float AIVisitor_Toilet = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI Visitor")
	float AIVisitor_Thirst = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI Visitor")
	float AIVisitor_Hunger = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI Life Guard")
	float AILifeGuard_Rest = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI Life Guard")
	float AILifeGuard_Recreation = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI Life Guard")
	float AILifeGuard_Toilet = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI Life Guard")
	float AILifeGuard_Salary = 0.0f;
};


USTRUCT(BlueprintType)
struct F_BuildingInventory {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Customer")
	FString ItemDescription = "Banejna";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Customer")
	int Price = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Customer")
	int AvailableStock = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Upkeep")
	int MaxStock = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Upkeep")
	int PricePerInventoryStock = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Upkeep")
	int UpkeepCost = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, AdvancedDisplay, Category="Upkeep")
	bool UnlimitedStock;
};

UCLASS(Blueprintable)
class U_Building_DataAsset : public UDataAsset {
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Buy Building")
	FString BuildingName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Buy Building")
	FString Description;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Buy Building")
	int CostToBuy = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Buy Building")
	int CostToSell = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Buy Building")
	UTexture2D* ButtonSprite;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Placement")
	bool IsDraggable = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Placement")
	int Extent_X = 1; 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Placement")
	int Extent_Y = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Progression")
	FString HowToUnlockDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Progression")
	TArray<TSubclassOf<ABuildingBase>> IsUnlockedBy;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Progression")
	int UnlockedByStarLevel;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Building Type")
	E_BuildingType BuildingType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Building Type")
	EAI_CustomAnimations WaitingAnimation;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Building Type")
	EAI_CustomAnimations InteractAnimation;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Inventory")
	F_BuildingInventory Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Work")
	bool WillRequireCleaning = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Work")
	int UsesUntilCleaning = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Building Behaviour")
	int MaxUsers = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Deocartion Value")
	int DecorationValue = 1;

	UPROPERTY(EditDefaultsOnly, Category="Mood Modifier")
	FBuildingMoodModifier BuildingMoodModifier;

	UPROPERTY(EditDefaultsOnly, Category="For Watch towers only!")
	float WatchTowerVisionRange;
	UPROPERTY(EditDefaultsOnly, Category="For Garbage Bin only!")
	float GarbageCollectRange;
};

USTRUCT(BlueprintType)
struct F_BuildingInteractionData {
	GENERATED_BODY()
	public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int Index;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    AAIBeach_Controller_Base* AIController;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    bool IsOccupied;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    FTransform InteractionTransform;
};