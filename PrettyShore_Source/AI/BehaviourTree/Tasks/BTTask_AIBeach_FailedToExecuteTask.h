#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Team9Assemble/AI/AILifeguard/AILifeGuard.h"
#include "Team9Assemble/AI/AIVisitor/AIVisitor.h"
#include "BTTask_Helper.h"
#include"BTTask_AIBeach_FailedToExecuteTask.generated.h"



UCLASS()
class UBTTask_AIBeach_FailedToExecuteTask : public UBTTaskNode {
	GENERATED_BODY()
public:

	UBTTask_AIBeach_FailedToExecuteTask();
	void OnGameplayTaskActivated(UGameplayTask& Task) override;
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void InitializeFromAsset(UBehaviorTree& Asset) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = AIType)
	EAIType AIType = EAIType::Visitor;

	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (EditCondition = "AIType == EAIType::Visitor", DisplayName = "Task That Failed"))
	E_AIVisitor_TaskType VisitorBehaviourState;
	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (EditCondition = "AIType == EAIType::LifeGuard"), DisplayName = "Task That Failed")
	E_AILifeGuard_TaskType LifeGuardBehaviourState;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	float LockdownTime = 3.0;
};
