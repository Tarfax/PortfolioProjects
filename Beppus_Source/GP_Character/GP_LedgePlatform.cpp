// Fill out your copyright notice in the Description page of Project Settings.


#include "GP_LedgePlatform.h"

#include "Components/CapsuleComponent.h"
#include "Components/SplineComponent.h"

// Sets default values
AGP_LedgePlatform::AGP_LedgePlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	LedgeSpline = CreateDefaultSubobject<USplineComponent>(TEXT("LedgeSpline"));
	LedgePoint = CreateDefaultSubobject<USceneComponent>(TEXT("LedgePoint"));

}

void AGP_LedgePlatform::BeginLedgeClimb(AGP_Minion* Minion)
{
	StartPos = LedgeSpline->FindLocationClosestToWorldLocation(	Minion->GetActorLocation(),
		ESplineCoordinateSpace::World);

	CourseCorrectionDistance = FVector::Dist(StartPos, Minion->GetActorLocation());
		
	Minion->IsInputEnabled = false;
	Minion->IsMovementEnabled = false;
	Minion->Velocity = FVector2D::ZeroVector;
	CourseCorrection = Minion->GetActorLocation();

	CurrentClimbTime = LedgeSpline->FindInputKeyClosestToWorldLocation(Minion->GetActorLocation()) /
		LedgeSpline->GetNumberOfSplinePoints();
	
	MinionToClimb = Minion;
	IsClimbing = true;
	IsMovingOntoSpline = true;
	ClimbSpeed = 1 / Minion->ClimbDuration;
}

AGP_LedgePlatform* AGP_LedgePlatform::CheckForPossibleClimb(AGP_Minion* Minion)
{
	// FVector MinionLocation = Minion->GetActorLocation();
	// FVector LedgeLocation = LedgePoint->GetComponentLocation();
	//
	// const float UpperBound = MinionLocation.Z + Minion->CapsuleComponent->GetScaledCapsuleHalfHeight() +
	// 	Minion->ClimbRange.Y;
	// const float LowerBound = MinionLocation.Z - Minion->CapsuleComponent->GetScaledCapsuleHalfHeight();
	//
	// if(LedgeLocation.Z < UpperBound && LedgeLocation.Z > LowerBound)
	// {
	// 	if(FMath::Abs(MinionLocation.Y - LedgeLocation.Y) < Minion->ClimbRange.X)
	// 	{
	// 		return this;
	// 	}
	// }
	return nullptr;
}

// Called when the game starts or when spawned
void AGP_LedgePlatform::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGP_LedgePlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	if(!IsClimbing) return;
		
		if(IsMovingOntoSpline)
		{
			// FVector MinionHalfHeight = FVector(0,0,MinionToClimb->CapsuleComponent->GetScaledCapsuleHalfHeight());
   //
			// FVector DeltaToMove = ((StartPos + MinionHalfHeight) - (CourseCorrection + MinionHalfHeight)).GetSafeNormal() * DeltaTime *
   //              MinionToClimb->ClimbCorrectionSpeed;
		 //
			// CourseCorrection += DeltaToMove;
		 //
			// if(FVector::DistSquared(StartPos, CourseCorrection) <= FMath::Square(DeltaTime *
   //              MinionToClimb->ClimbCorrectionSpeed))
			// {
			// 	IsMovingOntoSpline = false;
   //
			// }
		}
		
		CurrentClimbTime += ClimbSpeed * DeltaTime;
		if(CurrentClimbTime > LedgeSpline->Duration)
		{
			IsClimbing = false;
			MinionToClimb->IsInputEnabled = true;
			MinionToClimb->IsMovementEnabled = true;
			return;
		}
		
		if(MinionToClimb == nullptr) return;

	//FVector SplinePos = LedgeSpline->GetLocationAtTime(CurrentClimbTime, ESplineCoordinateSpace::World) +
            //FVector(0,0,MinionToClimb->CapsuleComponent->GetScaledCapsuleHalfHeight());

	//FVector MoveTo = FMath::Lerp(CourseCorrection, SplinePos,
		//1 - FVector::Dist(CourseCorrection, StartPos) / CourseCorrectionDistance);

		UE_LOG(LogTemp, Log, TEXT(" Course Correction(%f, %f, %f)"),
			CourseCorrection.X, CourseCorrection.Y, CourseCorrection.Z);
		//UE_LOG(LogTemp, Log, TEXT(" Spline Pos(%f, %f, %f)"),
			//SplinePos.X, SplinePos.Y, SplinePos.Z);
	
		//MinionToClimb->SetActorLocation(MoveTo);
}