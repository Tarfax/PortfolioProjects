// I like sand

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Team9Assemble/Buildings/BuildingBase.h"

#include "BTTask_AILifeGuard_TakeVisitorToHospital.generated.h"

class AAIVisitor_Controller;

UCLASS()
class TEAM9ASSEMBLE_API UBTTask_AILifeGuard_TakeVisitorToHospital : public UBTTaskNode {
	GENERATED_BODY()

	UBTTask_AILifeGuard_TakeVisitorToHospital();
	
	void OnGameplayTaskActivated(UGameplayTask& Task) override;
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void InitializeFromAsset(UBehaviorTree& Asset) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	AAIVisitor_Controller* GetAIVisitorFromKey(UBlackboardComponent* BlackboardComp) const;
	
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector AIVisitor;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector BuildingKey;
};
