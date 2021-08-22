// I like sand


#include "DummyBuilding.h"
#include "NavModifierComponent.h"
#include "Building_AIHome.h"
#include "Building_Observer.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Team9Assemble/TileSystem/Tile.h"
#include "Team9Assemble/TileSystem/TileMap.h"
#include "Team9Assemble/World/BeachGameState.h"
#include "Team9Assemble/World/EconomySystem.h"

// Sets default values
ADummyBuilding::ADummyBuilding() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = SceneComponent;

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	BoxCollider->SetCollisionProfileName(TEXT("NoCollision"));
	BoxCollider->SetupAttachment(SceneComponent);

	Building = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Building"));
	Building->SetCollisionProfileName(TEXT("NoCollision"));
	Building->SetupAttachment(SceneComponent);

	ArrowEnter = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Enter"));
	ArrowEnter->SetCollisionProfileName(TEXT("NoCollision"));
	ArrowEnter->SetupAttachment(SceneComponent);

	ArrowExit = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Exit"));
	ArrowExit->SetCollisionProfileName(TEXT("NoCollision"));
	ArrowExit->SetupAttachment(SceneComponent);

	// NavModifierObstacle = CreateDefaultSubobject<UNavModifierComponent>(TEXT("NavObstacles"));
	// NavModifierNormal = CreateDefaultSubobject<UNavModifierComponent>(TEXT("NavNormal"));
	// NavModifierPath = CreateDefaultSubobject<UNavModifierComponent>(TEXT("NavPath"));
}

// Called when the game starts or when spawned
void ADummyBuilding::BeginPlay() {
	Super::BeginPlay();
	EconomySystem = Cast<ABeachGameState>(GetWorld()->GetGameState())->EconomySystem;

	ABeachGameState* GameState = Cast<ABeachGameState>(GetWorld()->GetGameState());
	TileMap = GameState->TileMap;

	if (ShowLifeGuardVision == true) {
		UBuilding_Observer* BuildingObserver = GameState->BuildingObserver;
		for (auto building : BuildingObserver->Buildings) {
			if (building->BuildingData->BuildingType == E_BuildingType::WatchTower) {
				building->ShowLifeGuardVision = true;
			}
		}
	}
}

// Called every frame
void ADummyBuilding::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	CheckIfPlacementIsValid();
	if (ShowLifeGuardVision == true) {
		const FVector Direction = FVector::ForwardVector;
		const FVector Origin = GetActorLocation();

		const FVector Right = Direction.RotateAngleAxis(35, FVector::UpVector);
		const FVector Left = Direction.RotateAngleAxis(-35, FVector::UpVector);

		UKismetSystemLibrary::DrawDebugLine(GetWorld(), Origin, Origin + Right * BuildingData->WatchTowerVisionRange,
		                                    FLinearColor::Yellow, 0, 15);
		UKismetSystemLibrary::DrawDebugLine(GetWorld(), Origin, Origin + Left * BuildingData->WatchTowerVisionRange,
		                                    FLinearColor::Yellow, 0, 15);
		UKismetSystemLibrary::DrawDebugLine(GetWorld(), Origin + Right * BuildingData->WatchTowerVisionRange,
		                                    Origin + Left * BuildingData->WatchTowerVisionRange, FLinearColor::Yellow,
		                                    0, 15);
	}
}

ABuildingBase* ADummyBuilding::TryPlaceDummyBuilding(bool ForceConstruction, bool IsVisitorHome) {
	int Money = EconomySystem->GetBankAmount();
	if (Money < BuildingData->CostToBuy && ForceConstruction == false) {
		OnConstructionFailed(EConstructionFailType::NotEnoughMoney);
		return nullptr;
	}

	CheckIfPlacementIsValid();
	if ((CanPlace == true && CanPlaceFromRaytrace == true && IsRotating == false) || ForceConstruction == true) {

		ABuildingBase* SpawnedBuilding = GetWorld()->SpawnActorDeferred<ABuildingBase>(
			BuildingBlueprint, SceneComponent->GetComponentTransform());

		if (SpawnedBuilding != nullptr) {
			SpawnedBuilding->BuildingData = BuildingData;
			SpawnedBuilding->ArrowEnter->SetWorldTransform(ArrowEnter->GetComponentTransform());
			SpawnedBuilding->ArrowExit->SetWorldTransform(ArrowExit->GetComponentTransform());

			SpawnedBuilding->Building->SetStaticMesh(this->Building->GetStaticMesh());
			SpawnedBuilding->Building->SetWorldTransform(this->Building->GetComponentTransform());
			// SpawnedBuilding->OutlineMesh->SetStaticMesh(this->Building->GetStaticMesh());
			// SpawnedBuilding->OutlineMesh->SetWorldTransform(this->Building->GetComponentTransform());
			// SpawnedBuilding->OutlineMesh->SetRelativeScale3D(SpawnedBuilding->OutlineMesh->GetRelativeScale3D() * 1.03f);

			SpawnedBuilding->PlacementColliderComp->SetRelativeTransform(BoxCollider->GetRelativeTransform());
			SpawnedBuilding->PlacementColliderComp->SetBoxExtent(BoxCollider->GetPlacementExtent().BoxExtent, true);

			// SpawnedBuilding->InteractionTransforms = InteractPoints;
			for (int i = 0; i < InteractPoints.Num(); i++) {
				F_BuildingInteractionData InteractionData;
				InteractionData.Index = i;
				InteractionData.InteractionTransform = InteractPoints[i];
				InteractionData.IsOccupied = false;
				SpawnedBuilding->InteractionData.Add(InteractionData);
			}
			UGameplayStatics::FinishSpawningActor(SpawnedBuilding, SceneComponent->GetComponentTransform());

			PlaceBuildingOnTiles(SpawnedBuilding, IsVisitorHome);

			SpawnedBuilding->ConstructBuilding();
			OnPlacingDummyBuilding(SpawnedBuilding);
			return SpawnedBuilding;
		}
	}
	OnConstructionFailed(EConstructionFailType::InvalidPlacement);
	return nullptr;
}

void ADummyBuilding::CheckIfPlacementIsValid() {
	int SizeX;
	int SizeY;
	float StartLocationX;
	float StartLocationY;

	bool HasCash = true;
	int Money = EconomySystem->GetBankAmount();
	if (Money < BuildingData->CostToBuy) {
		HasCash = false;
	}

	if (IsRotated == false) {
		SizeX = (BoxCollider->GetPlacementExtent().BoxExtent.X * 2) / TileMap->TileSize;
		SizeY = (BoxCollider->GetPlacementExtent().BoxExtent.Y * 2) / TileMap->TileSize;
		StartLocationX = BoxCollider->GetComponentLocation().X - BoxCollider->GetPlacementExtent().BoxExtent.X;
		StartLocationY = BoxCollider->GetComponentLocation().Y - BoxCollider->GetPlacementExtent().BoxExtent.Y;
	}
	else {
		SizeX = (BoxCollider->GetPlacementExtent().BoxExtent.Y * 2) / TileMap->TileSize;
		SizeY = (BoxCollider->GetPlacementExtent().BoxExtent.X * 2) / TileMap->TileSize;
		StartLocationX = BoxCollider->GetComponentLocation().X - BoxCollider->GetPlacementExtent().BoxExtent.Y;
		StartLocationY = BoxCollider->GetComponentLocation().Y - BoxCollider->GetPlacementExtent().BoxExtent.X;
	}

	FVector StartPosition = FVector(StartLocationX, StartLocationY, 0);
	UTile* Tile = TileMap->GetTileFromWorldPosition(StartPosition);
	if (Tile == nullptr) {
		CollectedTiles.Reset();
	}
	else if (Tile == StartCheckTile && Tile->CanPlaceBuilding()) {
		TSet<UTile*>::TIterator Iterator = CollectedTiles.CreateIterator();

		for (TSet<UTile*>::TIterator; Iterator; ++Iterator) {
			// FVector tilePos = FVector(Iterator.ElementIt->Value->X + 25, Iterator.ElementIt->Value->Y + 25, 0);
			// FLinearColor Color = FLinearColor::Green;

			if (Iterator.ElementIt->Value->CanPlaceBuilding() == false || HasCash == false) {
				// Color = FLinearColor::Red;
				CanPlace = false;
			}

			// UKismetSystemLibrary::DrawDebugBox(GetWorld(), tilePos, FVector(25, 25, 2), Color,
			//                                    FRotator::ZeroRotator, 0, 3);
		}

		// if (CanPlace == false)
		// 	UE_LOG(LogTemp, Log, TEXT("can place is false"));

		return;
	}

	StartCheckTile = Tile;

	CollectedTiles.Reset();
	CollectedTiles.Reserve(3000);

	// MarginCheckPlacement;
	int loopCount = 0;
	CanPlace = true;
	for (int i = 0; i < SizeX; i++) {
		for (int j = 0; j < SizeY; j++) {
			loopCount++;
			StartPosition = FVector(StartLocationX + (i * TileMap->TileSize),
			                        StartLocationY + (j * TileMap->TileSize), 0);

			UTile* TileToSearch2 = TileMap->GetTileFromWorldPosition(StartPosition);

			// if (TileToSearch2 != nullptr) {
			// 	CollectedTiles.Add(TileToSearch2);
			if (TileToSearch2 != nullptr) {
				CollectedTiles.Add(TileToSearch2);

				FLinearColor Color = FLinearColor::Green;
				if (TileToSearch2->CanPlaceBuilding() == false || HasCash == false) {
					// Color = FLinearColor::Red;
					CanPlace = false;
				}

				// FVector tilePos = FVector(TileToSearch2->X + 25, TileToSearch2->Y + 25, 0);
				// UKismetSystemLibrary::DrawDebugBox(GetWorld(), tilePos, FVector(25, 25, 2), Color,
				//                                    FRotator::ZeroRotator, 0, 3);
			}
			else {
				CanPlace = false;
			}
		}
		// else {
		// 	CanPlace = false;
		// }
	}
}

void ADummyBuilding::PlaceBuildingOnTiles(ABuildingBase* SpawnedBuilding, bool isVisitorHome) {
	if (isVisitorHome == true) {

		int SizeX = (BoxCollider->GetPlacementExtent().BoxExtent.X * 2) / TileMap->TileSize;
		int SizeY = (BoxCollider->GetPlacementExtent().BoxExtent.Y * 2) / TileMap->TileSize;
		float StartLocationX = BoxCollider->GetComponentLocation().X - BoxCollider->GetPlacementExtent().BoxExtent.X;
		float StartLocationY = BoxCollider->GetComponentLocation().Y - BoxCollider->GetPlacementExtent().BoxExtent.Y;

		for (int i = 0; i < SizeX; i++) {
			for (int j = 0; j < SizeY; j++) {
				FVector StartPosition = FVector(StartLocationX + (i * TileMap->TileSize),
				                                StartLocationY + (j * TileMap->TileSize), 0);

				UTile* TileToSearch2 = TileMap->GetTileFromWorldPosition(StartPosition);
				if (TileToSearch2 != nullptr) {
					CollectedTiles.Add(TileToSearch2);
					if (TileToSearch2 != nullptr) {
						CollectedTiles.Add(TileToSearch2);
					}
				}

			}
		}
	}

	TSet<UTile*>::TIterator Iterator = CollectedTiles.CreateIterator();
	for (TSet<UTile*>::TIterator; Iterator; ++Iterator) {
		Iterator.ElementIt->Value->PlaceBuilding(SpawnedBuilding, nullptr);
		SpawnedBuilding->Tiles.AddUnique(Iterator.ElementIt->Value);
	}
}

void ADummyBuilding::Delete() {
	if (ShowLifeGuardVision == true) {
		ABeachGameState* GameState = Cast<ABeachGameState>(GetWorld()->GetGameState());
		UBuilding_Observer* BuildingObserver = GameState->BuildingObserver;
		for (int i = 0; i < BuildingObserver->Buildings.Num(); i++) {
			ABuildingBase* building = BuildingObserver->Buildings[i];
			if (building->BuildingData->BuildingType == E_BuildingType::WatchTower) {
				building->ShowLifeGuardVision = false;
			}
		}
	}
	Destroy();
}
