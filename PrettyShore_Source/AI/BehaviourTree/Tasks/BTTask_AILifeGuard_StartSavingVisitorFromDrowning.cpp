// I like sand


#include "BTTask_AILifeGuard_StartSavingVisitorFromDrowning.h"


#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Team9Assemble/AI/AIVisitor/AIVisitor_Controller.h"
#include "Team9Assemble/AI/AILifeguard/AILifeGuard_Controller.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTTask_AILifeGuard_StartSavingVisitorFromDrowning::UBTTask_AILifeGuard_StartSavingVisitorFromDrowning() {
	NodeName = "AILifeGuard Start Rescuing Visitor";

}
void UBTTask_AILifeGuard_StartSavingVisitorFromDrowning::OnGameplayTaskActivated(UGameplayTask& Task) {}

EBTNodeResult::Type UBTTask_AILifeGuard_StartSavingVisitorFromDrowning::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory) {

	AAIVisitor_Controller* Visitor = GetAIVisitorFromKey(OwnerComp.GetBlackboardComponent());
	AAILifeGuard_Controller* Lifeguard = Cast<AAILifeGuard_Controller>(OwnerComp.GetOwner());
	if (Visitor != nullptr) {
		Visitor->SavedFromDrowning(Cast<AAILifeGuard_Controller>(OwnerComp.GetOwner()));
		Lifeguard->GoingOutOfWater();
		return EBTNodeResult::Succeeded;
	} else {
		UE_LOG(LogTemp, Log, TEXT("Life Guard Visitor was nullptr"));
	}
	return EBTNodeResult::Failed;
}

void UBTTask_AILifeGuard_StartSavingVisitorFromDrowning::InitializeFromAsset(UBehaviorTree& Asset) {
	
	UBlackboardData* BBAsset = Asset.GetBlackboardAsset();
	if (ensure(BBAsset)) {
		AIVisitor.ResolveSelectedKey(*BBAsset);
	}
}

void UBTTask_AILifeGuard_StartSavingVisitorFromDrowning::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	float DeltaSeconds) {}


AAIVisitor_Controller* UBTTask_AILifeGuard_StartSavingVisitorFromDrowning::GetAIVisitorFromKey(UBlackboardComponent* BlackboardComp) const {
	UBlackboardKeyType_Object::FDataType UObject = BlackboardComp->GetValue<UBlackboardKeyType_Object>(AIVisitor.GetSelectedKeyID());
	AAIVisitor_Controller* AIVisitorCast = Cast<AAIVisitor_Controller>(UObject);
	return AIVisitorCast;
}
