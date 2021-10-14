// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GP_Pawn_Base.h"
#include "GP4/LevelStreaming/GP_LevelInstanceData.h"
#include "Components/SplineComponent.h"

#include "GP_CameraPawn.generated.h"

class AGP4GameModeBase;
class UCineCameraComponent;
class AGP_MinionMasterMindBigBrainController;
class USpringArmComponent;
class UCameraComponent;
class AGP_PlayerController;
class AGP_Pawn_Base;
class UGP_RestartComp;
class UGP_AudioSystem;

UENUM(BlueprintType)
enum class EGP_CameraLerpSettings : uint8{
	Lerp = 0,
	EaseIn = 1,
	EaseOut = 2,
	EaseInOut = 3
};

UCLASS(BlueprintType, Blueprintable)
class UCameraSettings : public UDataAsset {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Restart Level LERPS | Movement")
	float MovementLerpTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Restart Level LERPS | Movement")
	float MovementExpo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Restart Level LERPS | Distance")
	float DistanceLerpTime;		
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Restart Level LERPS | Distance")
	// float DistanceExpo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Restart Level LERPS | Distance")
	float LerpTimeMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Restart Level LERPS | Distance")
	float MidPointDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Restart Level LERPS | Distance")
	float ClampMinDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Restart Level LERPS | Distance")
	float ClampMaxDistance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Look At | Speed")
	float CameraRotateLookAtSpeed = 0.4f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Look At | Speed")
	float CameraRotateLookAtSpeedResetStillPossess= 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Look At | Speed")
	float CameraRotateLookAtSpeedReset= 4.0f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Possess")
	float CameraOffsetZMinusWhenPossessClamp = -300.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Possess")
	float CameraOffsetZPlusWhenPossessClamp = 100.0f;
};

UCLASS()
class AGP_CameraPawn : public AGP_Pawn_Base {
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGP_CameraPawn();

	virtual void BeginPlay() override;
	virtual void EndPlay( const EEndPlayReason::Type EndPlayReason ) override;
	void UnPossessedCameraMovement(float DeltaTime);
	void MinionCameraMovement(float DeltaTime);
	bool MoveToStart( float DeltaTime );
	bool MoveToResetPosition(float DeltaTime);
	virtual void Tick( float DeltaTime ) override;
	virtual void PossessedBy( AController* NewController ) override;
	virtual void UnPossessed() override;

	UPROPERTY()
	AGP4GameModeBase* GameMode;
	virtual void HandleUpInput( const float Value ) override;
	virtual void HandleRightInput( const float Value ) override;
	virtual void HandleZoomInput( float Value ) override;
	void FollowMinion( AGP_Pawn_Base* PawnToFollow );
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UMatineeCameraShake> LevelResetShakeClass;
	FTimerHandle InOutHandle;
	UFUNCTION(BlueprintCallable)
	void ShakeCamera( TSubclassOf<UMatineeCameraShake> ShakeClass );
	UFUNCTION(BlueprintImplementableEvent)
	void OnCameraShake();
	UFUNCTION(BlueprintImplementableEvent)
	void OnCameraShakeComplete();
	UFUNCTION(BlueprintImplementableEvent)
	void OnPossessMinion();
	UFUNCTION(BlueprintImplementableEvent)
	void OnUnPossessMinion();

	void SetLevelInstanceData( AGP_LevelInstanceData* LevelInstanceData );
	UFUNCTION(BlueprintImplementableEvent)
	void OnMoveToNewLevel();
	UFUNCTION(BlueprintImplementableEvent)
	void OnMoveToResetPosition();
	void PopulateSpline( const FTransform& Transform ) const;
	UFUNCTION(BlueprintCallable)
	void MoveToPlayerStart();
	UFUNCTION(BlueprintImplementableEvent)
	void OnCameraPawnTravelComplete();

	UFUNCTION(BlueprintCallable)
	void SetCameraDistanceWhenPossessingOffset(float OffsetAmount);
	UFUNCTION(BlueprintCallable)
	void SetCameraLocationWhenPossessingOffset(FVector OffsetAmount, FVector LookAtLocation, bool UseLookAt);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveToPlayerStartDistanceTolerance = 10.f;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* SceneComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USpringArmComponent* SpringArmComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UCineCameraComponent* CameraComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USplineComponent* SplineComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Level Handling")
	UGP_RestartComp* RestartComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Audio")
	UGP_AudioSystem* AudioSystem;
	
	UPROPERTY(EditAnywhere, Category = "Camera|Settings")
	float MaxAcceleration = 150.0f;
	UPROPERTY(EditAnywhere, Category = "Camera|Settings")
	float FrictionCoefficient = 6.0f;

	UPROPERTY(EditAnywhere, Category = "Camera|Boundary Settings")
	float BoundaryMinY;
	UPROPERTY(EditAnywhere, Category = "Camera|Boundary Settings")
	float BoundaryMaxY;

	UPROPERTY(EditAnywhere, Category = "Camera|Boundary Settings")
	float BoundaryMinZ;
	UPROPERTY(EditAnywhere, Category = "Camera|Boundary Settings")
	float BoundaryMaxZ;

	UPROPERTY(Transient)
	AGP_MinionMasterMindBigBrainController* MasterMind;

	UPROPERTY()
	AGP_PlayerController* PlayerController;

	bool IsPossessed;
	bool bShakeCameraThisTick = false;
	UPROPERTY()
	AGP_Pawn_Base* MinionToFollow;

	FTransform LevelStartLocation;

	float UpInput;
	float RightInput;
	float ZoomValueInput;

	FVector MoveVelocity;
	FTransform TargetTransform;
	FVector FrameVelocity;
	FVector PrevPosition;
	float t;
	float CameraYOffset;
	float CameraZOffset;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Camera Settings")
	float CameraDistance;
	UPROPERTY(EditAnywhere, Category = "Camera Possess Settings")
	float CameraFollowMinionSpeed;

	UPROPERTY(EditAnywhere, Category = "Camera Possess Settings")
	float CameraFollowMinionSpeedWhenMoving;
	UPROPERTY(EditAnywhere, Category = "Camera Possess Settings")
	float CameraDistanceWhenPossess;
	UPROPERTY(EditAnywhere, Category = "Camera Possess Settings")
	float CameraDistanceWhenPossessOffset = 0.0f;
	UPROPERTY(EditAnywhere, Category = "Camera Possess Settings")
	float CameraZOffsetWhenPossess;
	UPROPERTY(EditAnywhere, Category = "Camera Possess Settings")
	FVector CameraOffsetWhenPossessForced;
	UPROPERTY(EditAnywhere,  Category = "Camera Possess Settings")
	float CameraOffsetZValueDelta;
	UPROPERTY(EditAnywhere,  Category = "Camera Possess Settings")
	float CameraOffsetZValue;
	UPROPERTY(EditAnywhere, Category = "Camera Possess Settings")
	float CameraYOffsetWhenPossess;

	UPROPERTY(EditAnywhere, Category = "Camera Possess Settings")
	FVector CameraLocationWhenPossessOffset = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Camera Possess Settings")
	FVector CameraLookAtLocationWhenPossess = FVector::ZeroVector;
	
	UPROPERTY(EditAnywhere, Category="Camera Possess Settings")
	bool UseLookAtRotation;
	
	UPROPERTY(EditAnywhere, Category = "Camera Possess Settings")
	float CameraYTiltWhenPossess;

	UPROPERTY(EditAnywhere, Category = "Camera Possess Settings")
	float CameraOffsetWhenFalling;
	UPROPERTY(EditAnywhere, Category = "Camera Possess Settings")
	float CameraFollowMinionSpeedWhenFalling;
	UPROPERTY(EditAnywhere, Category = "Camera Possess Settings")
	float CameraOffsetFallVelocityThreshold;

	UPROPERTY(EditAnywhere, Category = "Camera Settings")
	float DefaultCameraDistance;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Camera Settings")
	float ZoomDistanceNormalized;
	UPROPERTY(EditAnywhere, Category = "Camera Settings")
	float CameraZoomSpeed;
	UPROPERTY(EditAnywhere, Category="Camera Settings")
	float MoveToPlayerStartSpeed = 1.f;
	UPROPERTY(EditAnywhere, Category = "Camera Settings")
	float ZoomStep;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera Settings")
	float ZoomMinDistance;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera Settings")
	float ZoomMaxDistance;
	bool bIsResettingToPlayerStart = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Settings")
	float TravelZoomOutAmount = 5500;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Camera Settings")
	UCameraSettings* CameraSettings;
	FVector LerpStartPosition;
	float CameraDistanceStartPosition;

	float CameraPositionLerpTimer;
	float CameraDistanceLerpTimer;
	float TotalCameraDistanceLerpTimer;
	float MidCameraDistance;

	bool UseCameraDistanceLerpTimer;
	bool HasPassedHalfZoomTime;
	bool StartCameraMovementResetting;
	
	

};
