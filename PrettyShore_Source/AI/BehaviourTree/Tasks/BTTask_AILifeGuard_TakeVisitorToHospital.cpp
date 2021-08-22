// I like sand


#include "BTTask_AILifeGuard_TakeVisitorToHospital.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Team9Assemble/AI/AIVisitor/AIVisitor_Controller.h"
#include "Team9Assemble/AI/AILifeguard/AILifeGuard_Controller.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_AILifeGuard_TakeVisitorToHospital::UBTTask_AILifeGuard_TakeVisitorToHospital() {
	NodeName = "AILifeGuard Leave Visitor At Building";
}

void UBTTask_AILifeGuard_TakeVisitorToHospital::OnGameplayTaskActivated(UGameplayTask& Task) {}

EBTNodeResult::Type UBTTask_AILifeGuard_TakeVisitorToHospital::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
                                                                           uint8* NodeMemory) {

	AAIVisitor_Controller* Visitor = GetAIVisitorFromKey(OwnerComp.GetBlackboardComponent());

	AAILifeGuard_Controller* LifeGuard = Cast<AAILifeGuard_Controller>(OwnerComp.GetOwner());

	if (Visitor != nullptr) {
		if (LifeGuard == nullptr) {
			UE_LOG(LogTemp, Log, TEXT("Why is Lifeguard nullptr?"));
		}
		else {
			UE_LOG(LogTemp, Log, TEXT("LIFEGUARD->FInishedSavingVisistor()"));
			ABuildingBase* Building = Cast<ABuildingBase>(
				OwnerComp.GetBlackboardComponent()->GetValueAsObject(BuildingKey.SelectedKeyName));
			Visitor->LeaveAtBuilding(Building);
			LifeGuard->FinishedSavingVisitor();
		}

		OwnerComp.GetBlackboardComponent()->SetValueAsObject(AIVisitor.SelectedKeyName, nullptr);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}

void UBTTask_AILifeGuard_TakeVisitorToHospital::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                                                         float DeltaSeconds) {}

AAIVisitor_Controller* UBTTask_AILifeGuard_TakeVisitorToHospital::GetAIVisitorFromKey(
	UBlackboardComponent* BlackboardComp) const {
	UBlackboardKeyType_Object::FDataType UObject = BlackboardComp->GetValue<UBlackboardKeyType_Object>(
		AIVisitor.SelectedKeyName);
	AAIVisitor_Controller* AIVisitorCast = Cast<AAIVisitor_Controller>(UObject);
	return AIVisitorCast;
}

void UBTTask_AILifeGuard_TakeVisitorToHospital::InitializeFromAsset(UBehaviorTree& Asset) {
	UBlackboardData* BBAsset = Asset.GetBlackboardAsset();
	if (ensure(BBAsset)) {
		AIVisitor.ResolveSelectedKey(*BBAsset);
		BuildingKey.ResolveSelectedKey(*BBAsset);
	}
}
