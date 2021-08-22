// I like sand


#include "BTTask_AIVisitor_FindBeachLocation.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Team9Assemble/AI/Models/AI_BeachArea.h"
#include "Team9Assemble/AI/AIVisitor/AIVisitor_Controller.h"
#include "Team9Assemble/TileSystem/Tile.h"
#include "Team9Assemble/TileSystem/TileMap.h"
#include "Team9Assemble/World/BeachGameState.h"

UBTTask_AIVisitor_FindBeachLocation::UBTTask_AIVisitor_FindBeachLocation() {
	NodeName = "AIVisitor Find Beach Location";
}


void UBTTask_AIVisitor_FindBeachLocation::OnGameplayTaskActivated(UGameplayTask& Task) {}

EBTNodeResult::Type UBTTask_AIVisitor_FindBeachLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
                                                                     uint8* NodeMemory) {
	AAIVisitor_Controller* Visitor = Cast<AAIVisitor_Controller>(OwnerComp.GetAIOwner());


	if (Visitor != nullptr) {

		ABeachGameState* GameState = Cast<ABeachGameState>(GetWorld()->GetGameState());
		FVector RandomPosition = GameState->BeachArea->FindRandomLocation();

		UTile* Tile = GameState->TileMap->GetTileFromWorldPosition(RandomPosition);
		if (Tile != nullptr && Tile->CanReservAsHome() == true) {
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(TileKey.SelectedKeyName, Tile);
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(
				DestinationKey.SelectedKeyName, FVector(Tile->X, Tile->Y, 0.0f));
			return EBTNodeResult::Succeeded;
		}

		UE_LOG(LogTemp, Warning, TEXT("Can reserv home == false"));
	}

	return EBTNodeResult::Failed;
}

void UBTTask_AIVisitor_FindBeachLocation::InitializeFromAsset(UBehaviorTree& Asset) {
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BBAsset = Asset.BlackboardAsset;
	if (ensure(BBAsset) == true) {
		DestinationKey.ResolveSelectedKey(*BBAsset);
		TileKey.ResolveSelectedKey(*BBAsset);
	}

}

void UBTTask_AIVisitor_FindBeachLocation::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                                                   float DeltaSeconds) {}
