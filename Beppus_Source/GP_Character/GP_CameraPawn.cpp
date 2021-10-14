// Fill out your copyright notice in the Description page of Project Settings.

#include "GP_CameraPawn.h"

#include "GP4/LevelStreaming/GP_RestartComp.h"
#include "CineCameraComponent.h"
#include "GP_AudioSystem.h"
#include "GP_PlayerController.h"
#include "Engine/LevelStreamingDynamic.h"
#include "GameFramework/SpringArmComponent.h"
#include "GP4/GP_GameStateBase.h"
#include "GP4/GP4.h"
#include "GP4/GP4GameModeBase.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AGP_CameraPawn::AGP_CameraPawn() {
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(SceneComponent);
	CameraComponent = CreateDefaultSubobject<UCineCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComp"));

	RestartComp = CreateDefaultSubobject<UGP_RestartComp>(TEXT("RestartComp"));
	AudioSystem = CreateDefaultSubobject<UGP_AudioSystem>(TEXT("AudioSystem"));
}

// Called when the game starts or when spawned
void AGP_CameraPawn::BeginPlay() {
	Super::BeginPlay();
	SpringArmComponent->TargetArmLength = DefaultCameraDistance;
	CameraDistance = DefaultCameraDistance;
	MasterMind = GetWorld()->GetGameState<AGP_GameStateBase>()->MasterMind;
	ensure(MasterMind!=nullptr);
	MasterMind->BeginCameraShakeDelegate.AddDynamic(this, &AGP_CameraPawn::ShakeCamera);
	GameMode = GetWorld()->GetAuthGameMode<AGP4GameModeBase>();
	// ensure(GameMode);
	// GameMode->OnLevelStartedUnloadingDelegate.AddDynamic(this, &AGP_CameraPawn::Reset);
}

void AGP_CameraPawn::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	MasterMind->BeginCameraShakeDelegate.RemoveDynamic(this, &AGP_CameraPawn::ShakeCamera);
	// GameMode->OnLevelStartedUnloadingDelegate.RemoveDynamic(this, &AGP_CameraPawn::Reset);
}

void AGP_CameraPawn::UnPossessedCameraMovement(float DeltaTime) {
	FVector Rotation = GetActorRotation().Euler();
	Rotation.Y = FMath::Lerp(Rotation.Y, 0.0f, CameraFollowMinionSpeed * DeltaTime);
	SetActorRotation(FRotator::MakeFromEuler(Rotation));
	CameraDistance += ZoomValueInput * ZoomStep;
	CameraDistance = FMath::Clamp(CameraDistance, ZoomMinDistance, ZoomMaxDistance);
	SpringArmComponent->TargetArmLength = FMath::Lerp(SpringArmComponent->TargetArmLength,
	                                                  CameraDistance,
	                                                  CameraZoomSpeed * DeltaTime);
	const FVector UpAcceleration = GetActorUpVector() * UpInput;
	const FVector RightAcceleration = GetActorRightVector() * RightInput;
	const FVector MoveAcceleration = (UpAcceleration + RightAcceleration).GetSafeNormal() * MaxAcceleration;
	const FVector MoveFriction = -MoveVelocity * FrictionCoefficient;
	MoveVelocity += (MoveAcceleration + MoveFriction) * DeltaTime;
	const FVector Position = GetActorLocation();
	if (Position.Y < BoundaryMinY) {
		MoveVelocity *= FVector(0.0f, 0.0f, 1.0f);
		SetActorLocation(FVector(0.0f, BoundaryMinY, Position.Z));
	}
	if (Position.Y > BoundaryMaxY) {
		MoveVelocity *= FVector(0.0f, 0.0f, 1.0f);
		SetActorLocation(FVector(0.0f, BoundaryMaxY, Position.Z));
	}
	if (Position.Z < BoundaryMinZ) {
		MoveVelocity *= FVector(0.0f, 1.0f, 0.0f);
		SetActorLocation(FVector(0.0f, Position.Y, BoundaryMinZ));
	}
	if (Position.Z > BoundaryMaxZ) {
		MoveVelocity *= FVector(0.0f, 1.0f, 0.0f);
		SetActorLocation(FVector(0.0f, Position.Y, BoundaryMaxZ));
	}
	AddActorWorldOffset(MoveVelocity);
}


void AGP_CameraPawn::MinionCameraMovement(float DeltaTime) {

	// -- Camera Distance -- //
	CameraDistance = CameraDistanceWhenPossess + CameraDistanceWhenPossessOffset;
	SpringArmComponent->TargetArmLength = FMath::Lerp(SpringArmComponent->TargetArmLength,
	                                                  CameraDistance,
	                                                  CameraZoomSpeed * DeltaTime);


	const FVector UpAcceleration = GetActorUpVector() * UpInput;

	if (UpAcceleration.Z < 0.0f && CameraOffsetZValueDelta > 0.0f) {
		CameraOffsetZValueDelta = 0.0f;
	}
	else if (UpAcceleration.Z > 0.0f && CameraOffsetZValueDelta < 0.0f) {
		CameraOffsetZValueDelta = 0.0f;
	}

	const FVector MoveAcceleration = (UpAcceleration).GetSafeNormal() * MaxAcceleration * 2.0f;
	const FVector MoveFriction = -CameraOffsetWhenPossessForced * FrictionCoefficient;
	CameraOffsetWhenPossessForced += (MoveAcceleration/* + MoveFriction*/) * DeltaTime;
	CameraOffsetZValueDelta += CameraOffsetWhenPossessForced.Z;
	CameraOffsetZValueDelta = FMath::Clamp(CameraOffsetZValueDelta, CameraSettings->CameraOffsetZMinusWhenPossessClamp,
	                                       CameraSettings->CameraOffsetZPlusWhenPossessClamp);

	if (UpAcceleration.IsNearlyZero() == true) {
		CameraOffsetWhenPossessForced.Z = 0.0f;

		CameraOffsetZValue = FMath::Lerp(CameraOffsetZValueDelta, 0.0f,
		                                 FMath::Abs(CameraOffsetZValueDelta) / FMath::Abs(CameraOffsetZValueDelta > 0
			                                 ? CameraSettings->
			                                 CameraOffsetZPlusWhenPossessClamp
			                                 : CameraSettings->
			                                 CameraOffsetZMinusWhenPossessClamp));


		if (FMath::IsNearlyZero(CameraOffsetZValue) == true) {
			CameraOffsetZValueDelta = 0.0f;
		}
	}
	else {
		CameraOffsetZValue = FMath::Lerp(0.0f, CameraOffsetZValueDelta,
		                                 FMath::Abs(CameraOffsetZValueDelta) / FMath::Abs(CameraOffsetZValueDelta > 0
			                                 ? CameraSettings->
			                                 CameraOffsetZPlusWhenPossessClamp
			                                 : CameraSettings->
			                                 CameraOffsetZMinusWhenPossessClamp));
	}


	// CameraOffsetZValue += CameraOffsetWhenPossessForced.Z;
	// CameraOffsetZValue = FMath::Clamp(CameraOffsetZValue, -CameraSettings->CameraOffsetZWhenPossessClamp,
	//                                   CameraSettings->CameraOffsetZWhenPossessClamp);

	// UE_LOG(LogTemp, Log, TEXT("CameraOffsetZValue %f"), CameraOffsetZValue);


	// -- Camera Location -- //
	const FVector CurrentLocation = GetActorLocation();
	FVector MinionLocation = MinionToFollow->GetActorLocation();
	const FVector MinionVelocity = (MinionLocation - PrevPosition) / DeltaTime;
	FrameVelocity = FMath::Lerp(FrameVelocity, MinionVelocity, 0.1f);
	PrevPosition = MinionLocation;
	if (MinionVelocity.Y > 100.0f) {
		CameraYOffset = FMath::Lerp(CameraYOffset,
		                            -CameraYOffsetWhenPossess + CameraLocationWhenPossessOffset.Y,
		                            CameraFollowMinionSpeedWhenMoving * DeltaTime);
	}
	else if (MinionVelocity.Y < -100.0f) {
		CameraYOffset = FMath::Lerp(CameraYOffset,
		                            CameraYOffsetWhenPossess + CameraLocationWhenPossessOffset.Y,
		                            CameraFollowMinionSpeedWhenMoving * DeltaTime);
	}
	else {
		CameraYOffset = FMath::Lerp(CameraYOffset, 0.0f + CameraLocationWhenPossessOffset.Y,
		                            CameraFollowMinionSpeedWhenMoving * DeltaTime);
	}

	if (MinionVelocity.Z > CameraOffsetFallVelocityThreshold) {
		CameraZOffset = FMath::Lerp(CameraZOffset,
		                            -CameraZOffsetWhenPossess + CameraOffsetWhenFalling + CameraOffsetZValue,
		                            CameraFollowMinionSpeedWhenFalling * DeltaTime);
	}
	else if (MinionVelocity.Z < -CameraOffsetFallVelocityThreshold) {
		CameraZOffset = FMath::Lerp(CameraZOffset,
		                            CameraZOffsetWhenPossess - CameraOffsetWhenFalling + CameraOffsetZValue,
		                            CameraFollowMinionSpeedWhenFalling * DeltaTime);
	}
	else {
		CameraZOffset = FMath::Lerp(CameraZOffset,
		                            CameraZOffsetWhenPossess + CameraOffsetZValue,
		                            CameraFollowMinionSpeedWhenFalling * DeltaTime);
	}

	MinionLocation.X = CurrentLocation.X;
	MinionLocation.Y = MinionLocation.Y - CameraYOffset;
	MinionLocation.Z = MinionLocation.Z + CameraZOffset;
	const FVector WishPosition = FMath::Lerp(CurrentLocation, MinionLocation, CameraFollowMinionSpeed * DeltaTime);

	// -- Camera Rotation -- //
	FVector CameraPawnRotation = GetActorRotation().Euler();

	CameraPawnRotation.Y = FMath::Lerp(CameraPawnRotation.Y, CameraYTiltWhenPossess,
	                                   CameraFollowMinionSpeed * DeltaTime);

	//Look at
	if (UseLookAtRotation == true) {
		const FRotator CameraRotation = CameraComponent->GetComponentRotation();
		const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(
			CameraComponent->GetComponentLocation(), CameraLookAtLocationWhenPossess);

		const FRotator RotationDelta = FMath::Lerp(CameraRotation, LookAtRotation,
		                                           CameraSettings->CameraRotateLookAtSpeed * DeltaTime);
		CameraComponent->SetWorldRotation(RotationDelta);
	}
	else {
		const FRotator CameraRotation = CameraComponent->GetComponentRotation();
		const FRotator RotationDelta = FMath::Lerp(CameraRotation, FRotator::MakeFromEuler(CameraPawnRotation),
		                                           CameraSettings->CameraRotateLookAtSpeedResetStillPossess * DeltaTime);
		CameraComponent->SetWorldRotation(RotationDelta);
	}

	// SetActorRotation(FRotator::MakeFromEuler(CameraPawnRotation));
	SetActorLocation(WishPosition);

}

// Called every frame
void AGP_CameraPawn::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	if (bIsResettingToPlayerStart) {
		if (!MoveToStart(DeltaTime)) {
			return;
		}
		bIsResettingToPlayerStart = false;
		return;
	}

	if (GetActorRotation().IsNearlyZero() == false) {
		SetActorRotation(FMath::Lerp(GetActorRotation(), FRotator::ZeroRotator, MoveToPlayerStartSpeed * DeltaTime));
	}


	if (IsPossessed == true) {
		UnPossessedCameraMovement(DeltaTime);
		if (CameraComponent->GetComponentRotation().IsNearlyZero() == false) {

			const FRotator CameraRotation = CameraComponent->GetComponentRotation();
			const FRotator RotationDelta = FMath::Lerp(CameraRotation, FRotator::ZeroRotator,
			                                           CameraSettings->CameraRotateLookAtSpeedReset * DeltaTime);
			CameraComponent->SetWorldRotation(RotationDelta);
		}
	}
	else if (MinionToFollow != nullptr) {
		MinionCameraMovement(DeltaTime);
	}

	ZoomDistanceNormalized = FMath::Lerp(
		0.0f, 1.0f, (CameraDistance - ZoomMinDistance) / (ZoomMaxDistance - ZoomMinDistance));
}

void AGP_CameraPawn::PossessedBy(AController* NewController) {
	Super::PossessedBy(NewController);
	PlayerController = Cast<AGP_PlayerController>(NewController);
	IsPossessed = true;
	if (bIsResettingToPlayerStart == false) {
		CameraDistance = DefaultCameraDistance;
	}
	MinionToFollow = nullptr;
	OnUnPossessMinion();
}

void AGP_CameraPawn::UnPossessed() {
	IsPossessed = false;
	OnUnPossessMinion();
}

// void AGP_CameraPawn::Reset() {
// 	OnUnPossessMinion();
// 	// ensure(LevelResetShakeClass!=nullptr);
// 	ShakeCamera(LevelResetShakeClass);
// 	MoveToPlayerStart();
//
//
// 	
// 	//SetActorLocation(PlayerController->LevelData->PlayerStartPosition->GetActorLocation());
// }

void AGP_CameraPawn::HandleUpInput(const float Value) {
	UpInput = Value;
}

void AGP_CameraPawn::HandleRightInput(const float Value) {
	RightInput = Value;
}

void AGP_CameraPawn::HandleZoomInput(const float Value) {
	ZoomValueInput = Value;
}

void AGP_CameraPawn::FollowMinion(AGP_Pawn_Base* PawnToFollow) {
	MinionToFollow = PawnToFollow;
	IsPossessed = false;
	OnPossessMinion();
}

void AGP_CameraPawn::ShakeCamera(TSubclassOf<UMatineeCameraShake> ShakeClass) {
	//	LOG("CameraShake on CameraPawn");
	UMatineeCameraShake* CurrentShake = PlayerController->PlayerCameraManager->StartMatineeCameraShake(ShakeClass);

	OnCameraShake();
	GetWorldTimerManager().SetTimer(InOutHandle,
	                                this,
	                                &AGP_CameraPawn::OnCameraShakeComplete,
	                                CurrentShake->OscillationDuration,
	                                false,
	                                -1.f);
}

void AGP_CameraPawn::SetLevelInstanceData(AGP_LevelInstanceData* LevelInstanceData) {
	ZoomMaxDistance = LevelInstanceData->ZoomOutAmount;
	BoundaryMinY = LevelInstanceData->LevelCameraBounds.TopLeft.Y; // CameraBounds.X;
	BoundaryMaxZ = LevelInstanceData->LevelCameraBounds.TopLeft.Z; // CameraBounds.W;
	BoundaryMaxY = LevelInstanceData->LevelCameraBounds.BottomRight.Y; //CameraBounds.Y;
	BoundaryMinZ = LevelInstanceData->LevelCameraBounds.BottomRight.Z; //CameraBounds.Z;
	LevelStartLocation = LevelInstanceData->GetPlayerStartTransform();
}

void AGP_CameraPawn::PopulateSpline(const FTransform& Transform) const {
	const FVector StartPoint = GetActorLocation();
	const FVector EndPoint = Transform.GetLocation();
	const float midOffset = FVector::Dist(StartPoint, EndPoint) / 2;
	FVector MidPoint;
	MidPoint.X = -midOffset;
	MidPoint.Y = (StartPoint.Y + EndPoint.Y) / 2.f;
	MidPoint.Z = (StartPoint.Z + EndPoint.Z) / 2.f;
	FVector MidPointInTangent;
	MidPointInTangent.X = MidPoint.X;
	MidPointInTangent.Y = StartPoint.Y;
	MidPointInTangent.Z = MidPoint.Z;
	FVector MidPointOutTangent;
	MidPointOutTangent.X = MidPoint.X;
	MidPointOutTangent.Y = EndPoint.Y;
	MidPointOutTangent.Z = MidPoint.Z;
	const FSplinePoint Start = {0, StartPoint, ESplinePointType::Curve, FRotator::ZeroRotator};
	FSplinePoint Mid = {1, MidPoint, ESplinePointType::Curve, FRotator::ZeroRotator};
	Mid.ArriveTangent = MidPointInTangent;
	Mid.LeaveTangent = MidPointOutTangent;
	const FSplinePoint End = {2, EndPoint, ESplinePointType::Curve, FRotator::ZeroRotator};
	SplineComponent->AddPoint(Start, false);
	SplineComponent->AddPoint(Mid, false);
	SplineComponent->AddPoint(End, false);
	SplineComponent->ReparamStepsPerSegment = 64;
	SplineComponent->UpdateSpline();
	SplineComponent->SplineCurves.UpdateSpline();
}

void AGP_CameraPawn::MoveToPlayerStart() {
	//TODO CompareLevelIndex from levelSpawner instead;
	if (GameMode->bIsResettingLevel) {
		OnMoveToResetPosition();
		// LOG("Instanced and Previous is the same")
	}
	else {
		OnMoveToNewLevel();
		SplineComponent->Duration = 3.f;
		// LOG("Instanced and Previous is not the same")
	}
	t = 0.0f;
	TargetTransform = LevelStartLocation;
	OnUnPossessMinion();
	SplineComponent->ClearSplinePoints(true);
	//SplineComponent->Duration = 1.f;
	SplineComponent->bDrawDebug = false;
	PopulateSpline(TargetTransform);
	bIsResettingToPlayerStart = true;

	//Before entering reset of camera animation, we calculate how far we zoom out.
	const float Distance = FVector::Distance(GetActorLocation(), LevelStartLocation.GetLocation()) / 5.0f;
	MidCameraDistance = FMath::Clamp(CameraSettings->MidPointDistance + Distance, CameraSettings->ClampMinDistance,
	                                 CameraSettings->ClampMaxDistance);
}

void AGP_CameraPawn::SetCameraDistanceWhenPossessingOffset(const float OffsetAmount) {
	CameraDistanceWhenPossessOffset = OffsetAmount;
}

void AGP_CameraPawn::SetCameraLocationWhenPossessingOffset(FVector OffsetAmount, FVector LookAtLocation,
                                                           bool UseLookAt) {
	CameraLocationWhenPossessOffset = OffsetAmount;
	CameraLookAtLocationWhenPossess = LookAtLocation;
	UseLookAtRotation = UseLookAt;
}

bool AGP_CameraPawn::MoveToStart(float DeltaTime) {
	if (GameMode->bIsResettingLevel == true) {
		return MoveToResetPosition(DeltaTime);
	}

	const float PreviousT = t;
	t += SplineComponent->GetSplineLength() / SplineComponent->Duration * MoveToPlayerStartSpeed * DeltaTime;
	const float p = FMath::Lerp(PreviousT, t, DeltaTime);
	const FVector Location = SplineComponent->GetWorldLocationAtDistanceAlongSpline(p);
	// UE_LOG(LogTemp, Log, TEXT("p: %f"), p)
	FRotator RotationTarget = p < SplineComponent->GetSplineLength() * 0.33f
		                          ? FRotator::MakeFromEuler(FVector(0, 0, -FMath::Clamp((p * 0.01f), -45.f, 45.f)))
		                          : FRotator::MakeFromEuler(FVector(0, 0, 0));


	// UE_LOG(LogTemp, Log, TEXT("p: %s"), *RotationTarget.ToString())
	//	FRotator DeltaRotation = FMath::Lerp(GetActorRotation(), RotationTarget, DeltaTime);
	//	SetActorRotation(DeltaRotation);
	SetActorLocation(Location);
	if (p >= SplineComponent->GetSplineLength()) {
		if (GetActorRotation().IsNearlyZero()) {
			SetActorRotation(FRotator::ZeroRotator);
		}
		GameMode->HandleCameraPawnTravelComplete();
		OnCameraPawnTravelComplete();
		SplineComponent->ClearSplinePoints();
		return true;
	}
	return false;
}

bool AGP_CameraPawn::MoveToResetPosition(const float DeltaTime) {
	if (StartCameraMovementResetting == false) {
		LerpStartPosition = GetActorLocation();
		CameraDistanceStartPosition = CameraDistance;
		StartCameraMovementResetting = true;
	}

	// -- Set Camera Position -- //
	//Setting Camera start and end Positions
	const FVector StartLocation = LerpStartPosition;
	const FVector EndLocation = LevelStartLocation.GetLocation();

	CameraPositionLerpTimer += DeltaTime;

	//Calculate Alpha for lerp
	const float CameraPositionAlpha = CameraPositionLerpTimer / CameraSettings->MovementLerpTime + 0.00001f;

	//Lerp start position and end position
	const FVector LocationAlongLerp = FMath::InterpEaseInOut(StartLocation, EndLocation, CameraPositionAlpha,
	                                                         CameraSettings->MovementExpo);

	//Set the position
	SetActorLocation(LocationAlongLerp);


	// -- Set Camera Boomstick Distance -- //

	float StartCameraDistance = CameraDistanceStartPosition;
	const float EndCameraDistance = DefaultCameraDistance;

	//Using two lerps to get a zoom out and in effect.
	TotalCameraDistanceLerpTimer += DeltaTime;
	CameraDistanceLerpTimer += DeltaTime;

	//Calculate Alpha for lerp
	const float DistanceAlpha = TotalCameraDistanceLerpTimer / CameraSettings->DistanceLerpTime + 0.00001f;

	//If we have lerped more than 50%, we switch over to zoom in to default zoom distance.
	if (DistanceAlpha > 0.5f) {
		UseCameraDistanceLerpTimer = true;
		if (HasPassedHalfZoomTime == false) {
			CameraDistanceStartPosition = CameraDistance;
			StartCameraDistance = CameraDistance;
			HasPassedHalfZoomTime = true;
			CameraDistanceLerpTimer = 0.0f;
		}
	}

	//This lerp is essentially multiplied by 2 to compensate for the time needed
	// to switch between zoom in and out before the camera location has reached its goal 
	const float HalfDistanceAlpha = CameraDistanceLerpTimer / CameraSettings->DistanceLerpTime * CameraSettings->
		LerpTimeMultiplier + 0.00001f;

	//Setting the correct lerp distances and calculates the lerp alpha.
	float UpdateCameraDistance;
	if (DistanceAlpha < 0.5f) {
		UpdateCameraDistance = FMath::InterpSinInOut(StartCameraDistance, MidCameraDistance, HalfDistanceAlpha);
	}
	else {
		UpdateCameraDistance = FMath::InterpSinInOut(StartCameraDistance, EndCameraDistance, HalfDistanceAlpha);
	}

	//Set camera boom distance	
	CameraDistance = UpdateCameraDistance;
	SpringArmComponent->TargetArmLength = CameraDistance;

	//Have we reached the goal?
	if (CameraPositionAlpha >= 1.0f) {
		GameMode->HandleCameraPawnTravelComplete();
		CameraPositionLerpTimer = 0.0f;
		CameraDistanceLerpTimer = 0.0f;
		TotalCameraDistanceLerpTimer = 0.0f;
		UseCameraDistanceLerpTimer = false;
		HasPassedHalfZoomTime = false;
		StartCameraMovementResetting = false;
		return true;
	}

	return false;
}
