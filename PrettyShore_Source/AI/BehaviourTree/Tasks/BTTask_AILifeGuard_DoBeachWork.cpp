#include "BTTask_AILifeGuard_DoBeachWork.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Team9Assemble/AI/Models/BeachWork.h"

UBTTask_AILifeGuard_DoBeachWork::UBTTask_AILifeGuard_DoBeachWork() {
	bNotifyTick = true;
	NodeName = "AILifeGuard Do Beach Work";
}

void UBTTask_AILifeGuard_DoBeachWork::OnGameplayTaskActivated(UGameplayTask& Task) {}

EBTNodeResult::Type UBTTask_AILifeGuard_DoBeachWork::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	FBTBeachWorkMemory* timer = reinterpret_cast<FBTBeachWorkMemory*>(NodeMemory);
	timer->Timer = WorkTime;
	return EBTNodeResult::InProgress;
}

void UBTTask_AILifeGuard_DoBeachWork::InitializeFromAsset(UBehaviorTree& Asset) {
	UBlackboardData* BBAsset = Asset.GetBlackboardAsset();
	if (ensure(BBAsset)) {
		TaskDataKey.ResolveSelectedKey(*BBAsset);
	}
	Super::InitializeFromAsset(Asset);
}

void UBTTask_AILifeGuard_DoBeachWork::
TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	FBTBeachWorkMemory* timer = reinterpret_cast<FBTBeachWorkMemory*>(NodeMemory);


	timer->Timer -= DeltaSeconds;
	// UE_LOG(LogTemp, Log, TEXT("BeachWork->Timer: %f"), BeachWork->Timer);
	if (timer->Timer <= 0.0f) {
		UBeachWork* BeachWork = Cast<UBeachWork>(
			OwnerComp.GetBlackboardComponent()->GetValueAsObject(TaskDataKey.SelectedKeyName));

		if (BeachWork != nullptr) {
			BeachWork->WorkOwner->FinishBeachWork();
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(
				TaskDataKey.SelectedKeyName, nullptr);
			UE_LOG(LogTemp, Log, TEXT("DoWork return SUCCESS!"));
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

			
			return;
		}
		UE_LOG(LogTemp, Log, TEXT("DoWork return FAIL!"));

		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
}
