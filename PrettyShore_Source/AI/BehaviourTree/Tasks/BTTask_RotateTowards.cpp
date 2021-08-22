
#include "BTTask_RotateTowards.h"
#include "GameFramework/Pawn.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
// #include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
// #include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

UBTTask_RotateTowards::UBTTask_RotateTowards()
{
	NodeName = TEXT("Smooth Rotate To Target");
	bNotifyTick = true;
}

void UBTTask_RotateTowards::OnGameplayTaskActivated(UGameplayTask& Task) { }

EBTNodeResult::Type UBTTask_RotateTowards::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_RotateTowards::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UBTTask_RotateTowards::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{

}

void UBTTask_RotateTowards::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	// FBTFaceDirectionTaskMemory* FaceDirectionMemory = (FBTFaceDirectionTaskMemory*)NodeMemory;

	AAIController* AIController = OwnerComp.GetAIOwner();

	if (!AIController)
		return;

	APawn* PawnOwner = AIController->GetPawn();

	if (!PawnOwner)
		return;

	// bool bSuccess = false;
	// // const FVector TargetLocation = GetTargetLocation(OwnerComp, bSuccess);
	//
	// if (!bSuccess)
	// {
	// 	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	// 	return;
	// }

	FRotator TargetRot = OwnerComp.GetBlackboardComponent()->GetValueAsRotator(RotateTowardsKey.SelectedKeyName);
	// TargetRot = FRotator::ZeroRotator;
	// const FVector DirectionToTarget = (TargetLocation - PawnOwner->GetActorLocation()).GetSafeNormal();
	FQuat Rot = FQuat::Slerp(PawnOwner->GetActorQuat(), TargetRot.Quaternion(), DeltaSeconds * GetRotationSpeed(OwnerComp));

	FRotator NewRot = Rot.Rotator();
	NewRot.Pitch = 0.0f;
	NewRot.Roll = 0.0f;

	PawnOwner->SetActorRotation(NewRot);

	if (FMath::IsNearlyEqual(TargetRot.Yaw, NewRot.Yaw, AcceptableTolerance)) {
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		// UE_LOG(LogTemp, Log, TEXT("ROTATION FINISH SUCCESS"));
		return;
	}
		// UE_LOG(LogTemp, Log, TEXT("ROTATION FINISH INPROGRESS"));
	FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);

}

void UBTTask_RotateTowards::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset))
	{
		// TargetLocationEntry.ResolveSelectedKey(*BBAsset);
		RotateTowardsKey.ResolveSelectedKey(*BBAsset);

		// if (RotationSpeedEntryType == EFGSmoothFaceDirectionSpeedType::BBEntry)
		// 	RotationSpeedBlackboard.ResolveSelectedKey(*BBAsset);
	}
}

uint16 UBTTask_RotateTowards::GetInstanceMemorySize() const
{
	return sizeof(FBTFaceDirectionTaskMemory);
}

FVector UBTTask_RotateTowards::GetTargetLocation(const UBehaviorTreeComponent& OwnerComp, bool& bSuccess) const
{
	bSuccess = true;

	// const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	//
	// if (BlackboardComp != nullptr)
	// {
	// 	if (TargetLocationEntry.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	// 	{
	// 		UObject* TargetObject = BlackboardComp->GetValue<UBlackboardKeyType_Object>(TargetLocationEntry.GetSelectedKeyID());
	// 		if (TargetObject != nullptr)
	// 		{
	// 			return CastChecked<AActor>(TargetObject)->GetActorLocation();
	// 		}
	// 	}
	// 	else if (TargetLocationEntry.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	// 	{
	// 		return BlackboardComp->GetValue<UBlackboardKeyType_Vector>(TargetLocationEntry.GetSelectedKeyID());
	// 	}
	// }
	//
	// bSuccess = false;

	return FVector::ZeroVector;
}

float UBTTask_RotateTowards::GetRotationSpeed(const UBehaviorTreeComponent& OwnerComp) const
{
	// if (RotationSpeedEntryType == EFGSmoothFaceDirectionSpeedType::BBEntry)
	// {
	// 	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	//
	// 	if (BlackboardComp && RotationSpeedBlackboard.SelectedKeyType == UBlackboardKeyType_Float::StaticClass())
	// 	{
	// 		return BlackboardComp->GetValue<UBlackboardKeyType_Float>(RotationSpeedBlackboard.GetSelectedKeyID());
	// 	}
	// }

	return RotationSpeedValue;
}