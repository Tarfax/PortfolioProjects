#include "Tile.h"
#include "TileMap.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Team9Assemble/Buildings/BuildingBase.h"

UTile::UTile() {}

void UTile::Init(FVector Position, UTileMap* TileMap) {
	X = (Position.X);
	Y = (Position.Y);

	const FVector UpSide(X, Y + TileMap->TileSize, 0.0f);
	const FVector UpRightSide(X + TileMap->TileSize, Y + TileMap->TileSize, 0.0f);

	const FVector RightSide(X + TileMap->TileSize, Y, 0.0f);
	const FVector RightDownSide(X + TileMap->TileSize, Y - TileMap->TileSize, 0.0f);

	const FVector DownSide(X, Y - TileMap->TileSize, 0.0f);
	const FVector DownLeftSide(X - TileMap->TileSize, Y - TileMap->TileSize, 0.0f);

	const FVector LeftSide(X - TileMap->TileSize, Y, 0.0f);
	const FVector LeftUpSide(X - TileMap->TileSize, Y + TileMap->TileSize, 0.0f);


	Up = TileMap->GetTileFromWorldPosition(UpSide);
	Neighbours.Add(Up);
	UpRight = TileMap->GetTileFromWorldPosition(UpRightSide);
	Neighbours.Add(UpRight);

	Right = TileMap->GetTileFromWorldPosition(RightSide);
	Neighbours.Add(Right);
	RightDown = TileMap->GetTileFromWorldPosition(RightDownSide);
	Neighbours.Add(RightDown);

	Down = TileMap->GetTileFromWorldPosition(DownSide);
	Neighbours.Add(Down);
	DownLeft = TileMap->GetTileFromWorldPosition(DownLeftSide);
	Neighbours.Add(DownLeft);

	Left = TileMap->GetTileFromWorldPosition(LeftSide);
	Neighbours.Add(Left);
	LeftUp = TileMap->GetTileFromWorldPosition(LeftUpSide);
	Neighbours.Add(LeftUp);
}

bool UTile::CanPlaceBuilding() {
	if (PlacedBuilding == nullptr) {
		return true;
	}

	if (PlacedBuilding->BuildingData->BuildingType == E_BuildingType::VisitorHome && PlacedBuilding->BuildingData->
		BuildingName == "Towel") {
		return true;
	}

	// if (PlacedBuilding->BuildingData->BuildingType == EBuildingType::Cosmetica) {
	// 	return true;
	// }

	return false;
}

bool UTile::CanReserv(AAIVisitor_Controller* Visitor) {
	if (ReservedBy == nullptr && PlacedBuilding == nullptr) {
		ReservedBy = Visitor;
		return true;

	}
	return false;
}

bool UTile::CanReservAsHome() {
	TSet<UTile*> CollectedTiles;
	for (int i = 0; i < Neighbours.Num(); i++) {
		UTile* NeighbourTile = Neighbours[i];


		if (NeighbourTile != nullptr) {
			for (int j = 0; j < NeighbourTile->Neighbours.Num(); j++) {
				UTile* NeighbourTile2 = Neighbours[j];

				if (NeighbourTile2 != nullptr) {
					for (int k = 0; k < NeighbourTile2->Neighbours.Num(); k++) {
						UTile* NeighbourTile3 = Neighbours[k];

						if (NeighbourTile3 != nullptr) {
							for (int l = 0; l < NeighbourTile3->Neighbours.Num(); l++) {
								UTile* NeighbourTile4 = Neighbours[l];

								if (CollectedTiles.Contains(NeighbourTile4)) {
									continue;
								}
								CollectedTiles.Add(NeighbourTile4);

								if (NeighbourTile4 == nullptr || NeighbourTile4->PlacedBuilding != nullptr) {
									return false;
								}

							}
						}


					}
				}
			}
		}
	}
	return true;
}


void UTile::PlaceBuilding(ABuildingBase* BuildingBase, AAIVisitor_Controller* ReservedAI) {
	if (PlacedBuilding != nullptr && BuildingBase != PlacedBuilding) {
		PlacedBuilding->DeconstructBuilding();
	}

	PlacedBuilding = BuildingBase;

	if (ReservedBy != nullptr && ReservedAI == ReservedBy) {
		ReservedBy = nullptr;
		return;
	}

	if (ReservedBy != nullptr) {
		//Tell AI their tile is no longer available.

	}
}


void UTile::RemoveBuilding() {
	PlacedBuilding = nullptr;
}

void UTile::DrawDebug() {
	FVector Position = FVector(X + 25, Y + 25, 1.0f);
	// FPlane plane = UKismetMathLibrary::MakePlaneFromPointAndNormal(Position + (Position/2), FVector(0,0,1));

	FLinearColor Color = FLinearColor::Green;
	if (PlacedBuilding != nullptr) {
		Color = FLinearColor::Red;
	}
	UKismetSystemLibrary::DrawDebugBox(GetWorld(), Position, FVector(25), Color, FRotator::ZeroRotator, 10, 10);
}
