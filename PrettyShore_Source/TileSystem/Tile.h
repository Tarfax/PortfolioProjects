#pragma once
#include "Tile.generated.h"

class AAIVisitor_Controller;
class ABuildingBase;
class UTileMap;

UCLASS(BlueprintType, Blueprintable)
class UTile : public UObject {
	GENERATED_BODY()
public:
	UTile();
	void Init(FVector Position, UTileMap* TileMap);

	UPROPERTY(VisibleAnywhere)
	int X;
	UPROPERTY(VisibleAnywhere)
	int Y;

	UPROPERTY(VisibleAnywhere)
	int XWorld;
	UPROPERTY(VisibleAnywhere)
	int YWorld;

	UFUNCTION(BlueprintCallable)
	FVector GetLocation() {
		return FVector(X, Y, 0.0f);
	}

	UPROPERTY(VisibleAnywhere)
	ABuildingBase* PlacedBuilding = nullptr;

	UPROPERTY(VisibleAnywhere)
	AAIVisitor_Controller* ReservedBy = nullptr;

	UFUNCTION(BlueprintCallable)
	bool CanPlaceBuilding();

	UFUNCTION(BlueprintCallable)
	bool CanReserv(AAIVisitor_Controller* Visitor);
	bool CanReservAsHome();

	UFUNCTION(BlueprintCallable)
	void PlaceBuilding(ABuildingBase* BuildingBase, AAIVisitor_Controller* ReservedAI);
	UFUNCTION(BlueprintCallable)
	void RemoveBuilding();

	void DrawDebug();

	TArray<UTile*> Neighbours;

	UPROPERTY(VisibleAnywhere)
	UTile* Up;
	UPROPERTY(VisibleAnywhere)
	UTile* UpRight;

	UPROPERTY(VisibleAnywhere)
	UTile* Right;
	UPROPERTY(VisibleAnywhere)
	UTile* RightDown;

	UPROPERTY(VisibleAnywhere)
	UTile* Down;
	UPROPERTY(VisibleAnywhere)
	UTile* DownLeft;

	UPROPERTY(VisibleAnywhere)
	UTile* Left;
	UPROPERTY(VisibleAnywhere)
	UTile* LeftUp;

};
