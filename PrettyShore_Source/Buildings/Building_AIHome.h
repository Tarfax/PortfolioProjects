#pragma once
#include "BuildingBase.h"
#include "Building_AIHome.generated.h"

UCLASS(BlueprintType)
class ABuilding_AIHome : public ABuildingBase {
	GENERATED_BODY()
public:
	ABuilding_AIHome();
	virtual void BeginPlay() override;

	//
	// UPROPERTY(VisibleAnywhere)
	// AAIVisitor_Controller* AIVisitor_Owner;

};
