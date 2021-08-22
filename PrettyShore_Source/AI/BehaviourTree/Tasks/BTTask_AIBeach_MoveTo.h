#pragma once
#include "BehaviorTree/BTTaskNode.h"
#include "Navigation/PathFollowingComponent.h"
#include "BTTask_AIBeach_MoveTo.generated.h"

struct FBTMoveToMemory {
	bool IsMoving = false;
	EPathFollowingRequestResult::Type MoveRequestID;
	float CheckTimer;
};

UCLASS()
class UBTTask_AIBeach_MoveTo : public UBTTaskNode {
	GENERATED_BODY()
public:
	UBTTask_AIBeach_MoveTo();
	void OnGameplayTaskActivated(UGameplayTask& Task) override;
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void InitializeFromAsset(UBehaviorTree& Asset) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override; 

	UPROPERTY(EditAnywhere, Category = Destination,meta = (EditCondition = "MoveToBuildingDestination == false"))
	FBlackboardKeySelector DestinationKey;
	UPROPERTY(EditAnywhere, Category = Destination)
	bool MoveToBuildingDestination;
	UPROPERTY(EditAnywhere, Category = Destination,meta = (EditCondition = "MoveToBuildingDestination == true"))
	FBlackboardKeySelector BuildingDestinationKey;
	UPROPERTY(EditAnywhere, Category = Destination)
	float AcceptableRadius = 2.0f;

	UPROPERTY(EditAnywhere, Category = CanMove)
	FBlackboardKeySelector IsInMovingStateKey;
	UPROPERTY(EditAnywhere, Category = CanMove)
	FBlackboardKeySelector IsInWaterKey;


	
};
