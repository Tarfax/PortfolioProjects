
#include "BTTask_SetAnimation.h"

#include "Team9Assemble/AI/AILifeguard/AILifeGuard_Controller.h"
#include "Team9Assemble/AI/AIVisitor/AIVisitor_Controller.h"


UBTTask_SetAnimation::UBTTask_SetAnimation() {NodeName ="AI Animation"; }

void UBTTask_SetAnimation::OnGameplayTaskActivated(UGameplayTask& Task) {}
EBTNodeResult::Type UBTTask_SetAnimation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	AAIVisitor_Controller* Controller = Cast<AAIVisitor_Controller>(OwnerComp.GetOwner());
	// Controller->SetAnimation(AIVisitor_Animation);
	return EBTNodeResult::Succeeded;
}

void UBTTask_SetAnimation::InitializeFromAsset(UBehaviorTree& Asset) {}

void UBTTask_SetAnimation::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {}
