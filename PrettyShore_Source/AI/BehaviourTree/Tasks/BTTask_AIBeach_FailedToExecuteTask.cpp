#include "BTTask_AIBeach_FailedToExecuteTask.h"

#include "Team9Assemble/AI/AIVisitor/AIVisitor_Controller.h"
#include "Team9Assemble/AI/AILifeguard//AILifeGuard_Controller.h"


UBTTask_AIBeach_FailedToExecuteTask::UBTTask_AIBeach_FailedToExecuteTask() {
	NodeName = "AIBeach Failed To Execute Task";
	} 
void UBTTask_AIBeach_FailedToExecuteTask::OnGameplayTaskActivated(UGameplayTask& Task) {}

EBTNodeResult::Type UBTTask_AIBeach_FailedToExecuteTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory) {

	if (AIType == EAIType::Visitor) {
		AAIVisitor_Controller* Visitor = Cast<AAIVisitor_Controller>(OwnerComp.GetAIOwner());
		Visitor->FailedToExecuteTask(VisitorBehaviourState, LockdownTime);
	} else {
		AAILifeGuard_Controller* LifeGuard = Cast<AAILifeGuard_Controller>(OwnerComp.GetAIOwner());
		LifeGuard->FailedToExecuteTask(LifeGuardBehaviourState, LockdownTime);
	}
	//
	// AAIBeach_Controller_Base* AIBeach = Cast<AAIBeach_Controller_Base>(OwnerComp.GetAIOwner());
	//
	// AIBeach->FailedToExecuteTask();

	return EBTNodeResult::Failed;
}

void UBTTask_AIBeach_FailedToExecuteTask::InitializeFromAsset(UBehaviorTree& Asset) {}

void UBTTask_AIBeach_FailedToExecuteTask::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	float DeltaSeconds) {}
