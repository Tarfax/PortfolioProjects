#include "RoadSegment.h"
#include "Components/SplineComponent.h"

ARoadSegment::ARoadSegment()
{
	SplineComp = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	RootComponent = SplineComp;
}
