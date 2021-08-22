#pragma once
#include "BehaviorTree/BTTaskNode.h"
#include "Team9Assemble/AI/Models/BeachWork.h"

#include "BTTask_AILifeGuard_FindBeachWork.generated.h"

UCLASS()
class UBTTask_AILifeGuard_FindBeachWork  : public UBTTaskNode {
	GENERATED_BODY()
public:
	UBTTask_AILifeGuard_FindBeachWork();
	void OnGameplayTaskActivated(UGameplayTask& Task) override;
	void SetBeachWork(UBehaviorTreeComponent& OwnerComp, UBeachWork* BeachWork);
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void InitializeFromAsset(UBehaviorTree& Asset) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector DestinationKey;
	
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector TaskDataKey;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector TaskTypeDataKey;
};
