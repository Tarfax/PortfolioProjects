#include "BTTask_AILifeGuard_SetMoveSpeed.h"

#include "Team9Assemble/AI/AIBeach_Controller_Base.h"

UBTTask_AILifeGuard_SetMoveSpeed::UBTTask_AILifeGuard_SetMoveSpeed() {
	NodeName ="AILifeGuard Set Move Speed";
}
void UBTTask_AILifeGuard_SetMoveSpeed::OnGameplayTaskActivated(UGameplayTask& Task) {}

EBTNodeResult::Type UBTTask_AILifeGuard_SetMoveSpeed::
ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	AAIBeach_Controller_Base* AIController = Cast<AAIBeach_Controller_Base>(OwnerComp.GetOwner());
	AIController->SetNewMoveSpeed(NewMoveSpeed);
	return  EBTNodeResult::Succeeded;
}

void UBTTask_AILifeGuard_SetMoveSpeed::InitializeFromAsset(UBehaviorTree& Asset) {}

void UBTTask_AILifeGuard_SetMoveSpeed::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	float DeltaSeconds) {}
