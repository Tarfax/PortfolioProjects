// I like sand

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildingData.h"
#include "Team9Assemble/AI/Interfaces/AI_Workable.h"
#include "Team9Assemble/World/BeachGameState.h"
#include "BuildingBase.generated.h"

class UBuilding_Observer;
class UBeachWork;
class UBoxComponent;
class USceneComponent;
class UStaticMeshComponent;
class UNavModifierComponent;
class UClickableObject;
class UBuildingBehaviour;
class AStaticMeshActor;
class U_Building_DataAsset;
class AAIBeach_Controller_Base;
class UTile;
class AAIVisitor_Controller;
class AEconomySystem;
class ARatingSystem;

UCLASS()
class TEAM9ASSEMBLE_API ABuildingBase : public AActor, public IAI_Workable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildingBase();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void FinishWork(UBeachWork* BeachWork) override; 
	
	UFUNCTION(BlueprintCallable, Category = "Building")
	void ConstructBuilding();
	UFUNCTION(BlueprintImplementableEvent)
	void OnConstructBuilding();
	UFUNCTION(BlueprintCallable, Category = "Building")
	void DeconstructBuilding();
	UFUNCTION(BlueprintImplementableEvent)
	void OnDeconstructBuilding();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnLeavingHome();

	EAI_CustomAnimations GetInteractionAnimation(EAIBuildingInteractionType WantedAnimationType);

	// UFUNCTION(BlueprintCallable, Category="Building")
	// void SetupNavmesh();
	
	void CreateWork_CleanBuilding();
	void FinishWork_CleanBuilding();
	void CreateWork_ReceiveStock();
	void FinishWork_ReceiveStock(int Quantity);
	void CreateWork_RemoveTowel();
	void FinishWork_RemoveTowel();

	UFUNCTION(BlueprintImplementableEvent)
	void OnStockNeedsRefill();
	UFUNCTION(BlueprintImplementableEvent)
	void OnStockIsRefilled();

	UFUNCTION(BlueprintImplementableEvent)
	void OnBuildingNeedsCleaning();
	UFUNCTION(BlueprintImplementableEvent)
	void OnBuildingIsCleaned();

	UFUNCTION(BlueprintImplementableEvent)
	void OnTowelNeedsRemoval();
	UFUNCTION(BlueprintImplementableEvent)
	void OnTowelIsRemoved();

	
	// Functionality
	FTransform GetAvailableInteractionTransform(AAIBeach_Controller_Base* Visitor);
	FTransform GetExitTransform() const;

	UFUNCTION(BlueprintCallable)
	bool Enter(AAIBeach_Controller_Base* Controller);
	void EnterHome(AAIVisitor_Controller* Controller);
	UFUNCTION(BlueprintImplementableEvent)
	void OnEnterBuilding();
	bool AnnounceIncoming(AAIBeach_Controller_Base* Controller);
	void CancelIncoming(AAIBeach_Controller_Base* Controller);
	UFUNCTION(BlueprintCallable)
	void Leave(AAIBeach_Controller_Base* Controller);
	
	void LeaveHome(AAIVisitor_Controller* Controller);
	UFUNCTION(BlueprintImplementableEvent)
	void OnLeavingBuilding();
	FBuildingMoodModifier GetBuildingMoodModifier() const;
	void ReceivePayment(const int32 Payment);
	void TakeMoneyFromPlayerBank(const int Amount);
	
	UFUNCTION()
	void EndOfDayUpdate(AEconomySystem* EcoSystem);

	UFUNCTION()
	void NotificationUpdate(AEconomySystem* EcoSystem);

	void BindToEvents();
	void RemoveEvents();

public: //Variables
	UPROPERTY(VisibleAnywhere)
	UBuilding_Observer* BuildingObserver;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USceneComponent* SceneComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UBoxComponent* PlacementColliderComp;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* Building;

	// UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	// UStaticMeshComponent* OutlineMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) 
	UStaticMeshComponent* ArrowEnter;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) 
	UStaticMeshComponent* ArrowExit;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UNavModifierComponent* NavModifierObstacle;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UNavModifierComponent* NavModifierPath;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UNavModifierComponent* NavModifierNormal;
	UPROPERTY(EditAnywhere)
	UClickableObject* ClickableComp;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Building", meta = (ExposeOnSpawn = "true"))
	U_Building_DataAsset* BuildingData;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Building")
	TArray<AAIBeach_Controller_Base*> Users;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere,Category = "Building")
	TArray<AAIBeach_Controller_Base*> ReservedUsers;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Building")
	bool IsFull;
	
	UPROPERTY(VisibleAnywhere,Category = "Building")
	TArray<UTile*> Tiles;

	UPROPERTY(EditAnywhere,Category = "Building")
	bool ShowOccupiedTiles;
	float Timer;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool IsSelected;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vision")
	bool ShowLifeGuardVision;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool IsMarkedForDeconstruction;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	AAIVisitor_Controller* AIVisitor_Owner;
	

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<FTransform> InteractionTransforms;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TMap<int, bool> UsedInteractions;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TMap<AAIBeach_Controller_Base*, int> VisitorInteractionIndex;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Building Interaction")
	TArray<F_BuildingInteractionData> InteractionData;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Building Inventory")
	F_BuildingInventory Inventory;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Building Info")
	int Uses = 0;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Building Info")
	bool NeedsCleaning = false;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Building Info")
	int TotalVisits = 0;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Building Info")
	bool NeedRestock = false;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Beach Work")
	TArray<UBeachWork*> BeachWorks;

	UPROPERTY(VisibleAnywhere, Category="Building References")
	ARatingSystem* RatingSystem;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Economy")
	int TotalEarnings;

	UPROPERTY()
	AEconomySystem* EconomySystem;

	UPROPERTY(BlueprintReadWrite, Category="Building Color")
	bool UsesCustomColor = false;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Building Color")
	bool CanBeColord = false;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category="Building Color")
	FLinearColor BuildingColor;
private:
	
	int32 TotalBuildingBank{0};
	FTimerHandle WaitForReferenceHandle;
};
