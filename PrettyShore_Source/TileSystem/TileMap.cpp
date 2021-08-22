#include "TileMap.h"
#include "Tile.h"
#include "Kismet/KismetSystemLibrary.h"

UTileMap::UTileMap() { }

void UTileMap::Init(FVector OriginPos, FVector Extent) {
	this->Origin = OriginPos;
	this->BoxExtent = Extent;

	Width = FMath::RoundToInt(Extent.X * 2) / TileSize;
	Height = FMath::RoundToInt(Extent.Y * 2) / TileSize;

	const float XMinRange = OriginPos.X - (Extent.X);
	const float XMaxRange = OriginPos.X + (Extent.X);

	const float YMinRange = OriginPos.Y - (Extent.Y);
	const float YMaxRange = OriginPos.Y + (Extent.Y);

	MinPosition = FVector(XMinRange, YMinRange, 0.0f);
	MaxPosition = FVector(XMaxRange, YMaxRange, 0.0f);


	for (int x = XMinRange; x < XMaxRange; x += TileSize) {
		for (int y = YMinRange; y < YMaxRange; y += TileSize) {
			UTile* tile = NewObject<UTile>(this, Tile_Class);
			Add(tile, x, y);
			Count++;
		}
	}

	int Counter = 0;
	for (int y = YMinRange; y < YMaxRange; y += TileSize) {
		for (int x = XMinRange; x < XMaxRange; x += TileSize) {
			Tiles[Counter]->Init(FVector(x, y, 0.0f), this);
			Counter++;
		}
	}
}

void UTileMap::Add(UTile* Tile, int X, int Y) {
	Tiles.Add(Tile);
}

UTile* UTileMap::GetTileFromWorldPosition(FVector Position) {
	// UE_LOG(LogTemp, Log, TEXT("Position %f %f"), Position.X, Position.Y);
	// UE_LOG(LogTemp, Log, TEXT("MINPosin %f %f"), MinPosition.X, MinPosition.Y);
	// UE_LOG(LogTemp, Log, TEXT("MAXPosin %f %f"), MaxPosition.X, MaxPosition.Y);


	if (Position.X > MinPosition.X && Position.X < MaxPosition.X && Position.Y > MinPosition.Y && Position.Y <
		MaxPosition.Y) {

		FVector LocalPosition = GetLocalPosition(Position);
		int index = GetIndexFromLocalPosition(LocalPosition);

		if (index < Tiles.Num() && index > 0) {
			return Tiles[index];
		}
	}
	
	// UE_LOG(LogTemp, Log, TEXT("Returning NULLPTR from GetTileFromWorldPosition"));

	return nullptr;
}

FVector UTileMap::GetLocalPosition(FVector Position) const {
	// UKismetSystemLibrary::DrawDebugLine(GetWorld(), Position, Position + (FVector::UpVector * 1000), FLinearColor::Green, 0, 10);

	FVector NewPos = Position - Origin;
	NewPos.X += BoxExtent.X;
	NewPos.Y += BoxExtent.Y;

	const int XMod = int(NewPos.X) % TileSize;
	const int YMod = int(NewPos.Y) % TileSize;

	int X = NewPos.X - XMod;
	int Y = NewPos.Y - YMod;

	return FVector(X, Y, 0.0f);
}

int UTileMap::GetIndexFromLocalPosition(FVector Position) const {
	int index = (Position.Y / TileSize) * Width + (Position.X / TileSize);
	return index;
}

int UTileMap::GetIndexFromWorldPosition(FVector Position) const {
	FVector LocalPosition = GetLocalPosition(Position);
	int index = GetIndexFromLocalPosition(LocalPosition);
	return index;
}
