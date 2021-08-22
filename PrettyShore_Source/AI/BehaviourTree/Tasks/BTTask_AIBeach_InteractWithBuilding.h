// I like sand

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AIBeach_InteractWithBuilding.generated.h"

UCLASS()
class TEAM9ASSEMBLE_API UBTTask_AIBeach_InteractWithBuilding  : public UBTTaskNode {
	GENERATED_BODY()
	UBTTask_AIBeach_InteractWithBuilding();
	void OnGameplayTaskActivated(UGameplayTask& Task) override;
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void InitializeFromAsset(UBehaviorTree& Asset) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
