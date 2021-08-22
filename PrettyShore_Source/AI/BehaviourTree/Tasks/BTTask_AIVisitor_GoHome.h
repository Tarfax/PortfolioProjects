#pragma once
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AIVisitor_GoHome.generated.h"


UCLASS()
class UBTTask_AIVisitor_GoHome   : public UBTTaskNode {
	GENERATED_BODY()
public:
	UBTTask_AIVisitor_GoHome();

	void OnGameplayTaskActivated(UGameplayTask& Task) override;
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void InitializeFromAsset(UBehaviorTree& Asset) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
