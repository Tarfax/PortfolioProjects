// I like sand

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"

#include "BTTask_AILifeGuard_StartSavingVisitorFromDrowning.generated.h"

class AAIVisitor_Controller;

UCLASS()
class TEAM9ASSEMBLE_API UBTTask_AILifeGuard_StartSavingVisitorFromDrowning : public UBTTaskNode {
	GENERATED_BODY()

	UBTTask_AILifeGuard_StartSavingVisitorFromDrowning();
	
	void OnGameplayTaskActivated(UGameplayTask& Task) override;
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void InitializeFromAsset(UBehaviorTree& Asset) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	AAIVisitor_Controller* GetAIVisitorFromKey(UBlackboardComponent* BlackboardComp) const;
	
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector AIVisitor;
	
};
