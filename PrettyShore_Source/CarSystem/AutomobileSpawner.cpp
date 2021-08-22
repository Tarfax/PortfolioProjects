#include "AutomobileSpawner.h"
#include "Automobile.h"
#include "RoadSegment.h"
#include "Kismet/KismetMathLibrary.h"

AAutomobileSpawner::AAutomobileSpawner()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAutomobileSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	const int MaxIterations = MaxSimultaneouslyAllowedCars > StartSegments.Num() ? StartSegments.Num() : MaxSimultaneouslyAllowedCars;
	for (int i = 0; i < MaxIterations; ++i)
	{
		SpawnAutomobile(StartSegments[i]);
	}

	SpawnTimer = TimeBetweenSpawns;
}

void AAutomobileSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (SpawnTimer > 0.f)
		SpawnTimer -= DeltaTime;

	if (SpawnedAutomobiles.Num() < MaxSimultaneouslyAllowedCars && SpawnTimer < 0.f)
	{
		const int Rnd = UKismetMathLibrary::RandomIntegerInRange(0, StartSegments.Num() - 1);
		SpawnAutomobile(StartSegments[Rnd]);
		SpawnTimer = TimeBetweenSpawns;
	}
}

void AAutomobileSpawner::SpawnAutomobile(ARoadSegment* SpawnAtSegment)
{
	const int CarClass = UKismetMathLibrary::RandomIntegerInRange(0, CarsToSpawn.Num() - 1);
	const FActorSpawnParameters SpawnInfo;
	
	auto* NewAutomobile = GetWorld()->SpawnActor<AAutomobile>(CarsToSpawn[CarClass],
		SpawnAtSegment->GetActorLocation(), SpawnAtSegment->GetActorRotation(), SpawnInfo);

	NewAutomobile->Setup(this);
	NewAutomobile->SetTargetSegment(SpawnAtSegment);
	NewAutomobile->ReleaseAutomobile();

	SpawnedAutomobiles.Add(NewAutomobile);
}

void AAutomobileSpawner::DestroyAutomobile(AAutomobile* AutomobileToDestroy)
{
	if (AutomobileToDestroy == nullptr)
		return;
	
	if (SpawnedAutomobiles.Contains(AutomobileToDestroy))
	{
		SpawnedAutomobiles.Remove(AutomobileToDestroy);
		AutomobileToDestroy->Destroy();
	}
}

//setactorcollisionenable
	//settickenable
	//setvisibilityfalse
