#include "BTTask_AIBeach_MoveTo.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Team9Assemble/AI/AIBeach_Controller_Base.h"
#include "Team9Assemble/AI/AI_BeachCharacter.h"

UBTTask_AIBeach_MoveTo::UBTTask_AIBeach_MoveTo() {
	NodeName = "Move AI To";
	bNotifyTick = true;
}

void UBTTask_AIBeach_MoveTo::OnGameplayTaskActivated(UGameplayTask& Task) {}

EBTNodeResult::Type UBTTask_AIBeach_MoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	return EBTNodeResult::InProgress;
}

void UBTTask_AIBeach_MoveTo::InitializeFromAsset(UBehaviorTree& Asset) {
	Super::InitializeFromAsset(Asset);
	UBlackboardData* BBAsset = Asset.GetBlackboardAsset();
	if (ensure(BBAsset)) {
		IsInMovingStateKey.ResolveSelectedKey(*BBAsset);
		IsInWaterKey.ResolveSelectedKey(*BBAsset);
		DestinationKey.ResolveSelectedKey(*BBAsset);
		BuildingDestinationKey.ResolveSelectedKey(*BBAsset);
	}

}

void UBTTask_AIBeach_MoveTo::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	FBTMoveToMemory* Memory = reinterpret_cast<FBTMoveToMemory*>(NodeMemory);
	AAIBeach_Controller_Base* AIBeachController = Cast<AAIBeach_Controller_Base>(OwnerComp.GetAIOwner());
	// UE_LOG(LogTemp, Log, TEXT("TickTask"));

	if (Memory != nullptr) {

		if (Memory->IsMoving == false) {
			const bool IsInMoveState = OwnerComp.GetBlackboardComponent()->GetValueAsBool(
				IsInMovingStateKey.SelectedKeyName);
			const bool IsInWater = OwnerComp.GetBlackboardComponent()->GetValueAsBool(IsInWaterKey.SelectedKeyName);

			if (IsInMoveState == true || IsInWater == true) {
				FVector Destination;
				if (MoveToBuildingDestination == false) {
					Destination = OwnerComp.GetBlackboardComponent()->GetValueAsVector(DestinationKey.SelectedKeyName);
				}
				else {
					Destination = Cast<AActor>(
							OwnerComp.GetBlackboardComponent()->
							          GetValueAsObject(BuildingDestinationKey.SelectedKeyName))->
						GetActorLocation();
				}

				if (AIBeachController) {
					Memory->MoveRequestID = AIBeachController->MoveToLocation(
						Destination, AcceptableRadius, false, true, true, false, nullptr, true);

					if (Memory->MoveRequestID == EPathFollowingRequestResult::Failed) {
						FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
						return;
					}
					if (Memory->MoveRequestID == EPathFollowingRequestResult::AlreadyAtGoal) {
						FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
						return;
					}
					Memory->IsMoving = true;
					Memory->CheckTimer = 1.0f;
					return;
				}
			}
		}
		else {


			Memory->CheckTimer -= DeltaSeconds;
			if (Memory->CheckTimer <= 0.0f) {
				FVector Destination;
				if (MoveToBuildingDestination == false) {
					Destination = OwnerComp.GetBlackboardComponent()->GetValueAsVector(DestinationKey.SelectedKeyName);
				}
				else {
					Destination = Cast<AActor>(
							OwnerComp.GetBlackboardComponent()->
							          GetValueAsObject(BuildingDestinationKey.SelectedKeyName))->
						GetActorLocation();
				}

				if (AIBeachController) {
					Memory->MoveRequestID = AIBeachController->MoveToLocation(
						Destination, AcceptableRadius, false, true, true, false, nullptr, true);

					if (Memory->MoveRequestID == EPathFollowingRequestResult::Failed) {
						FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
						return;
					}
					if (Memory->MoveRequestID == EPathFollowingRequestResult::AlreadyAtGoal) {
						AIBeachController->StopMovement();
						AIBeachController->AI_BeachCharacter->GetMovementComponent()->StopMovementImmediately();
						FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
						return;
					}
					return;
				}
				Memory->CheckTimer = 1.0f;
			}
		}

	}
	else {
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

}

EBTNodeResult::Type UBTTask_AIBeach_MoveTo::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	// AAIBeach_Controller_Base* ai = Cast<AAIBeach_Controller_Base>(OwnerComp.GetAIOwner());
	// if (ai) {
	// 	ai->StopMovement();
	// 	ai->AI_BeachCharacter->GetMovementComponent()->StopMovementImmediately();
	// }
	return EBTNodeResult::Aborted;
}
