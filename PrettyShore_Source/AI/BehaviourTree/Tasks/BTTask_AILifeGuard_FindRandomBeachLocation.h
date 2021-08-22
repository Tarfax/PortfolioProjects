// I like sand

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AILifeGuard_FindRandomBeachLocation.generated.h"

/**
 * 
 */
UCLASS()
class UBTTask_AILifeGuard_FindRandomBeachLocation : public UBTTaskNode {
	GENERATED_BODY()
	UBTTask_AILifeGuard_FindRandomBeachLocation();
	
	void OnGameplayTaskActivated(UGameplayTask& Task) override;
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void InitializeFromAsset(UBehaviorTree& Asset) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector DestinationKey;
};
