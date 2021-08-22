// I like sand
#include "BTTask_AIBeach_InteractWithBuilding.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Team9Assemble/AI/AIBeach_Controller_Base.h"

UBTTask_AIBeach_InteractWithBuilding::UBTTask_AIBeach_InteractWithBuilding() {
	NodeName = "AIBeach Interact With Building";
}
void UBTTask_AIBeach_InteractWithBuilding::OnGameplayTaskActivated(UGameplayTask& Task) {}

EBTNodeResult::Type UBTTask_AIBeach_InteractWithBuilding::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
                                                                      uint8* NodeMemory) {

		AAIBeach_Controller_Base* AIController = Cast<AAIBeach_Controller_Base>(OwnerComp.GetOwner());
		if (AIController->StartInteractWithBuilding() == true) {
			return EBTNodeResult::Succeeded;
		}
	return EBTNodeResult::Failed;
}

void UBTTask_AIBeach_InteractWithBuilding::InitializeFromAsset(UBehaviorTree& Asset) { }

void UBTTask_AIBeach_InteractWithBuilding::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                                                    float DeltaSeconds) {}
