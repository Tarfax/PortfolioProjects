// I like sand

#include "BuildingBase.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Team9Assemble/ClickableObject.h"
// #include "Team9Assemble/AI/AIBeach_Controller_Base.h"
#include "Building_Observer.h"
#include "Team9Assemble/TileSystem/Tile.h"
#include "Team9Assemble/AI/AIVisitor/AIVisitor_Controller.h"
#include "Team9Assemble/World/BeachGameState.h"
#include "Team9Assemble/World/EconomySystem.h"
#include "Team9Assemble/World/RatingSystem.h"

// Sets default values
ABuildingBase::ABuildingBase() {
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = SceneComponent;

	PlacementColliderComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Colldier"));
	PlacementColliderComp->SetupAttachment(SceneComponent);

	Building = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Building"));
	Building->SetCollisionProfileName(TEXT("NoCollision"));
	Building->SetupAttachment(SceneComponent);


	ArrowEnter = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Enter"));
	ArrowEnter->SetCollisionProfileName(TEXT("NoCollision"));
	ArrowEnter->SetupAttachment(SceneComponent);

	ArrowExit = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Exit"));
	ArrowExit->SetCollisionProfileName(TEXT("NoCollision"));
	ArrowExit->SetupAttachment(SceneComponent);

	ClickableComp = CreateDefaultSubobject<UClickableObject>(TEXT("ClickableObject"));
}

void ABuildingBase::BeginPlay() {
	ABeachGameState* GameState = Cast<ABeachGameState>(GetWorld()->GetGameState());
	BuildingObserver = GameState->BuildingObserver;
	RatingSystem = GameState->RatingSystem;
	Timer = 0.0f;
	Super::BeginPlay();
}

void ABuildingBase::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	if (ShowOccupiedTiles == true) {
		Timer -= DeltaTime;
		if (Timer <= 0.0f) {

			for (int i = 0; i < Tiles.Num(); i++) {
				if (Tiles[i] != nullptr) {
					Tiles[i]->DrawDebug();
				}

			}
			Timer = 10.0f;
		}
	}

	if ((IsSelected == true && BuildingData->BuildingType == E_BuildingType::WatchTower) || ShowLifeGuardVision ==
		true) {
		const FVector Direction = FVector::ForwardVector;
		FVector Origin = GetActorLocation(); // + (FVector::UpVector * 100);
		Origin.Z = 20.0f;

		const FVector Right = Direction.RotateAngleAxis(35, FVector::UpVector);
		const FVector Left = Direction.RotateAngleAxis(-35, FVector::UpVector);

		UKismetSystemLibrary::DrawDebugLine(GetWorld(), Origin, Origin + Right * BuildingData->WatchTowerVisionRange,
		                                    FLinearColor::Green, 0.15f, 15);
		UKismetSystemLibrary::DrawDebugLine(GetWorld(), Origin, Origin + Left * BuildingData->WatchTowerVisionRange,
		                                    FLinearColor::Green, 0.15f, 15);
		UKismetSystemLibrary::DrawDebugLine(GetWorld(), Origin + Right * BuildingData->WatchTowerVisionRange,
		                                    Origin + Left * BuildingData->WatchTowerVisionRange, FLinearColor::Green,
		                                    0.15f, 15);
	}
}


void ABuildingBase::ConstructBuilding() {
	ABeachGameState* GameState = Cast<ABeachGameState>(GetWorld()->GetGameState());
	if (GameState->EconomySystem != nullptr) {
		EconomySystem = GameState->EconomySystem;
		BindToEvents();
	}
	else {
		GetWorld()->GetTimerManager().SetTimer(WaitForReferenceHandle, this, &ABuildingBase::BindToEvents, .2f, false);
		UE_LOG(LogTemp, Log, TEXT("BuildingBase: Reference handle!"))
	}

	BuildingObserver->AddBuilding(this);

	for (int i = 0; i < InteractionTransforms.Num(); i++) {
		UsedInteractions.Add(i, false);
	}
	Inventory = BuildingData->Inventory;
	OnConstructBuilding();
}

void ABuildingBase::DeconstructBuilding() {
	IsMarkedForDeconstruction = true;
	UE_LOG(LogTemp, Log, TEXT("ABuildingBase::DeconstructBuilding"));
	for (auto User : Users) {
		User->StopInteractWithBuilding();
	}

	if (AIVisitor_Owner != nullptr) {
		AIVisitor_Owner->HomeWasDestroyed();
	}

	for (int i = 0; i < Tiles.Num(); i++) {
		if (Tiles[i] != nullptr) {
			Tiles[i]->RemoveBuilding();
		}
	}

	if (BeachWorks.Num() > 0) {
		for (int i = 0; i < BeachWorks.Num(); i++) {
			if (BeachWorks[i]->WorkType == EBeachWorkType::CleanBuilding) {
				BuildingObserver->RemoveNeedsCleaning(BeachWorks[i]);
			}
			else if (BeachWorks[i]->WorkType == EBeachWorkType::LeaveRestock) {
				BuildingObserver->RemoveNeedsStocking(BeachWorks[i]);
			}
			else {
				BuildingObserver->RemoveNeedsHomeRemoval(BeachWorks[i]);
			}
		}
	}

	RemoveEvents();
	BuildingObserver->RemoveBuilding(this);
	OnDeconstructBuilding();
	Destroy();
	//Handle other stuff?
}

bool ABuildingBase::Enter(AAIBeach_Controller_Base* Controller) {
	ReservedUsers.Remove(Controller);

	if (Inventory.AvailableStock > 0 || Inventory.UnlimitedStock == true) {
		if ((Users.Num() + ReservedUsers.Num()) < BuildingData->MaxUsers || BuildingData->MaxUsers == -1) {
			Users.Add(Controller);

			if (BuildingData->MaxUsers != -1 && Users.Num() + ReservedUsers.Num() == BuildingData->MaxUsers) {
				IsFull = true;
			}

			if (Inventory.UnlimitedStock == false) {
				Inventory.AvailableStock--;
				if (Inventory.AvailableStock <= BuildingData->MaxUsers && NeedRestock == false) {
					CreateWork_ReceiveStock();
				}
			}
			OnEnterBuilding();
			return true;
		}
	}
	
	for (int i = 0; i < InteractionData.Num(); i++) {
		if (InteractionData[i].AIController == Controller) {
			InteractionData[i].AIController = nullptr;
			InteractionData[i].IsOccupied = false;
		}
	}
	return false;
}

void ABuildingBase::EnterHome(AAIVisitor_Controller* Controller) {
	AIVisitor_Owner = Controller;
	ReservedUsers.Remove(Controller);
}


void ABuildingBase::Leave(AAIBeach_Controller_Base* Controller) {
	Uses++;
	TotalVisits++;
	if (ReservedUsers.Remove(Controller) == 0) {
		// UE_LOG(LogTemp, Log, TEXT("UBuildingBehaviour::RemoveUser:: Did not remove user from Reserved!!"));
	}
	if (Users.Remove(Controller) == 0) {
		// UE_LOG(LogTemp, Log, TEXT("UBuildingBehaviour::RemoveUser:: Did not remove user from Users!!"));
	}

	IsFull = false;

	if (BuildingData->BuildingType == E_BuildingType::VisitorHome) {
		return;
	}

	ReceivePayment(BuildingData->Inventory.Price);
	TotalEarnings += BuildingData->Inventory.Price;
	//EconomySystem->AddToIncome(BuildingData->Inventory.Price);
	RatingSystem->AddEarnings(Inventory.Price);

	for (int i = 0; i < InteractionData.Num(); i++) {
		if (InteractionData[i].AIController == Controller) {
			InteractionData[i].AIController = nullptr;
			InteractionData[i].IsOccupied = false;
		}
	}


	if (BuildingData->WillRequireCleaning == true && Uses >= BuildingData->UsesUntilCleaning) {
		if (NeedsCleaning == false) {
			CreateWork_CleanBuilding();
		}
	}

	OnLeavingBuilding();
}

void ABuildingBase::LeaveHome(AAIVisitor_Controller* Controller) {
	if (BuildingData->Inventory.Price > 0) {
		CreateWork_CleanBuilding();
	}
	else {
		CreateWork_RemoveTowel();
	}
	Uses++;
	TotalVisits++;
	//EconomySystem->AddToIncome(BuildingData->Inventory.Price);
	ReceivePayment(BuildingData->Inventory.Price);
	TotalEarnings += BuildingData->Inventory.Price;
	ReservedUsers.Remove(Controller);
	Users.Remove(Controller);
	IsFull = false;
	OnLeavingHome();
}

FTransform ABuildingBase::GetAvailableInteractionTransform(AAIBeach_Controller_Base* Visitor) {
	for (int i = 0; i < InteractionData.Num(); i++) {
		if (InteractionData[i].IsOccupied == false) {
			InteractionData[i].IsOccupied = true;
			InteractionData[i].AIController = Visitor;
			return InteractionData[i].InteractionTransform;
		}
	}
	UE_LOG(LogTemp, Log, TEXT("GetAvailableInteractionTransform"));
	UE_LOG(LogTemp, Log, TEXT("NUM? %i"), InteractionData.Num());
	
	return ArrowEnter->GetComponentTransform();
}

FTransform ABuildingBase::GetExitTransform() const {
	return ArrowExit->GetComponentTransform();
}

bool ABuildingBase::AnnounceIncoming(AAIBeach_Controller_Base* Controller) {
	if ((Users.Num() + ReservedUsers.Num()) < BuildingData->MaxUsers) {
		ReservedUsers.Add(Controller);
		if (Users.Num() + ReservedUsers.Num() == BuildingData->MaxUsers) {
			IsFull = true;
		}
		return true;
	}

	if (BuildingData->MaxUsers < 0) {
		return true;
	}

	return false;
}

void ABuildingBase::CancelIncoming(AAIBeach_Controller_Base* Controller) {
	if (ReservedUsers.Remove(Controller) != 0) {
		IsFull = false;
	}

	if (VisitorInteractionIndex.Contains(Controller) == true) {
		UsedInteractions[VisitorInteractionIndex[Controller]] = false;
		VisitorInteractionIndex.Remove(Controller);
		UE_LOG(LogTemp, Log, TEXT("UBuildingBehaviour::DID remove interaction index BUT FROM CancelIncoming"));
	}
}


FBuildingMoodModifier ABuildingBase::GetBuildingMoodModifier() const {
	return BuildingData->BuildingMoodModifier;
}

void ABuildingBase::FinishWork(UBeachWork* BeachWork) {
	if (BeachWork != nullptr) {
		if (BeachWork->WorkType == EBeachWorkType::LeaveRestock) {
			BuildingObserver->RemoveNeedsStocking(BeachWork);
			FinishWork_ReceiveStock(Inventory.MaxStock - Inventory.AvailableStock);
		}
		else if (BeachWork->WorkType == EBeachWorkType::CleanBuilding) {
			BuildingObserver->RemoveNeedsCleaning(BeachWork);
			FinishWork_CleanBuilding();
		}
		else {
			BuildingObserver->RemoveNeedsHomeRemoval(BeachWork);
			FinishWork_RemoveTowel();
		}

		BeachWorks.Remove(BeachWork);

		BeachWork->ConditionalBeginDestroy();
	}
}

EAI_CustomAnimations ABuildingBase::GetInteractionAnimation(EAIBuildingInteractionType WantedAnimationType) {
	switch (WantedAnimationType) {
	case EAIBuildingInteractionType::Waiting:
		return BuildingData->WaitingAnimation;
	case EAIBuildingInteractionType::Interacting:
	default:
		return BuildingData->InteractAnimation;
	}
}

void ABuildingBase::CreateWork_CleanBuilding() {
	NeedsCleaning = true;
	OnBuildingNeedsCleaning();

	UBeachWork* BeachWork = NewObject<UBeachWork>(this);
	BeachWork->Timer = 5;
	BeachWork->WorkLocation = GetTransform();
	BeachWork->BeachWorkObject = this;
	BeachWork->WorkType = EBeachWorkType::CleanBuilding;

	BuildingObserver->AddNeedsCleaning(BeachWork);
	BeachWorks.Add(BeachWork);
}

void ABuildingBase::CreateWork_ReceiveStock() {
	NeedRestock = true;
	OnStockNeedsRefill();

	UBeachWork* BeachWork = NewObject<UBeachWork>(this);
	BeachWork->Timer = 5;
	BeachWork->WorkLocation = GetTransform();
	BeachWork->BeachWorkObject = this;
	BeachWork->WorkType = EBeachWorkType::LeaveRestock;

	BuildingObserver->AddNeedsStocking(BeachWork);
	BeachWorks.Add(BeachWork);
}

void ABuildingBase::CreateWork_RemoveTowel() {
	if (BuildingData->Inventory.Price == 0) {
		OnTowelNeedsRemoval();

		UBeachWork* BeachWork = NewObject<UBeachWork>(this);
		BeachWork->Timer = 5;
		BeachWork->WorkLocation = GetTransform();
		BeachWork->BeachWorkObject = this;
		BeachWork->WorkType = EBeachWorkType::RemoveTowel;

		BuildingObserver->AddNeedsHomeRemoval(BeachWork);
		BeachWorks.Add(BeachWork);
	}
	// else {
	// 	CreateWork_CleanBuilding();
	// }
}

void ABuildingBase::FinishWork_CleanBuilding() {
	NeedsCleaning = false;
	OnBuildingIsCleaned();

	Uses = 0;
}

void ABuildingBase::FinishWork_RemoveTowel() {
	OnTowelIsRemoved();
	DeconstructBuilding();
}

void ABuildingBase::FinishWork_ReceiveStock(int Quantity) {
	NeedRestock = false;
	OnStockIsRefilled();

	Inventory.AvailableStock += Quantity;
	int TotalCost = Inventory.PricePerInventoryStock * Quantity;
	TakeMoneyFromPlayerBank(TotalCost);
}

void ABuildingBase::ReceivePayment(const int32 Payment) {
	auto* BeachState = Cast<ABeachGameState>(GetWorld()->GetGameState());
	if (BeachState != nullptr)
		BeachState->EconomySystem->AddToIncome(Payment, this);
	else
		UE_LOG(LogTemp, Log, TEXT("ABuildingBase:ReceivePayment; GameState is not <ABeachGameState>!"))
}

void ABuildingBase::TakeMoneyFromPlayerBank(const int Amount)
{
	auto* BeachState = Cast<ABeachGameState>(GetWorld()->GetGameState());
	if (BeachState != nullptr)
	{
		BeachState->EconomySystem->AddToExpenses(Amount, this);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("ABuildingBase:TakeMoneyFromPlayerBank; GameState is not <ABeachGameState>!"))
	}
}

void ABuildingBase::EndOfDayUpdate(AEconomySystem* EcoSystem) {
	if (EconomySystem != nullptr && BuildingData != nullptr) {
		EconomySystem->AddToUpkeep(BuildingData->Inventory.UpkeepCost);
	}
}

void ABuildingBase::NotificationUpdate(AEconomySystem* EcoSystem) {
	//EconomySystem->ReceiveEconomicUpdate(TotalBuildingBank);
	//TotalBuildingBank = 0;
}

void ABuildingBase::BindToEvents() {
	if (BuildingData->BuildingType == E_BuildingType::VisitorHome && BuildingData->CostToBuy == 0) {
		return;
	}

	ABeachGameState* GameState = Cast<ABeachGameState>(GetWorld()->GetGameState());
	if (GameState->EconomySystem != nullptr) {
		GameState->EconomySystem->OnEndOfDayUpdate.AddDynamic(this, &ABuildingBase::EndOfDayUpdate);
		//GameState->EconomySystem->OnNotificationUpdate.AddDynamic(this, &ABuildingBase::NotificationUpdate);
		UE_LOG(LogTemp, Log, TEXT("BuildingBase: Bound to events in EconomySystem after a delay! <---"))
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("BuildingBase; BindToEvents: EconomySystem is still null! <---"))
	}
}

void ABuildingBase::RemoveEvents() {
	if (BuildingData->BuildingType == E_BuildingType::VisitorHome && BuildingData->CostToBuy == 0) {
		return;
	}

	ABeachGameState* GameState = Cast<ABeachGameState>(GetWorld()->GetGameState());
	if (GameState->EconomySystem != nullptr) {
		GameState->EconomySystem->OnEndOfDayUpdate.RemoveDynamic(this, &ABuildingBase::EndOfDayUpdate);
		//GameState->EconomySystem->OnNotificationUpdate.RemoveDynamic(this, &ABuildingBase::NotificationUpdate);

		UE_LOG(LogTemp, Log, TEXT("BuildingBase: Removed events in EconomySystem! <---"))
	}
}
