#include "Automobile.h"

#include "AutomobileSpawner.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SplineComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "RoadSegment.h"

AAutomobile::AAutomobile()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	RootComponent = SceneComp;

	CarMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CarMeshComponent"));
	CarMeshComp->SetupAttachment(SceneComp);

	PeepMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PeepMeshComp"));
	PeepMeshComp->SetupAttachment(SceneComp);
}

void AAutomobile::BeginPlay()
{
	Super::BeginPlay();
}

void AAutomobile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsReleased == false)
		return;
	
	if (TargetSpline == nullptr)
		return;

	const float SplineModifier = TargetSpline->GetScaleAtDistanceAlongSpline(DistanceAlongSpline).Y;
	DistanceAlongSpline += (Speed * SplineModifier) * DeltaTime;

	if (DistanceAlongSpline >= TargetSpline->GetSplineLength())
	{
		if (TargetSegment->SegmentConnections.Num() != 0)
		{
			ARoadSegment* NewSegment;

			if (TargetSegment->SegmentConnections.Num() == 1)
			{
				NewSegment = TargetSegment->SegmentConnections[0];
			}
			else
			{
				const int Rnd = UKismetMathLibrary::RandomIntegerInRange(0, 100);
				const int SegmentIndex = Rnd >= 50 ? 1 : 0;
				NewSegment = TargetSegment->SegmentConnections[SegmentIndex];
			}

			DistanceAlongSpline = 0.f;
			SetTargetSegment(NewSegment);
		}
		else
		{
			Spawner->DestroyAutomobile(this);
			return;
		}
	}

	const FVector SplineVector = TargetSpline->GetWorldLocationAtDistanceAlongSpline(DistanceAlongSpline);
	const FVector ActualLocation = UKismetMathLibrary::VLerp(GetActorLocation(), SplineVector, Responsiveness * DeltaTime);
	const FRotator LookRot = ((ActualLocation - GetActorLocation()).GetSafeNormal()).Rotation();

	SetActorLocationAndRotation(ActualLocation, LookRot);

	// Debug: SplineVector Location.
	// const FVector EndPoint = SplineVector + FVector::UpVector * 500.f;
	// UKismetSystemLibrary::DrawDebugLine(this, SplineVector, EndPoint,
	// 	FColor::Red, 0.f, 50.f);
}

void AAutomobile::Setup(AAutomobileSpawner* SpawnerRef)
{
	Spawner = SpawnerRef;
}

void AAutomobile::ReleaseAutomobile()
{
	bIsReleased = true;
}

void AAutomobile::SetTargetSegment(ARoadSegment* NewTargetSegment)
{
	if (NewTargetSegment == nullptr)
		return;
	
	TargetSegment = NewTargetSegment;
	TargetSpline = TargetSegment->FindComponentByClass<USplineComponent>();
}
