// I like sand


#include "BTTask_AILifeGuard_FindRandomBeachLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Team9Assemble/AI/Models/AI_BeachArea.h"
#include "Team9Assemble/AI/AILifeGuard/AILifeGuard_Controller.h"
#include "Team9Assemble/World/BeachGameState.h"

UBTTask_AILifeGuard_FindRandomBeachLocation::UBTTask_AILifeGuard_FindRandomBeachLocation() {
	NodeName = "AILifeGuard Find Random Beach Location";
}


void UBTTask_AILifeGuard_FindRandomBeachLocation::OnGameplayTaskActivated(UGameplayTask& Task) {}

EBTNodeResult::Type UBTTask_AILifeGuard_FindRandomBeachLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	// AAILifeGuard_Controller* LifeGuard = Cast<AAILifeGuard_Controller>(OwnerComp.GetAIOwner());
	ABeachGameState* GameState = Cast<ABeachGameState>(GetWorld()->GetGameState());
	
	FVector RandomPosition = GameState->BeachArea->FindRandomLocation();
	OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(DestinationKey.SelectedKeyName, RandomPosition);
	
	return EBTNodeResult::Succeeded;
}

void UBTTask_AILifeGuard_FindRandomBeachLocation::InitializeFromAsset(UBehaviorTree& Asset) {
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset) == true) {
		DestinationKey.ResolveSelectedKey(*BBAsset);
	}
	
}
void UBTTask_AILifeGuard_FindRandomBeachLocation::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {}
