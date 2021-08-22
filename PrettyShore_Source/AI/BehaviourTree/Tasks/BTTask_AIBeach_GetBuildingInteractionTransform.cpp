#include "BTTask_AIBeach_GetBuildingInteractionTransform.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Team9Assemble/AI/AIBeach_Controller_Base.h"
#include "Team9Assemble/Buildings/BuildingBase.h"

UBTTask_AIBeach_GetBuildingInteractionTransform::UBTTask_AIBeach_GetBuildingInteractionTransform() {
	NodeName = "Get Building Interaction Point";
}

void UBTTask_AIBeach_GetBuildingInteractionTransform::OnGameplayTaskActivated(UGameplayTask& Task) {}

EBTNodeResult::Type UBTTask_AIBeach_GetBuildingInteractionTransform::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory) {
	AAIBeach_Controller_Base* AIController = Cast<AAIBeach_Controller_Base>(OwnerComp.GetOwner());

	ABuildingBase* Building = Cast<ABuildingBase>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(BuildingKey.SelectedKeyName));
	if(Building != nullptr) {

		
		FTransform Transform = Building->GetAvailableInteractionTransform(AIController);

		
		
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(DestinationTransformKey.SelectedKeyName, Transform.GetLocation()/*+Building->GetActorLocation()*/);
		OwnerComp.GetBlackboardComponent()->SetValueAsRotator(RotateTowardsKey.SelectedKeyName, Transform.Rotator());
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}

void UBTTask_AIBeach_GetBuildingInteractionTransform::InitializeFromAsset(UBehaviorTree& Asset) {
	Super::InitializeFromAsset(Asset);
	UBlackboardData* BBAsset = Asset.GetBlackboardAsset();
	if (ensure(BBAsset)) {
		BuildingKey.ResolveSelectedKey(*BBAsset);
	}	
}

void UBTTask_AIBeach_GetBuildingInteractionTransform::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	float DeltaSeconds) {}
