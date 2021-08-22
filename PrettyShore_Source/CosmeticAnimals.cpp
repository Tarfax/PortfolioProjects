// I like sand

#include "CosmeticAnimals.h"

#include "SkeletalDebugRendering.h"
#include "Components/SplineComponent.h"

// Sets default values
ACosmeticAnimals::ACosmeticAnimals()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	RootComponent = SceneComponent;
}

// Called every frame
void ACosmeticAnimals::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (AnimalPaths.Num() > 0)
	{
		//If there is no path (true), destroy the actor, otherwise (there is a path) pick a path
		if (activePath == nullptr)
		{
			RandomPath = rand() % AnimalPaths.Num(); //Picks a random path available
			activePath = AnimalPaths[RandomPath];
		}

		else
		{
			if (loopMotion)
			{
				if (traveledDistance >= activePath->GetSplineLength())
				{
					traveledDistance = 0;
				}
			}
			else
			{
				//If the object has reach an end, destroy it
				if (traveledDistance >= activePath->GetSplineLength())
				{
					Destroy();
				}
			}

			//Keep it moving
			traveledDistance += TravelSpeed * DeltaTime;
			FTransform ObjectOnSplinePosition = activePath->GetTransformAtDistanceAlongSpline
				(traveledDistance, ESplineCoordinateSpace::World);
			SetActorTransform(ObjectOnSplinePosition);
		}
	}
}
