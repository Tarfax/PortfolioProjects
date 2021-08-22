#include "BTTask_AILifeGuard_FindBeachWork.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"
#include "Team9Assemble/Buildings/Building_Observer.h"
#include "Team9Assemble/World/BeachGameState.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Team9Assemble/AI/AITrash_Observer.h"
#include "Team9Assemble/AI/AI_BeachCharacter.h"

UBTTask_AILifeGuard_FindBeachWork::UBTTask_AILifeGuard_FindBeachWork() {
	NodeName = "AILifeGuard Find Beach Work";
}

void UBTTask_AILifeGuard_FindBeachWork::OnGameplayTaskActivated(UGameplayTask& Task) {}

void UBTTask_AILifeGuard_FindBeachWork::SetBeachWork(UBehaviorTreeComponent& OwnerComp, UBeachWork* BeachWork) {
	AAILifeGuard_Controller* Controller = Cast<AAILifeGuard_Controller>(OwnerComp.GetAIOwner());
	Controller->AssignBeachWork(BeachWork);
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(TaskDataKey.SelectedKeyName, BeachWork);
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(DestinationKey.SelectedKeyName, BeachWork->WorkLocation.GetLocation());
	OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Enum>(TaskTypeDataKey.SelectedKeyName, static_cast<UBlackboardKeyType_Enum::FDataType>(BeachWork->WorkType));
}

EBTNodeResult::Type UBTTask_AILifeGuard_FindBeachWork::
ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	AAILifeGuard_Controller* Controller = Cast<AAILifeGuard_Controller>(OwnerComp.GetAIOwner());

	ABeachGameState* GameState = Cast<ABeachGameState>(GetWorld()->GetGameState());

	UBuilding_Observer* Building_Observer = GameState->BuildingObserver;
	if (Building_Observer->HasWork() == true) {
		// UE_LOG(LogTemp, Log, TEXT("UBTTask_AILifeGuard_FindBeachWork::BUILDING HAS WORK!"));

		UBeachWork* BeachWork = Building_Observer->GetWork(Controller->AI_BeachCharacter->GetActorLocation());
		if (BeachWork != nullptr) {
			SetBeachWork(OwnerComp, BeachWork);
			// UE_LOG(LogTemp, Log, TEXT("Returning building work"));

			return EBTNodeResult::Succeeded;
		}
	}

	UAITrash_Observer* Trash_Observer = GameState->TrashObserver;
	if (Trash_Observer->HasWork() == true) {
		UBeachWork* BeachWork = Trash_Observer->GetWork();
		if (BeachWork != nullptr) {
			SetBeachWork(OwnerComp, BeachWork);
			// UE_LOG(LogTemp, Log, TEXT("Returning trash"));

			return EBTNodeResult::Succeeded;
		}
	}

	// UE_LOG(LogTemp, Log, TEXT("failed"));
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(TaskDataKey.SelectedKeyName, nullptr);
	return EBTNodeResult::Failed;
}

void UBTTask_AILifeGuard_FindBeachWork::InitializeFromAsset(UBehaviorTree& Asset) {
	Super::InitializeFromAsset(Asset);

}

void UBTTask_AILifeGuard_FindBeachWork::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                                                 float DeltaSeconds) {
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}
