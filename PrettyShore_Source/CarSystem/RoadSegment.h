#pragma once
#include "GameFramework/Actor.h"
#include "RoadSegment.generated.h"

class USplineComponent;

UENUM(BlueprintType)
enum class RoadSegmentType : uint8
{
	StartSegment = 0 UMETA(DisplayName = "Start Segment"),
	MiddleSegment = 1 UMETA(DisplayName = "Middle Segment"),
	EndSegment = 2 UMETA(DisplayName = "End Segment")
};

UCLASS()
class ARoadSegment : public AActor
{
	GENERATED_BODY()
public:
	ARoadSegment();

	UPROPERTY(EditAnywhere)
	RoadSegmentType SegmentType;
	
	UPROPERTY(EditAnywhere)
	USplineComponent* SplineComp;

	UPROPERTY(EditAnywhere)
	TArray<ARoadSegment*> SegmentConnections;
};
