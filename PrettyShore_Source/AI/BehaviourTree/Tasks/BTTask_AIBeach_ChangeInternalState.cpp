// I like sand

#pragma once
#include "BTTask_AIBeach_ChangeInternalState.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"


UBTTask_AIBeach_ChangeInternalState::UBTTask_AIBeach_ChangeInternalState() {
	NodeName = "AIBeach Change Internal State";
}
void UBTTask_AIBeach_ChangeInternalState::OnGameplayTaskActivated(UGameplayTask& Task) {}

EBTNodeResult::Type UBTTask_AIBeach_ChangeInternalState::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory) {
	if (AIType == EAIType::Visitor) {
		OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Enum>(InternalStateKey.SelectedKeyName, static_cast<UBlackboardKeyType_Enum::FDataType>(VisitorTask));
	} else {
		OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Enum>(InternalStateKey.SelectedKeyName, static_cast<UBlackboardKeyType_Enum::FDataType>(LifeGuardTask));
	}
	return EBTNodeResult::Succeeded;
}

void UBTTask_AIBeach_ChangeInternalState::InitializeFromAsset(UBehaviorTree& Asset) {
	
	UBlackboardData* BBAsset = Asset.GetBlackboardAsset();
	if (ensure(BBAsset)) {
		InternalStateKey.ResolveSelectedKey(*BBAsset);
	}	
}

void UBTTask_AIBeach_ChangeInternalState::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	float DeltaSeconds) {}
