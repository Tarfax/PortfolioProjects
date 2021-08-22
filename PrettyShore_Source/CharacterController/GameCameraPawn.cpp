#include "GameCameraPawn.h"

// #include <Actor.h>

#include "BeachPlayerController.h"
// #include "Blueprint/WidgetBlueprintLibrary.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
// #include "Kismet/KismetSystemLibrary.h"

AGameCameraPawn::AGameCameraPawn() {
	PrimaryActorTick.bTickEvenWhenPaused = true;
	PrimaryActorTick.bCanEverTick = true;
	CameraAnchorComp = CreateDefaultSubobject<USceneComponent>(TEXT("CameraAnchor"));
	RootComponent = CameraAnchorComp;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(CameraAnchorComp);
}

void AGameCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AGameCameraPawn::HandleMoveForwardInput);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AGameCameraPawn::HandleMoveRightInput);
	PlayerInputComponent->BindAxis(TEXT("Zoom"), this, &AGameCameraPawn::HandleZoomInput);
	PlayerInputComponent->BindAxis(TEXT("RotateRight"), this, &AGameCameraPawn::HandleRotateRightInput);
	PlayerInputComponent->BindAxis(TEXT("PanCamera"), this, &AGameCameraPawn::HandleTiltCameraInput);
	PlayerInputComponent->BindAction(TEXT("MiddleMouseButton"), IE_Pressed, this,
	                                 &AGameCameraPawn::HandleMiddleMouseInputPressed);
	PlayerInputComponent->BindAction(TEXT("MiddleMouseButton"), IE_Released, this,
	                                 &AGameCameraPawn::HandleMiddleMouseInputReleased);
	PlayerInputComponent->BindAction(TEXT("SetTimeSpeed_1"), IE_Pressed, this,
	                                 &AGameCameraPawn::HandleSetTimeSpeed_1Input);
	PlayerInputComponent->BindAction(TEXT("SetTimeSpeed_2"), IE_Pressed, this,
	                                 &AGameCameraPawn::HandleSetTimeSpeed_2Input);
	PlayerInputComponent->BindAction(TEXT("SetTimeSpeed_3"), IE_Pressed, this,
	                                 &AGameCameraPawn::HandleSetTimeSpeed_3Input);
	PlayerInputComponent->BindAction(TEXT("SetTimeSpeed_4"), IE_Pressed, this,
	                                 &AGameCameraPawn::HandleSetTimeSpeed_4Input);
	PlayerInputComponent->BindAction(TEXT("SetTimeSpeed_Pause"), IE_Pressed, this,
	                                 &AGameCameraPawn::HandleSetTimeSpeed_PauseInput);

}

void AGameCameraPawn::BeginPlay() {
	Super::BeginPlay();

	const FVector ZeroZAxis(1.f, 1.f, 0.f);
	SetActorLocation(GetActorLocation() * ZeroZAxis);
}

void AGameCameraPawn::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
	// Tick safeguard. Makes sure the camera won't spin out of control when pausing the game.
	if (DeltaTime > Max_Tick_Time)
		return;

#pragma region Setup

	CameraDistance += CameraDistanceInput * ZoomStep;
	CameraDistance = FMath::Clamp(CameraDistance, MinZoomDistance, MaxZoomDistance);
	const float CameraDistanceNormalized = GetNormalized(MinZoomDistance, MaxZoomDistance, CameraDistance);

	//Calculating allowed camera tilt based on CameraDistance.
	const float MaxTiltAngleNormalizedRange = ILerpClamped(MinZoomDistance + MaxAngleDistanceThresholdStart,
	                                                       MaxZoomDistance - MaxAngleDistanceThresholdEnd,
	                                                       CameraDistance);
	const float MaxTiltAngle = FMath::Lerp(MaxAngleMinZoom, MaxAngleMaxZoom, MaxTiltAngleNormalizedRange);

	const float MinTiltAngleNormalizedRange = ILerpClamped(MinZoomDistance + MinAngleDistanceThresholdStart,
	                                                       MaxZoomDistance - MinAngleDistanceThresholdEnd,
	                                                       CameraDistance);
	const float MinTiltAngle = FMath::Lerp(MinAngleMinZoom, MinAngleMaxZoom, MinTiltAngleNormalizedRange);

	FVector2D MouseDelta(0.f, 0.f);
	if (bMiddleMouseHeld) {
		auto* BeachController = Cast<ABeachPlayerController>(
			UGameplayStatics::GetPlayerController(this, 0));

		if (BeachController)
			MouseDelta = BeachController->RetrieveMouseDelta();
	}

	CameraTiltInput += -MouseDelta.Y * CameraTiltMouseSensitivity;
	const float TiltAngleStep = FMath::Lerp(TiltAngleStepMinZoom, TiltAngleStepMaxZoom, CameraDistanceNormalized);
	CameraTiltNormalizedRange += ((CameraTiltInput * TiltAngleStep) * DeltaTime) * .01f;
	CameraTiltNormalizedRange = FMath::Clamp(CameraTiltNormalizedRange, 0.f, 1.f);
	AppliedCameraTiltAngleDeg = FMath::Lerp(MinTiltAngle, MaxTiltAngle, CameraTiltNormalizedRange);

	// Calculating camera movement velocity based on CameraDistance.
	MaxAcceleration = FMath::Lerp(AccelerationMinZoom, AccelerationMaxZoom, CameraDistanceNormalized);
#pragma endregion Setup

	//todo: Make the base position follow the terrain. 
#pragma region CameraBaseMovement
	// Camera base position
	if (bFollowTargetedActor == false)
	{
		const FVector MoveForwardAcceleration = GetActorForwardVector() * MoveForwardInput;
		const FVector MoveRightAcceleration = GetActorRightVector() * MoveRightInput;
		const FVector MoveAcceleration = (MoveForwardAcceleration + MoveRightAcceleration).GetSafeNormal() *
			MaxAcceleration;

		const FVector MoveFriction = -MoveVelocity * FrictionCoefficient;

		MoveVelocity += (MoveAcceleration + MoveFriction) * DeltaTime;

		const FVector Pos = GetActorLocation();
		if (Pos.X < CameraWorldBoundary_Min_X) {
			MoveVelocity *= FVector(0.f, 1.f, 1.f);
			SetActorLocation(FVector(CameraWorldBoundary_Min_X, Pos.Y, Pos.Z));
		}
		if (Pos.X > CameraWorldBoundary_Max_X) {
			MoveVelocity *= FVector(0.f, 1.f, 1.f);
			SetActorLocation(FVector(CameraWorldBoundary_Max_X, Pos.Y, Pos.Z));
		}
		if (Pos.Y < CameraWorldBoundary_Min_Y) {
			MoveVelocity *= FVector(1.f, 0.f, 1.f);
			SetActorLocation(FVector(Pos.X, CameraWorldBoundary_Min_Y, Pos.Z));
		}
		if (Pos.Y > CameraWorldBoundary_Max_Y) {
			MoveVelocity *= FVector(1.f, 0.f, 1.f);
			SetActorLocation(FVector(Pos.X, CameraWorldBoundary_Max_Y, Pos.Z));
		}
		
		AddActorWorldOffset(MoveVelocity);
		//UE_LOG(LogTemp, Log, TEXT("GameCamera::NormalTick!"))
	}
	else if (TargetedActor != nullptr)
	{
		SetActorLocation(TargetedActor->GetActorLocation());
		//UE_LOG(LogTemp, Log, TEXT("GameCamera::FollowTargetTick!"))
	}
	else
	{
		bFollowTargetedActor = false;
		//UE_LOG(LogTemp, Log, TEXT("GameCamera::Going back to NormalTick! <---"))
	}

	// Camera base rotation
	RotateRightInput += MouseDelta.X * RotationMouseSensitivity;
	const float AngularAcceleration = RotateRightInput * MaxAngularAcceleration;

	const float AngularFriction = -AngularVelocity.Yaw * AngularFrictionCoefficient;

	AngularVelocity += FRotator(0.f, (AngularAcceleration + AngularFriction) * DeltaTime, 0.f);

	AddActorWorldRotation(AngularVelocity);

#pragma endregion CameraBaseMovement

	//todo: Make the camera go up in case of building.
	//todo: Make camera look up when close to ground at low angle.
#pragma region CameraBoomArmManipulation
	// Camera position	
	const float AngRad = UKismetMathLibrary::DegreesToRadians(AppliedCameraTiltAngleDeg);
	const float X = cosf(AngRad);
	const float Y = sinf(AngRad);

	const FVector CameraWishPos = FVector(-X, 0.f, Y) * CameraDistance;

	FVector CameraCompWorldPos = CameraComp->GetComponentLocation();
	//	FVector TraceStart(CameraCompWorldPos.X, CameraCompWorldPos.Y, 10000.f);
	//	FVector TraceEnd(CameraCompWorldPos.X, CameraCompWorldPos.Y, -10000.f);
	//	FHitResult Hit;
	//	UKismetSystemLibrary::SphereTraceSingle(this, TraceStart, TraceEnd,200.f, TraceType,
	//		false,TArray<AActor*>(), EDrawDebugTrace::ForOneFrame, Hit, true);
	//
	//	FVector CameraTargetPos = CameraWishPos;
	//	FVector AdjustedLookTargetPos(0.f);
	//	if (Hit.bBlockingHit)
	//	{
	//		FVector DistToHit = Hit.Location - CameraCompWorldPos;
	//		CameraTargetPos += DistToHit + (FVector::UpVector * 2000.f);
	//		AdjustedLookTargetPos = DistToHit + (FVector::UpVector * 2000.f);
	//	}

	CameraLocalPos = UKismetMathLibrary::VLerp(CameraLocalPos, CameraWishPos, CameraResponsiveness * DeltaTime);
	CameraComp->SetRelativeLocation(CameraLocalPos);

	//	FVector LerpedAdjustedLookTargetPos;
	//	LerpedAdjustedLookTargetPos = UKismetMathLibrary::VLerp(LerpedAdjustedLookTargetPos, AdjustedLookTargetPos, CameraResponsiveness * DeltaTime);

	// Camera look rotation
	//	const FVector DirToTargetLocal = FVector((GetActorLocation() + LerpedAdjustedLookTargetPos) - CameraCompWorldPos).GetSafeNormal();
	const FVector DirToTargetLocal = -CameraLocalPos.GetSafeNormal();
	const float AngDegToTarget = UKismetMathLibrary::RadiansToDegrees(
		UKismetMathLibrary::Atan2(DirToTargetLocal.Z, DirToTargetLocal.X));

	const FRotator RotationToTarget = FRotator(AngDegToTarget, 0.f, 0.f);

	CameraComp->SetRelativeRotation(RotationToTarget);

#pragma endregion CameraBoomArmManipulation

#if WITH_EDITORONLY_DATA
	if (bDebug) {
		//FVector DebugCameraWishPos = GetActorLocation() + CameraWishPos;
		//UKismetSystemLibrary::DrawDebugSphere(this, DebugCameraWishPos, 100.f, 12,
		//                                      FLinearColor::Red, 0.f, 10.f);

		// UKismetSystemLibrary::DrawDebugSphere(this, CameraPos, 100.f, 12,
		// 	FLinearColor::Green, 0.f, 10.f);

		//UKismetSystemLibrary::DrawDebugSphere(this, CameraComp->GetComponentLocation(), 100.f, 12,
		//                                      FLinearColor::Blue, 0.f, 1.f);

		//FVector DebugCameraPos = GetActorLocation() + CameraLocalPos;
		//FVector bojs = DebugCameraPos + RotationToTarget.Vector() * 10000.f;
		//UKismetSystemLibrary::DrawDebugLine(this, CameraCompWorldPos, CameraCompWorldPos + DirToTargetLocal * 100000.f,
		//                                    FLinearColor::Blue, 0.f, 10.f);
		const FVector LowRightCorner = FVector(CameraWorldBoundary_Min_X, CameraWorldBoundary_Max_Y, 100.f);
		const FVector UpRightCorner = FVector(CameraWorldBoundary_Max_X, CameraWorldBoundary_Max_Y, 100.f);
		const FVector UpLeftCorner = FVector(CameraWorldBoundary_Max_X, CameraWorldBoundary_Min_Y, 100.f);
		const FVector LowLeftCorner = FVector(CameraWorldBoundary_Min_X, CameraWorldBoundary_Min_Y, 100.f);

		UKismetSystemLibrary::DrawDebugLine(this, LowRightCorner,
		                                    UpRightCorner, FLinearColor::Red, 0.f, 20.f);
		UKismetSystemLibrary::DrawDebugLine(this, UpRightCorner,
		                                    UpLeftCorner, FLinearColor::Red, 0.f, 20.f);
		UKismetSystemLibrary::DrawDebugLine(this, UpLeftCorner,
		                                    LowLeftCorner, FLinearColor::Red, 0.f, 20.f);
		UKismetSystemLibrary::DrawDebugLine(this, LowLeftCorner,
		                                    LowRightCorner, FLinearColor::Red, 0.f, 20.f);
	}
#endif

}

void AGameCameraPawn::FollowTargetActor(AActor* TargetActor)
{
	TargetedActor = TargetActor;
	bFollowTargetedActor = true;

	//UE_LOG(LogTemp, Log, TEXT("AGameCameraPawn::FollowTargetActor was called! <---"))
}

#pragma region InputFunctions
void AGameCameraPawn::HandleMoveForwardInput(const float Value) {
	MoveForwardInput = Value;
	if (bFollowTargetedActor == true && Value != 0.f)
	    bFollowTargetedActor = false;
	//UE_LOG(LogTemp, Log, TEXT("AGameCameraPawn::HandleMoveForwardInput: bFollowTargetActor = %s"), bFollowTargetedActor ? TEXT("True") : TEXT("False"))
}

void AGameCameraPawn::HandleMoveRightInput(const float Value) {
	MoveRightInput = Value;
	if (bFollowTargetedActor == true && Value != 0.f)
    	bFollowTargetedActor = false;
	//UE_LOG(LogTemp, Log, TEXT("AGameCameraPawn::HandleMoveRightInput: bFollowTargetActor = %s"), bFollowTargetedActor ? TEXT("True") : TEXT("False"))
}

void AGameCameraPawn::HandleRotateRightInput(const float Value) {
	RotateRightInput = -Value;
}

void AGameCameraPawn::HandleZoomInput(const float Value) {
	CameraDistanceInput = -Value;
}

void AGameCameraPawn::HandleTiltCameraInput(const float Value) {
	CameraTiltInput = Value;
}

void AGameCameraPawn::HandleMiddleMouseInputPressed() {
	bMiddleMouseHeld = true;

	auto* BeachController = Cast<ABeachPlayerController>(
		UGameplayStatics::GetPlayerController(this, 0));

	if (BeachController) {
		BeachController->SetLockHideMouseCursor(true);

		//This works
		BeachController->SetShowMouseCursor(false);
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
		InputMode.SetHideCursorDuringCapture(true);
		BeachController->SetInputMode(InputMode);
	}
}

void AGameCameraPawn::HandleMiddleMouseInputReleased() {
	bMiddleMouseHeld = false;

	auto* BeachController = Cast<ABeachPlayerController>(
		UGameplayStatics::GetPlayerController(this, 0));

	if (BeachController) {
		BeachController->SetLockHideMouseCursor(false);

		//This works to get the UI working again.
		BeachController->SetShowMouseCursor(true);
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false);
		BeachController->SetInputMode(InputMode);
	}
}

void AGameCameraPawn::HandleSetTimeSpeed_1Input() {
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
	this->CustomTimeDilation = 1.f;
	IsPaused = false;
	TimeScale = 1.0f;
}

void AGameCameraPawn::HandleSetTimeSpeed_2Input() {
	const float TimeDilation = 2.f;
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TimeDilation);
	this->CustomTimeDilation = 1 / TimeDilation;
	IsPaused = false;
	TimeScale = TimeDilation;
}

void AGameCameraPawn::HandleSetTimeSpeed_3Input() {
	const float TimeDilation = 3.f;
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TimeDilation);
	this->CustomTimeDilation = 1 / TimeDilation;
	IsPaused = false;
	TimeScale = TimeDilation;
}

void AGameCameraPawn::HandleSetTimeSpeed_4Input() {
	const float TimeDilation = 5.f;
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TimeDilation);
	this->CustomTimeDilation = 1 / TimeDilation;
	IsPaused = false;
	TimeScale = TimeDilation;
}

void AGameCameraPawn::HandleSetTimeSpeed_PauseInput() {
	if (IsPaused == true) {

		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TimeScale);
		this->CustomTimeDilation = 1 / TimeScale;
		IsPaused = false;
	}
	else {
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.001f);
		this->CustomTimeDilation = 1 / 0.001f;
		IsPaused = true;
	}
}
#pragma endregion InputFunctions

float AGameCameraPawn::RemapValue(const float From1, const float To1, const float From2, const float To2,
                                  const float Value) {
	return From2 + (Value - From1) * (To2 - From2) / (To1 - From1);
}

float AGameCameraPawn::ILerpClamped(float A, float B, const float Value) {
	if (A == B) {
		return Value < A ? 0.f : 1.f;
	}

	if (A < Value && Value < B) // Check for early return.
	{
		return (Value - A) / (B - A);
	}

	if (A > B) {
		Swap(A, B);
	}

	if (Value < A) {
		return 0.f;
	}
	else if (Value > B) {
		return 1.f;
	}

	return (Value - A) / (B - A);
}

float AGameCameraPawn::GetNormalized(float Min, float Max, const float Value) {
	if (Min == Max) {
		return Value < Min ? 0.f : 1.f;
	}

	if (Min < Value && Value < Max) // Check for early return.
	{
		return (Value - Min) / (Max - Min);
	}

	if (Min > Max) {
		Swap(Min, Max);
	}

	if (Value < Min) {
		return 0.f;
	}
	if (Value > Max) {
		return 1.f;
	}

	return (Value - Min) / (Max - Min);
}
