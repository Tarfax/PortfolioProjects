#pragma once
#include "BehaviorTree/BTTaskNode.h"
#include "Team9Assemble/AI/Models/BeachWork.h"
#include "BTTask_AILifeguard_BeachWork_FindNearest.generated.h"

class AAI_Trash;

UCLASS()
class UBTTask_AILifeguard_BeachWork_FindNearest: public UBTTaskNode {
	GENERATED_BODY()
public:
	UBTTask_AILifeguard_BeachWork_FindNearest();
	void OnGameplayTaskActivated(UGameplayTask& Task) override;
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void InitializeFromAsset(UBehaviorTree& Asset) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	AAI_Trash* SearchForTrash(AActor* Actor, AAILifeGuard_Controller* Controller, UBehaviorTreeComponent& OwnerComp) const;
	ABuildingBase* SearchForTowel(AActor* Actor, AAILifeGuard_Controller* Controller, UBehaviorTreeComponent& OwnerComp) const;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector BeachWorkKey;
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector DestinationKey;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	TArray<TEnumAsByte<EObjectTypeQuery>> OverlapObjectTypes;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	EBeachWorkType WorkTypeToSearchFor;
	UPROPERTY(EditAnywhere, Category = Blackboard)
	float SearchRadius = 500.0f;
};
