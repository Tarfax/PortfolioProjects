#pragma once
#include "BehaviorTree/BTTaskNode.h"
#include "Team9Assemble/AI/AILifeguard/AILifeGuard.h"
#include "Team9Assemble/AI/AIVisitor/AIVisitor.h"
#include "BTTask_Helper.h"
#include "BTTask_SetAnimation.generated.h"

UCLASS()
class UBTTask_SetAnimation  : public UBTTaskNode  {
	GENERATED_BODY()
public:
	UBTTask_SetAnimation();

	void OnGameplayTaskActivated(UGameplayTask& Task) override;
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void InitializeFromAsset(UBehaviorTree& Asset) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = AnimationType)
	EAI_CustomAnimations AIVisitor_Animation;
};
