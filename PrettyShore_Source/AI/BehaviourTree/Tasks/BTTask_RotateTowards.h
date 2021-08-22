#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RotateTowards.generated.h"

struct FBTFaceDirectionTaskMemory
{
	float SomeFloat = 0.0f;
};

UENUM(BlueprintType)
enum class EFGSmoothFaceDirectionSpeedType : uint8
{
	Value,
	BBEntry
};

UCLASS()
class UBTTask_RotateTowards : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_RotateTowards();

	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual uint16 GetInstanceMemorySize() const override;

	FVector GetTargetLocation(const UBehaviorTreeComponent& OwnerComp, bool& bSuccess) const;
	float GetRotationSpeed(const UBehaviorTreeComponent& OwnerComp) const;

	// UPROPERTY(EditAnywhere, Category = Entry)
	// EFGSmoothFaceDirectionSpeedType RotationSpeedEntryType = EFGSmoothFaceDirectionSpeedType::Value;

	// Can be either an AActor or FVector
	// UPROPERTY(EditAnywhere, Category = Entry)
	// FBlackboardKeySelector TargetLocationEntry;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector RotateTowardsKey;
	
	UPROPERTY(EditAnywhere, Category = Blackboard)
	float RotationSpeedValue = 2.0f;
	// UPROPERTY(EditAnywhere, Category = Entry, meta = (EditCondition = "RotationSpeedEntryType == EFGSmoothFaceDirectionSpeedType::BBEntry", EditConditionHides, DisplayName = "RotationSpeed"))
	// FBlackboardKeySelector RotationSpeedBlackboard;

	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (ClampMin = "0.0"))
	float AcceptableTolerance = 1.0f;
};
