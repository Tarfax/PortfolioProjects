// I like sand

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AIVisitor_SetupBeachHomeArea.generated.h"

class AAIVisitor_BeachHome;

struct FBTTimerTaskMemory {
	float Timer = 0.0f;
};

UCLASS()
class UBTTask_AIVisitor_SetupBeachHomeArea : public UBTTaskNode {
	GENERATED_BODY()
	
public:
	UBTTask_AIVisitor_SetupBeachHomeArea();
	
	void OnGameplayTaskActivated(UGameplayTask& Task) override;
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void InitializeFromAsset(UBehaviorTree& Asset) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual uint16 GetInstanceMemorySize() const override;

	FBTTimerTaskMemory Timer;
	UPROPERTY(EditAnywhere, Category = Blackboard)
	float Time = 0.5f;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector TileKey;
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector BuildingKey;
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector HomePosition;

};
