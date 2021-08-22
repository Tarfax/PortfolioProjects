// I like sand

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Team9Assemble/AI/AILifeguard/AILifeGuard.h"
#include "Team9Assemble/AI/AIVisitor/AIVisitor.h"
#include "BTTask_Helper.h"

#include "BTTask_AIBeach_ChangeInternalState.generated.h"

UCLASS()
class UBTTask_AIBeach_ChangeInternalState : public UBTTaskNode {
public:
	GENERATED_BODY()
	UBTTask_AIBeach_ChangeInternalState();

	void OnGameplayTaskActivated(UGameplayTask& Task) override;
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void InitializeFromAsset(UBehaviorTree& Asset) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	

	UPROPERTY(EditAnywhere, Category = AIType)
	EAIType AIType = EAIType::Visitor;
	UPROPERTY(EditAnywhere, Category=Blackboard)
	FBlackboardKeySelector InternalStateKey;

	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (EditCondition = "AIType == EAIType::Visitor"))
	E_AIVisitor_TaskType VisitorTask;
	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (EditCondition = "AIType == EAIType::LifeGuard"))
	E_AILifeGuard_TaskType LifeGuardTask;
};

