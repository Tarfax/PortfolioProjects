// I like sand


#include "BeachGameState.h"

#include "RatingSystem.h"
#include "World_Light.h"
#include "World_LightInterpolation.h"
#include "Team9Assemble/Buildings/Building_Observer.h"
#include "Team9Assemble/TileSystem/TileMap.h"
#include "Team9Assemble/AI/AIBeach_Observer.h"
#include "Team9Assemble/AI/Models/AI_BeachArea.h"
#include "Team9Assemble/AI/Models/AI_WaterArea.h"
#include "Kismet/GameplayStatics.h"
#include "Team9Assemble/World/EconomySystem.h"
#include "Team9Assemble/World/World_Time.h"

ABeachGameState::ABeachGameState() {

	// BuildingObserver->SetupAttachment(RootComponent);
}

void ABeachGameState::BeginPlay() {
	BuildingObserver->GameState = this;

	// BuildingObserver = Cast<UBuilding_Observer>(UGameplayStatics::GetActorOfClass(GetWorld(), UBuilding_Observer::StaticClass()));
	BeachArea = Cast<AAI_BeachArea>(UGameplayStatics::GetActorOfClass(GetWorld(), AAI_BeachArea::StaticClass()));
	WaterArea = Cast<AAI_WaterArea>(UGameplayStatics::GetActorOfClass(GetWorld(), AAI_WaterArea::StaticClass()));
	RatingSystem = Cast<ARatingSystem>(UGameplayStatics::GetActorOfClass(GetWorld(), ARatingSystem::StaticClass()));
    // WorldLight = Cast<AWorld_Light>(UGameplayStatics::GetActorOfClass(GetWorld(), AWorld_Light::StaticClass()));
	WorldLightInterpolation = Cast<AWorld_LightInterpolation>(UGameplayStatics::GetActorOfClass(GetWorld(), AWorld_LightInterpolation::StaticClass()));
	// const FActorSpawnParameters SpawnInfo;
	// UE_LOG(LogTemp, Log, TEXT("GameInstance spawning World_Time..."))
	// WorldTime = GetWorld()->SpawnActor<AWorld_Time>(SpawnInfo);
	// UE_LOG(LogTemp, Log, TEXT("GameInstance spawning EconomySystem..."))
	// EconomySystem = GetWorld()->SpawnActor<AEconomySystem>(SpawnInfo);
	// BuildingObserver = CreateDefaultSubobject<UBuilding_Observer>(TEXT("Building Observer"));

	// EconomySystem = GetWorld()->SpawnActor<AEconomySystem>();
	// WorldTime = GetWorld()->SpawnActor<AWorld_Time>();

	if (WorldTime != nullptr) {
		// WorldLight->WorldTime = WorldTime;
		WorldLightInterpolation->WorldTime = WorldTime;
		UE_LOG(LogTemp, Warning, TEXT("World Time Should be set now"));

	} else {
		UE_LOG(LogTemp, Warning, TEXT("World Time is nullptr"));
	}

	Super::BeginPlay();

}

void ABeachGameState::HandleBeginPlay() {
	BuildingObserver = NewObject<UBuilding_Observer>(this, BuildingObserver_Class);
	AIObserver = NewObject<UAIBeach_Observer>(this, AIObserver_Class);
	TileMap = NewObject<UTileMap>(this, TileMap_Class);
	TrashObserver = NewObject<UAITrash_Observer>(this, TrashObserver_Class);

	Super::HandleBeginPlay();
}

void ABeachGameState::SetWorldTime(AWorld_Time* worldTime) {
	if (worldTime != nullptr) {
		WorldTime = worldTime;
		UE_LOG(LogTemp, Log, TEXT("BeachGameState::SetWorldTime; WorldTime has been set. <---"))
		CheckReferenceSetupComplete();
	}
	else
		UE_LOG(LogTemp, Log, TEXT("BeachGameState::SetWorldTime; WorldTime was null. <---"))
}
//
// void ABeachGameState::SetWorldLight(AWorld_Light* worldLight) {
// 	if (worldLight != nullptr) {
// 		WorldLight = worldLight;
// 		UE_LOG(LogTemp, Log, TEXT("BeachGameState::SetWorldLight; WorldLight has been set. <---"))
// 		CheckReferenceSetupComplete();
// 	}
// 	else
// 		UE_LOG(LogTemp, Log, TEXT("BeachGameState::SetWorldLight; WorldLight was null. <---"))
// }

void ABeachGameState::SetEconomySystem(AEconomySystem* economySystem) {
	if (economySystem != nullptr) {
		EconomySystem = economySystem;
		UE_LOG(LogTemp, Log, TEXT("BeachGameState::SetEconomySystem; EcoSys has been set. <---"))
		CheckReferenceSetupComplete();
	}
	else
		UE_LOG(LogTemp, Log, TEXT("BeachGameState::SetEconomySystem; EcoSys was null. <---"))
}

void ABeachGameState::CheckReferenceSetupComplete() {
	if (EconomySystem != nullptr && WorldTime != nullptr/* && WorldLight != nullptr*/) {
		UE_LOG(LogTemp, Log, TEXT("BeachGameState: CheckSetupComplete has passed. <---"))

		// now it should be safe to bind to events and use references from GameState.
		DoCompleteSetupForMembers();
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("BeachGameState: CheckSetupComplete hasn't meet the requirements yet. <---"))
	}
}

void ABeachGameState::DoCompleteSetupForMembers() {
	EconomySystem->CompleteSetup();
	// WorldLight->CompleteSetup(WorldTime);
}

void ABeachGameState::SendLogMessage(FString Message, FLinearColor Color, AActor* Sender) {
	OnLogUpdate.Broadcast(Message, Color, Sender);
}
