#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AILifeGuard_SetMoveSpeed.generated.h"

UCLASS()
class UBTTask_AILifeGuard_SetMoveSpeed  : public UBTTaskNode{
public:
	GENERATED_BODY()
	UBTTask_AILifeGuard_SetMoveSpeed();
	
	void OnGameplayTaskActivated(UGameplayTask& Task) override;
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void InitializeFromAsset(UBehaviorTree& Asset) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	float NewMoveSpeed;
};
