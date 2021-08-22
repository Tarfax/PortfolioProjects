#include "AILifeGuardTask_HasSpawned.h"

#include "Components/CapsuleComponent.h"
// #include "Kismet/KismetMathLibrary.h"
// #include "Kismet/KismetSystemLibrary.h"
#include "Team9Assemble/AI/AI_BeachCharacter.h"

void AILifeGuardTask_HasSpawned::Enter() {
	Owner->AI_BeachCharacter->GetCapsuleComponent()->SetEnableGravity(false);
}

void AILifeGuardTask_HasSpawned::OnTick(float DeltaTime) {
	if (Owner->IsPlayerControlled == false) {
		if (HasMadeDecision == false) {
			Owner->MakeDecision();
			HasMadeDecision = true;
		}
		// FVector Position;
		// FVector Direction;
		// Owner->GetWorld()->GetFirstPlayerController()->DeprojectMousePositionToWorld(Position, Direction);
		// FHitResult HitResult;
		// ECollisionChannel CollisionChannel = ECC_GameTraceChannel3;
		// Owner->GetWorld()->LineTraceSingleByChannel(HitResult, Position, Position + (Direction * 40000),
		//                                             CollisionChannel);
		//
		// const float LockedHeight = 400.f;
		// const float CamToCloseThreshold = 3000.f;
		//
		// const FVector HitLocation = HitResult.ImpactPoint;
		// const FVector StartLocation = HitResult.TraceStart;
		// const float DistToCam = (StartLocation - HitLocation).Size();
		//
		// // Calculate the angle of the -Direction vector, which points from the Hit.ImpactPoint back towards the camera.
		// FVector VecA = FVector::UpVector;
		// FVector VecB = -Direction.GetSafeNormal();
		// const float AngA = acosf((VecA.X * VecB.X + VecA.Y * VecB.Y + VecA.Z * VecB.Z) /
		// 	(sqrtf(powf(VecA.X, 2) + powf(VecA.Y, 2) + powf(VecA.Z, 2)) *
		// 		sqrtf(powf(VecB.X, 2) + powf(VecB.Y, 2) + powf(VecB.Z, 2))));
		//
		// const float AngB = 1.570796f; // 90 degrees in radians.
		// const float AngC = UKismetMathLibrary::GetPI() - AngA - AngB; // pi = 180 degrees in radians.
		//
		// const float LengthAlongGround = LockedHeight * (sinf(AngA) / sinf(AngC));
		// const float LAG = DistToCam < CamToCloseThreshold ? 0.f : LengthAlongGround;
		//
		// const FVector DirToCamXY = ((StartLocation * FVector(1.f, 1.f, 0.f)) -
		// 	(HitLocation * FVector(1.f, 1.f, 0.f))).GetSafeNormal();
		//
		// const FVector HoldActorOffset = FVector(0.f, 0.f, -200.f);
		//
		// const FVector HoverLocation = HitLocation + FVector(LAG * DirToCamXY.X,
		//                                                     LAG * DirToCamXY.Y, LockedHeight) + HoldActorOffset;
		//
		// const FVector LerpLocation = UKismetMathLibrary::VLerp(
		// 	Owner->AI_BeachCharacter->GetActorLocation(), HoverLocation, .4f);
		//
		// // Only move the position of the Lifeguard if inside beach area.
		// if (HitResult.bBlockingHit) {
		// 	Owner->AI_BeachCharacter->SetActorLocation(LerpLocation);
		// }

		// Debug
		//UKismetSystemLibrary::DrawDebugLine(Owner, HitLocation, HitLocation + (-Direction * LengthToMaxHeight),
		//	FLinearColor::Red, 0.f, 10.f);
		//UKismetSystemLibrary::DrawDebugSphere(Owner, HitLocation + (-Direction * LengthToMaxHeight), 50.f, 12,
		//	FLinearColor::Red,0.f,5.f);
		//UKismetSystemLibrary::DrawDebugLine(Owner, HitLocation, HitLocation + FVector(0.f, 0.f, MaxHeight),
		//	FLinearColor::Blue, 0.f, 10.f);
		//
		//const FVector AngleInfo = FVector(AngA, AngB, AngC);
		//GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, AngleInfo.ToString());
		//
		//GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, FString::SanitizeFloat(CameraAngle));
	}
	// else {
	// 	// FVector ActorLocation = Owner->AI_BeachCharacter->GetActorLocation();
	// 	// if (ActorLocation.Z < 100) {
	// 	// 	ActorLocation.Z = 100;
	// 	// 	Owner->AI_BeachCharacter->SetActorLocation(ActorLocation);
	// 	// }
	// 	//
	// 	
	// 		// UE_LOG(LogTemp, Log, TEXT("HasSpawnedDecision"));
	//
	// 	
	// }
}

void AILifeGuardTask_HasSpawned::Exit() {
	Owner->AI_BeachCharacter->GetCapsuleComponent()->SetEnableGravity(true);
	Owner->CanBePickedUp = true;
	// UE_LOG(LogTemp, Log, TEXT("HasSpawned::Exit"));

}

void AILifeGuardTask_HasSpawned::Reset() {}
