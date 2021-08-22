// I like sand


#include "BTTask_AIVisitor_FindRandomSwimLocation.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Team9Assemble/AI/AIVisitor/AIVisitor_Controller.h"
#include "Team9Assemble/AI/Models/AI_WaterArea.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Team9Assemble/World/BeachGameState.h"

UBTTask_AIVisitor_FindRandomSwimLocation::UBTTask_AIVisitor_FindRandomSwimLocation() {
	NodeName = "AIVisitor Find Swim Location";
}

void UBTTask_AIVisitor_FindRandomSwimLocation::OnGameplayTaskActivated(UGameplayTask& Task) {}

EBTNodeResult::Type UBTTask_AIVisitor_FindRandomSwimLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory) {

	ABeachGameState* GameState = Cast<ABeachGameState>(GetWorld()->GetGameState());
	//
	AAIVisitor_Controller* Visitor = Cast<AAIVisitor_Controller>(OwnerComp.GetAIOwner());
	// FVector RandomPosition = Visitor->WaterArea->FindRandomLocation();

	FVector RandomPosition = GameState->WaterArea->FindRandomLocation();
	if (IsGoingToSwim == true) {
		float YHomePosition = OwnerComp.GetBlackboardComponent()->GetValueAsVector( HomePositionKey.SelectedKeyName).Y;
		RandomPosition.Y = FMath::RandRange(YHomePosition - 100, YHomePosition + 100);
		Visitor->BeginTransitionIntoWater();
	}
	
	OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(DestinationKey.SelectedKeyName, RandomPosition);

	return EBTNodeResult::Succeeded;
}

void UBTTask_AIVisitor_FindRandomSwimLocation::InitializeFromAsset(UBehaviorTree& Asset) {
	UBlackboardData* BBAsset = Asset.BlackboardAsset;
	if (ensure(BBAsset) == true) {
		HomePositionKey.ResolveSelectedKey(*BBAsset);
	}
}

void UBTTask_AIVisitor_FindRandomSwimLocation::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	float DeltaSeconds) {}
