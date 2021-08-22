#include "BTTask_AIVisitor_GoHome.h"

#include "Kismet/GameplayStatics.h"
#include "Team9Assemble/AI/AIVisitor/AIVisitor_Controller.h"
#include "Team9Assemble/World/BeachGameInstance.h"


UBTTask_AIVisitor_GoHome::UBTTask_AIVisitor_GoHome() {}

void UBTTask_AIVisitor_GoHome::OnGameplayTaskActivated(UGameplayTask& Task) {}

EBTNodeResult::Type UBTTask_AIVisitor_GoHome::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	
	AAIVisitor_Controller* Visitor = Cast<AAIVisitor_Controller>(OwnerComp.GetAIOwner());


	if (Visitor != nullptr) {
		Visitor->DestroyVisitor();
	}
	
	return EBTNodeResult::Succeeded;
}

void UBTTask_AIVisitor_GoHome::InitializeFromAsset(UBehaviorTree& Asset) {}

void UBTTask_AIVisitor_GoHome::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {}
