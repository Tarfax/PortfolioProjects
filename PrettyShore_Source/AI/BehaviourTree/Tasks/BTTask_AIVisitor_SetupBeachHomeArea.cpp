// I like sand


#include "BTTask_AIVisitor_SetupBeachHomeArea.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Team9Assemble/TileSystem/Tile.h"
#include "Team9Assemble/AI/AIVisitor/AIVisitor_Controller.h"
#include "Team9Assemble/Buildings/BuildingBase.h"


UBTTask_AIVisitor_SetupBeachHomeArea::UBTTask_AIVisitor_SetupBeachHomeArea() {
	bNotifyTick = true;
	NodeName = "AIVisitor Setup Beach Home Area";
}

void UBTTask_AIVisitor_SetupBeachHomeArea::OnGameplayTaskActivated(UGameplayTask& Task) {}

EBTNodeResult::Type UBTTask_AIVisitor_SetupBeachHomeArea::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
                                                                      uint8* NodeMemory) {

	AAIVisitor_Controller* controller = Cast<AAIVisitor_Controller>(OwnerComp.GetOwner());
	controller->SetDebugText.Broadcast("Unfolding blanket and getting it ready for action");
	FBTTimerTaskMemory* timer = reinterpret_cast<FBTTimerTaskMemory*>(NodeMemory);
	timer->Timer = Time;
	return EBTNodeResult::InProgress;
	

	//
	// ABuildingBase* Building = Cast<ABuildingBase>(
	// 			OwnerComp.GetBlackboardComponent()->GetValueAsObject(BuildingKey.SelectedKeyName));
	//
	//
	//
	// timer->Timer -= DeltaSeconds;
	// if (timer->Timer <= 0.0f) {
	// AAIVisitor_Controller* controller = Cast<AAIVisitor_Controller>(OwnerComp.GetOwner());
	// UTile* Tile = Cast<UTile>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TileKey.SelectedKeyName));
	// 	
	// if (Tile != nullptr) {
	// 	Building = controller->SetupBeachHomeArea(Tile, Building);
	// 	if (Building != nullptr) {
	// 		OwnerComp.GetBlackboardComponent()->SetValueAsVector(HomePosition.SelectedKeyName, Building->ArrowEnter->GetComponentLocation());
	// 		return EBTNodeResult::Succeeded;
	// 	}
	// }
	// 	
	// if (Building != nullptr) {
	// 	ABuildingBase* HomeBuilding = controller->SetupBeachHomeArea(Tile, Building);
	// 	if (HomeBuilding == nullptr) {
	// 		return EBTNodeResult::Failed;
	// 	}
	// 	OwnerComp.GetBlackboardComponent()->SetValueAsVector(HomePosition.SelectedKeyName, Building->ArrowEnter->GetComponentLocation());
	// 	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	// 	return EBTNodeResult::Succeeded;
	// }
	// return EBTNodeResult::Failed;
}

void UBTTask_AIVisitor_SetupBeachHomeArea::InitializeFromAsset(UBehaviorTree& Asset) {

	UBlackboardData* BBAsset = Asset.BlackboardAsset;
	if (ensure(BBAsset) == true) {
		TileKey.ResolveSelectedKey(*BBAsset);
		BuildingKey.ResolveSelectedKey(*BBAsset);
	}

}

void UBTTask_AIVisitor_SetupBeachHomeArea::
TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {

	FBTTimerTaskMemory* timer = reinterpret_cast<FBTTimerTaskMemory*>(NodeMemory);
	ABuildingBase* Building = Cast<ABuildingBase>(
				OwnerComp.GetBlackboardComponent()->GetValueAsObject(BuildingKey.SelectedKeyName));


	
	timer->Timer -= DeltaSeconds;
	if (timer->Timer <= 0.0f) {
		AAIVisitor_Controller* controller = Cast<AAIVisitor_Controller>(OwnerComp.GetOwner());
		UTile* Tile = Cast<UTile>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TileKey.SelectedKeyName));
		
		if (Tile != nullptr) {
			Building = controller->SetupBeachHomeArea(Tile, Building);
			if (Building != nullptr) {
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(HomePosition.SelectedKeyName, Building->ArrowEnter->GetComponentLocation());
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
			}
		}
		
		if (Building != nullptr) {
			ABuildingBase* HomeBuilding = controller->SetupBeachHomeArea(Tile, Building);
			if (HomeBuilding == nullptr) {
				FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
				return;
			}
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(HomePosition.SelectedKeyName, Building->ArrowEnter->GetComponentLocation());
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}
	FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);

}

uint16 UBTTask_AIVisitor_SetupBeachHomeArea::GetInstanceMemorySize() const {
	return sizeof(FBTTimerTaskMemory);
}
