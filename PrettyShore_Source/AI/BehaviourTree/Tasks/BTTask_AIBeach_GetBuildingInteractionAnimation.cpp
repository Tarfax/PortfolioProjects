#include "BTTask_AIBeach_GetBuildingInteractionAnimation.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"
#include "Team9Assemble/Buildings/BuildingBase.h"

UBTTask_AIBeach_GetBuildingInteractionAnimation::UBTTask_AIBeach_GetBuildingInteractionAnimation() {}
void UBTTask_AIBeach_GetBuildingInteractionAnimation::OnGameplayTaskActivated(UGameplayTask& Task) {}

EBTNodeResult::Type UBTTask_AIBeach_GetBuildingInteractionAnimation::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory) {

	ABuildingBase* Building = Cast<ABuildingBase>(
		OwnerComp.GetBlackboardComponent()->GetValueAsObject(BuildingKey.SelectedKeyName));

	if (Building != nullptr) {
		OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Enum>(
			StoreInBuildingAnimationEnumKey.SelectedKeyName,
			static_cast<UBlackboardKeyType_Enum::FDataType>(Building->GetInteractionAnimation(
				SearchForBuildingInteractionType)));
		return EBTNodeResult::Succeeded;
	}
	else {
		return EBTNodeResult::Failed;
	}

}

void UBTTask_AIBeach_GetBuildingInteractionAnimation::InitializeFromAsset(UBehaviorTree& Asset) {
	UBlackboardData* BBAsset = Asset.BlackboardAsset;
	if (ensure(BBAsset) == true) {
		BuildingKey.ResolveSelectedKey(*BBAsset);
	}
}

void UBTTask_AIBeach_GetBuildingInteractionAnimation::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                                                               float DeltaSeconds) {}
