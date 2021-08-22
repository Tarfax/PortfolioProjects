// I like sand

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AIVisitor_FindRandomSwimLocation.generated.h"

UCLASS()
class UBTTask_AIVisitor_FindRandomSwimLocation: public UBTTaskNode{
	GENERATED_BODY()
	public:
	UBTTask_AIVisitor_FindRandomSwimLocation();
	
	void OnGameplayTaskActivated(UGameplayTask& Task) override;
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void InitializeFromAsset(UBehaviorTree& Asset) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector DestinationKey;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	bool IsGoingToSwim;
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector HomePositionKey;
	
};
