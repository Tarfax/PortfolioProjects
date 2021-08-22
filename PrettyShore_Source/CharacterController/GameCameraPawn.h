#pragma once
#include "GameFramework/Pawn.h"
#include "GameCameraPawn.generated.h"

class UCameraComponent;

UCLASS()
class AGameCameraPawn : public APawn
{
	GENERATED_BODY()

public:
	AGameCameraPawn();
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* CameraAnchorComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UFUNCTION(BlueprintCallable)
	void FollowTargetActor(AActor* TargetActor);

	
	// --- Camera Base Movement public ---

	/** How fast the camera is allowed to move at minimum zoom distance. */
	UPROPERTY(EditAnywhere, Category = "Camera Base Movement")
	float AccelerationMinZoom = 200.f;

	/** How fast the camera is allowed to move at maximum zoom distance. */
	UPROPERTY(EditAnywhere, Category = "Camera Base Movement")
	float AccelerationMaxZoom = 1000.f;

	/** How fast the camera will come to a stop after moving. */
	UPROPERTY(EditAnywhere, Category = "Camera Base Movement")
	float FrictionCoefficient = 8.f;

	/** How fast the camera will be rotated by the mouse. */
	UPROPERTY(EditAnywhere, Category = "Camera Base Movement")
	float RotationMouseSensitivity = .1f;
	/** How fast the camera is allowed to rotate. */
	UPROPERTY(EditAnywhere, Category = "Camera Base Movement")
	float MaxAngularAcceleration = 15.f;

	/** How fast the camera will come to a stop after rotating. */
	UPROPERTY(EditAnywhere, Category = "Camera Base Movement")
	float AngularFrictionCoefficient = 8.f;

	UPROPERTY(EditAnywhere, Category = "Camera Base Movement")
	float CameraWorldBoundary_Max_X = 8000.f;
	
	UPROPERTY(EditAnywhere, Category = "Camera Base Movement")
	float CameraWorldBoundary_Min_X = -4200.f;
	
	UPROPERTY(EditAnywhere, Category = "Camera Base Movement")
	float CameraWorldBoundary_Max_Y = 14500.f;
	
	UPROPERTY(EditAnywhere, Category = "Camera Base Movement")
	float CameraWorldBoundary_Min_Y = -10000.f;
	// --- --- ---
	
	
	// --- Camera Boom Arm Manipulation public ---
	
	/** Minimum tilt angle of the Camera when at minimum zoom distance. */
	UPROPERTY(EditAnywhere, Category = "Camera Boom Arm Manipulation")
	float MinAngleMinZoom = 10.f;
	
	/** Maximum tilt angle of the Camera when at minimum zoom distance. */
	UPROPERTY(EditAnywhere, Category = "Camera Boom Arm Manipulation")
	float MaxAngleMinZoom = 90.f;
	
	/** Minimum tilt angle of the Camera when at maximum zoom distance. */
	UPROPERTY(EditAnywhere, Category = "Camera Boom Arm Manipulation")
	float MinAngleMaxZoom = 40.f;

	/** Maximum tilt angle of the Camera when at maximum zoom distance. */
	UPROPERTY(EditAnywhere, Category = "Camera Boom Arm Manipulation")
	float MaxAngleMaxZoom = 90.f;

	/** Threshold for the distance at which the camera should start altering the minimum tilt angle. The distance is added on top of the MinZoomDistance. */
	UPROPERTY(EditAnywhere, Category = "Camera Boom Arm Manipulation")
	float MinAngleDistanceThresholdStart = 0.f;
	
	/** Threshold for the distance at which the camera should stop altering the minimum tilt angle. The distance is subtracted from the MaxZoomDistance. */
	UPROPERTY(EditAnywhere, Category = "Camera Boom Arm Manipulation")
	float MinAngleDistanceThresholdEnd = 2000.f;
	
	/** Threshold for the distance at which the camera should start altering the maximum tilt angle. The distance is added on top of the MinZoomDistance. */
	UPROPERTY(EditAnywhere, Category = "Camera Boom Arm Manipulation")
	float MaxAngleDistanceThresholdStart = 0.f;
	
	/** Threshold for the distance at which the camera should stop altering the maximum tilt angle. The distance is subtracted from the MaxZoomDistance. */
	UPROPERTY(EditAnywhere, Category = "Camera Boom Arm Manipulation")
	float MaxAngleDistanceThresholdEnd = 0.f;

	/** Change to the camera tilt angle, change measured in degrees per second. */
	UPROPERTY(EditAnywhere, Category = "Camera Boom Arm Manipulation")
	float TiltAngleStepMinZoom = 90.f;
	
	/** Change to the camera tilt angle, change measured in degrees per second. */
	UPROPERTY(EditAnywhere, Category = "Camera Boom Arm Manipulation")
	float TiltAngleStepMaxZoom = 150.f;

	/** The mouse sensitivity of the camera tilt. */
	UPROPERTY(EditAnywhere, Category = "Camera Boom Arm Manipulation")
	float CameraTiltMouseSensitivity = .2f;

	/** Closest distance the camera is allowed to be to the ground, measured in units. */
	UPROPERTY(EditAnywhere, Category = "Camera Boom Arm Manipulation")
	float MinZoomDistance = 500.f;

	/** Farthest distance the camera is allowed to be from the ground, measured in units. */
	UPROPERTY(EditAnywhere, Category = "Camera Boom Arm Manipulation")
	float MaxZoomDistance = 10000.f;

	/** How far the camera will zoom with each step of the scroll wheel, applied in units. */
	UPROPERTY(EditAnywhere, Category = "Camera Boom Arm Manipulation")
	float ZoomStep = 500.f;

	/** How fast the camera moves to its intended location. Higher values means more responsive. */
	UPROPERTY(EditAnywhere, Category = "Camera Boom Arm Manipulation")
	float CameraResponsiveness = 4.f;
	// --- --- ---

	/** Which trace channel the camera should test for when looking for objects to avoid. */
	UPROPERTY(EditAnywhere, Category = "Other")
	TEnumAsByte<ETraceTypeQuery> TraceType;

	// --- Game Time Speed Input ---
	UFUNCTION(BlueprintCallable, Category="Worldtime Manipulation")
	void HandleSetTimeSpeed_1Input();
	UFUNCTION(BlueprintCallable, Category="Worldtime Manipulation")
	void HandleSetTimeSpeed_2Input();
	UFUNCTION(BlueprintCallable, Category="Worldtime Manipulation")
	void HandleSetTimeSpeed_3Input();
	UFUNCTION(BlueprintCallable, Category="Worldtime Manipulation")
	void HandleSetTimeSpeed_4Input();
	UFUNCTION(BlueprintCallable, Category="Worldtime Manipulation")
	void HandleSetTimeSpeed_PauseInput();
	// --- --- ---

#if WITH_EDITORONLY_DATA
	// --- Debug public ---
	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bDebug = false;
	
	UPROPERTY(EditAnywhere, Category = "Debug")
	float DebugCameraAngle = 45.f;
	
	UPROPERTY(EditAnywhere, Category = "Debug")
	float DebugCameraDistance = 2000.f;
	// --- --- ---
#endif

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool IsPaused = false;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float TimeScale = 1.0f;

private:

	// --- Tick Safeguard private ---
	const float Max_Tick_Time = 5.f;
	
	// --- Camera Base Movement private ---
	float MaxAcceleration;
	float MoveForwardInput;
	float MoveRightInput;
	FVector MoveVelocity;

	float RotateRightInput;
	FRotator AngularVelocity;

	UPROPERTY()
	AActor* TargetedActor;
	bool bFollowTargetedActor = false;
	
	void HandleMoveForwardInput(const float Value);
	void HandleMoveRightInput(const float Value);
	void HandleRotateRightInput(const float Value);
	// --- --- ---

	// --- Camera Boom Arm Manipulation private ---
	float CameraTiltInput;
	float CameraTiltNormalizedRange = .05f;
	float AppliedCameraTiltAngleDeg;
	float CameraDistanceInput;
	float CameraDistance = 5000.f;
	FVector CameraLocalPos;

	bool bMiddleMouseHeld;

	void HandleZoomInput(const float Value);
	void HandleTiltCameraInput(const float Value);
	void HandleMiddleMouseInputPressed();
	void HandleMiddleMouseInputReleased();
	// --- --- ---

	// --- Support Functions ---
	float RemapValue(float From1, float To1, float From2, float To2, float Value);
	float ILerpClamped(float A, float B, float Value);
	float GetNormalized(float Min, float Max, float Value);
	// --- --- ---

};
