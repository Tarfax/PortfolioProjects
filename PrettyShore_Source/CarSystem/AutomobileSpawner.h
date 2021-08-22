#pragma once
#include "GameFramework/Actor.h"
#include "AutomobileSpawner.generated.h"

class ARoadSegment;
class AAutomobile;

UCLASS()
class AAutomobileSpawner : public AActor
{
	GENERATED_BODY()
public:
	AAutomobileSpawner();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void SpawnAutomobile(ARoadSegment* SpawnAtSegment);
	void DestroyAutomobile(AAutomobile* AutomobileToDestroy);

	UPROPERTY(EditAnywhere)
	int MaxSimultaneouslyAllowedCars = 5;

	UPROPERTY(EditAnywhere)
	float TimeBetweenSpawns = 5.f;
	
	UPROPERTY(EditAnywhere)
	TArray<ARoadSegment*> StartSegments;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AAutomobile>> CarsToSpawn;

private:
	
	TArray<AAutomobile*> SpawnedAutomobiles;
	float SpawnTimer;
};
