#pragma once

#include "TileMap.generated.h"

class UTile;

UCLASS(BlueprintType, Blueprintable)
class UTileMap : public UObject{
	GENERATED_BODY()
public:
	UTileMap();
	// ~UTileMap();
	void Add(UTile* Tile, int X, int Y);
	
	UFUNCTION(BlueprintCallable, Category = "Tile Map")
	void Init(FVector Origin, FVector Extent);
	
	UFUNCTION(BlueprintCallable, Category = "Tile Map")
	UTile* GetTileFromWorldPosition(FVector Position);

public: //Variables
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int TileSize = 50;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<UTile*> Tiles;

	TMap<FVector, int> Positions;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int Count = 0;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int Width;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int Height;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FVector MinPosition;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FVector MaxPosition;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FVector Origin;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FVector BoxExtent;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Classes")        
	TSubclassOf<UTile> Tile_Class;

	UFUNCTION(BlueprintCallable, Category = "Tile Map")
	FVector GetLocalPosition(FVector Position) const;
	UFUNCTION(BlueprintCallable, Category = "Tile Map")
	int GetIndexFromLocalPosition(FVector Position) const;
	UFUNCTION(BlueprintCallable, Category = "Tile Map")
	int GetIndexFromWorldPosition(FVector Position) const;

};
