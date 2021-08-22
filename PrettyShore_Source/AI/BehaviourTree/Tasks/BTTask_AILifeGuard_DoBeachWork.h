#pragma once
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AILifeGuard_DoBeachWork.generated.h"

struct FBTBeachWorkMemory {
	float Timer = 0.0f;
};

UCLASS()
class UBTTask_AILifeGuard_DoBeachWork  : public UBTTaskNode{
	GENERATED_BODY()
public:

	UBTTask_AILifeGuard_DoBeachWork();
	void OnGameplayTaskActivated(UGameplayTask& Task) override;
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void InitializeFromAsset(UBehaviorTree& Asset) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	

	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector TaskDataKey;

	FBTBeachWorkMemory Timer;
	
	UPROPERTY(EditAnywhere, Category=Blackboard)
	float WorkTime = 0.1f;
};
