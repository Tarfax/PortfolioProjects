// I like sand

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Team9Assemble/Buildings/BuildingData.h"
#include "Team9Assemble/Buildings/Building_Observer.h"

#include "BTTask_AIBeach_FindNearstBuildingOfType.generated.h"

UCLASS()
class TEAM9ASSEMBLE_API UBTTask_AIBeach_FindNearstBuildingOfType : public UBTTaskNode {
	GENERATED_BODY()

	UBTTask_AIBeach_FindNearstBuildingOfType();
	void OnGameplayTaskActivated(UGameplayTask& Task) override;
	EBTNodeResult::Type SearchForAvailableHome(UBehaviorTreeComponent& OwnerComp, UBlackboardComponent* BlackboardComp,
	                                           FVector CharacterPosition, UBuilding_Observer* BuildingObserver);
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void InitializeFromAsset(UBehaviorTree& Asset) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	E_BuildingType SearchForBuildingType;
	
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector DestinationKey;
	
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector BuildingComponentKey;
	
	UPROPERTY(EditAnywhere, Category = Blackboard)
	bool IgnoreBuildingMaximumUsers;

	UPROPERTY(EditAnywhere, Category=Blackboard)
	bool SearchForHome;
};