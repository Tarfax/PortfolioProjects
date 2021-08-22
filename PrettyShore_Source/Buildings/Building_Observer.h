// I like sand

#pragma once

#include "CoreMinimal.h"
#include "BuildingData.h"
#include "Building_Observer.generated.h"

class ABuildingBase;
class UBeachWork;
class ABeachGameState;
class AAIBeach_Controller_Base;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBuildingConstructed);

UCLASS(Blueprintable, BlueprintType)
class TEAM9ASSEMBLE_API UBuilding_Observer : public UObject {
	GENERATED_BODY()
	public:
	UBuilding_Observer();
	void AddBuilding(ABuildingBase* Building);
	void RemoveBuilding(ABuildingBase* Building);

	void AddNeedsStocking(UBeachWork* Building);
	void RemoveNeedsStocking(UBeachWork* Building);

	void AddNeedsCleaning(UBeachWork* Building);
	void RemoveNeedsCleaning(UBeachWork* Building);

	void AddNeedsHomeRemoval(UBeachWork* Building);
	void RemoveNeedsHomeRemoval(UBeachWork* Building);

	UFUNCTION(BlueprintCallable)
	int CountBuildingOfType(E_BuildingType Type);

	UFUNCTION(BlueprintCallable)
	int CountBuildingOfName(E_BuildingType Type, FString NameOfBuilding);

	UPROPERTY(BlueprintAssignable)
	FOnBuildingConstructed OnBuildingConstructed;
	
	ABuildingBase* GetClosestBuildingOfType(E_BuildingType Type, FVector SearchPosition,bool IgnoreMaxUsers, AAIBeach_Controller_Base* AIController);
	ABuildingBase* GetAvailableHome();

	TArray<ABuildingBase*> Buildings; 
	
	TMap<E_BuildingType, TArray<ABuildingBase*>> BuildingTypes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UBeachWork*> NeedsRestocking;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UBeachWork*> NeedsCleaning;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UBeachWork*> NeedsHomeRemoval;

	bool HasWork() const;
	UBeachWork* GetWork(FVector StartLocation);

	UPROPERTY()
	ABeachGameState* GameState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Decoration")
	int TotalDecoration;
};
