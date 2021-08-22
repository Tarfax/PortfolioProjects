#pragma once
#include "BTTask_Helper.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AIBeach_GetBuildingInteractionAnimation.generated.h"

UCLASS()
class UBTTask_AIBeach_GetBuildingInteractionAnimation: public UBTTaskNode  {
	GENERATED_BODY()

public:
	UBTTask_AIBeach_GetBuildingInteractionAnimation();
	void OnGameplayTaskActivated(UGameplayTask& Task) override;
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void InitializeFromAsset(UBehaviorTree& Asset) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector BuildingKey;
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector StoreInBuildingAnimationEnumKey;
	UPROPERTY(EditAnywhere, Category=Blackboard)
	EAIBuildingInteractionType SearchForBuildingInteractionType;
};
