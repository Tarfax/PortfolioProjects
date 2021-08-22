#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
// #include "Team9Assemble/Buildings/BuildingData.h"
#include "BTTask_AIBeach_GetBuildingInteractionTransform.generated.h"

UCLASS()
class UBTTask_AIBeach_GetBuildingInteractionTransform  : public UBTTaskNode  {
	GENERATED_BODY()
public:

	UBTTask_AIBeach_GetBuildingInteractionTransform();
	void OnGameplayTaskActivated(UGameplayTask& Task) override;
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void InitializeFromAsset(UBehaviorTree& Asset) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector BuildingKey;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector DestinationTransformKey;
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector RotateTowardsKey;
};
