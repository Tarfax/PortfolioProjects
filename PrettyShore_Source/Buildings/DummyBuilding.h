// I like sand

#pragma once

#include "CoreMinimal.h"

#include "BuildingBase.h"
#include "GameFramework/Actor.h"
#include "Team9Assemble/CharacterController/BeachPlayerController.h"
#include "Team9Assemble/World/BeachGameState.h"

#include "DummyBuilding.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class UBoxComponent;
class U_Building_DataAsset;
class UTileMap;

UCLASS()
class TEAM9ASSEMBLE_API ADummyBuilding : public AActor {
	GENERATED_BODY()

public:
	ADummyBuilding();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	ABuildingBase* TryPlaceDummyBuilding(bool ForceConstruction = false, bool IsVisitorHome = false);

	UFUNCTION(BlueprintImplementableEvent)
	void OnPlacingDummyBuilding(ABuildingBase* SpawnedBuilding);
	
	void CheckIfPlacementIsValid();
	void PlaceBuildingOnTiles(ABuildingBase* SpawnedBuilding, bool IsVisitorHome);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USceneComponent* SceneComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UBoxComponent* BoxCollider;

	// UPROPERTY(BlueprintReadWrite, EditAnywhere)
	// UNavModifierComponent* NavModifierObstacle;
	// UPROPERTY(BlueprintReadWrite, EditAnywhere)
	// UNavModifierComponent* NavModifierPath;
	// UPROPERTY(BlueprintReadWrite, EditAnywhere)
	// UNavModifierComponent* NavModifierNormal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* Building;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* ArrowEnter;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* ArrowExit;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Behaviour")
	U_Building_DataAsset* BuildingData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vision")
	bool ShowLifeGuardVision;


	UPROPERTY()
	AEconomySystem* EconomySystem = nullptr;

	UFUNCTION(BlueprintImplementableEvent)
	void OnConstructionFailed(EConstructionFailType ConstructionFailType);
	
	UFUNCTION(BlueprintCallable)
	void Delete();

	UPROPERTY(BlueprintReadWrite)
	bool CanPlace;
	UPROPERTY(BlueprintReadWrite)
	bool CanPlaceFromRaytrace;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Building To Spawn")
	TSubclassOf<ABuildingBase> BuildingBlueprint;
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	// bool IsCustomBuilding;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int MarginCheckPlacement = 1;	
	TSet<UTile*> CollectedTiles;

	UPROPERTY()
	UTile* StartCheckTile = nullptr;

	UPROPERTY(VisibleAnywhere)
	UTileMap* TileMap = nullptr;

	UPROPERTY(BlueprintReadWrite)
	bool IsRotated;
	UPROPERTY(BlueprintReadWrite)
	bool IsRotating;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interactions")
	TArray<FTransform> InteractPoints;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interactions")
	bool HasInteractionPoints;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	ABeachPlayerController* PlayerController;
};
