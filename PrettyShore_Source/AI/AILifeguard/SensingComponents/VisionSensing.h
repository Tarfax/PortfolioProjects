// I like sand

#pragma once


#include "Components/ActorComponent.h"
#include "VisionSensing.generated.h"

class AAIVisitor_Controller;
class AAILifeGuard_Controller;

USTRUCT(BlueprintType)
struct FVisionSensingResults
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	AAIVisitor_Controller* SensedVisitor = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FVisionSensingEvent, const FVisionSensingResults, Results);

UCLASS(Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent))
class UVisionSensing : public UActorComponent {
	GENERATED_BODY()

public:
	UVisionSensing();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadWrite)
	AAILifeGuard_Controller* AILifeGuard;

	float GetVisionInRadians() const;
	bool IsAIVisitorVisible(FVector& PointToTest, FVector& Origin, FVector& Direction, float DistanceMinimum) const;
	
	UPROPERTY(EditAnywhere, Category="AIVision")
	float VisionAngle = 35;

	UPROPERTY(EditAnywhere, Category="AIVision")
	float VisionLength = 3500;
	
	UPROPERTY(BlueprintAssignable)
	FVisionSensingEvent OnVisitorDrowningSensed;

	UPROPERTY(BlueprintAssignable)
	FVisionSensingEvent OnVisitorDrowningLost;

	UPROPERTY(EditAnywhere, Category="AIVision Debug")
	bool DebugVision = false;
};
