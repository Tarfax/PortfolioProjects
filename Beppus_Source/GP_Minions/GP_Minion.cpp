#include "GP_Minion.h"

#include "DestructibleComponent.h"
#include "GP_AIMinionController.h"

#include "GameFramework/PawnMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"

#include "GP4/GP_Character/GP_CameraPawn.h"
#include "GP4/GP_Character/GP_PlayerController.h"

#include "GP_MinionCommands/GP_MinionCommand_Base.h"
#include "../GP_Character/GP_LedgePlatform.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "../GP_Puzzles/GP_PuzzlePiece_Blob.h"
#include "GP4/GP_GameStateBase.h"
#include "GP_MinionCommands/GP_MinionCommand_Explode.h"
#include "GP_PatrollingAIController.h"

#include "GP4/FG_SaveGame/GP_SaveGameManager.h"
#include "GP4/LevelStreaming/GP_RestartComp.h"

AGP_Minion::AGP_Minion() {
	PrimaryActorTick.bCanEverTick = true;
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComp");
	RootComponent = CapsuleComponent;
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComponent->SetupAttachment(RootComponent);
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	DeathComponent = CreateDefaultSubobject<UGP_MinionDeathComponent>(TEXT("DeathComponent"));
	RestartComp = CreateDefaultSubobject<UGP_RestartComp>(TEXT("RestartComp"));
}

void AGP_Minion::PossessedBy(AController* NewController) {
	Super::PossessedBy(NewController);
	if (PlayerController == nullptr || MinionAiController == nullptr) {
		if (NewController->IsPlayerController()) {
			PlayerController = Cast<AGP_PlayerController>(NewController);
		}
		else {
			MinionAiController = Cast<AGP_PatrollingAIController>(NewController);
		}
	}
	if (NewController == PlayerController) {
		SetIsPossessedByPlayer(true);
		OnGetPossessedByPlayer();
	}
	else if (NewController == MinionAiController) {
		SetIsPossessedByPlayer(false);
		OnGetPossessedByAI();
	}
}

void AGP_Minion::ChangeMovementSpeed(float newValue) {
	MovementSpeed = newValue;
}

void AGP_Minion::BeginPlay() {
	Super::BeginPlay();
	if (!Controller->IsPlayerController()) {
		//TODO LedgeGrabStuff should not be here.
		// TArray<AActor*> ActorsFound;
		// UGameplayStatics::GetAllActorsOfClass(this, AGP_LedgePlatform::StaticClass(), ActorsFound);
		// for ( AActor* actor : ActorsFound ){
		// 	AGP_LedgePlatform* LedgePlatform = Cast<AGP_LedgePlatform>(actor);
		// 	if ( LedgePlatform != nullptr )
		// 		LedgePlatforms.Add(LedgePlatform);
		// }

		MinionAiController = GetController<AGP_PatrollingAIController>();
		CurrentSurfaceNormal = GetActorUpVector();
		PlayerController = GetWorld()->GetFirstPlayerController<AGP_PlayerController>();
		MasterMind = GetWorld()->GetGameState<AGP_GameStateBase>()->MasterMind;
		MasterMind->Minions.Add(this);
		FallStartHeight = GetActorLocation().Z;
		SetIsPossessedByPlayer(false);
		MinionAiController->bDebug = bDebug;
		MasterMind->AllMinionsExplodeDelegate.AddDynamic(this, &AGP_Minion::OnKillWithDelay);
	}
}

void AGP_Minion::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	// LOG("A Minion was killed. RIP");
	if (CurrentCommand != nullptr) {
		CurrentCommand->MarkPendingKill();
	}
	MasterMind->AllMinionsExplodeDelegate.RemoveDynamic(this, &AGP_Minion::OnKillWithDelay);
	MasterMind->Minions.Remove(this);
	MasterMind->Minions.Shrink();
	if (MinionAiController != nullptr) {
		MinionAiController->Destroy();

	}
}

// Called every frame
void AGP_Minion::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	HandleMovement(DeltaTime);
}

bool AGP_Minion::IsPossessedByPlayer() {
	return bIsPossessedByPlayer;
}

void AGP_Minion::SetIsPossessedByPlayer(bool value) {
	bIsPossessedByPlayer = value;
	MovementSpeed = value ? PlayerMovementSpeed : AIMovementSpeed;
}

void AGP_Minion::MouseOver(int StencilValue, TSubclassOf<UGP_MinionCommand_Base> Command) {
	SkeletalMeshComponent->SetRenderCustomDepth(true);
	SkeletalMeshComponent->SetCustomDepthStencilValue(StencilValue);
	SkeletalMeshComponent->MarkRenderStateDirty();
	if (!IsPossessedByPlayer()) {
		ChangeMovementSpeed(HoverMovementSpeed);
	}
	if (Command != nullptr) {
		OnMouseOver(Command);
	}
	// }

	if (!IsPossessedByPlayer()) {
		ChangeMovementSpeed(HoverMovementSpeed);
	}
	if (Command != nullptr) {
		OnMouseOver(Command);
	}
}

void AGP_Minion::MouseOverEnd() {
	SkeletalMeshComponent->SetRenderCustomDepth(false);
	SkeletalMeshComponent->MarkRenderStateDirty();
	if (!IsPossessedByPlayer()) {
		ChangeMovementSpeed(AIMovementSpeed);
	}
	else {
		ChangeMovementSpeed(PlayerMovementSpeed);
	}
	OnMouseOverEnd();
}

/**
 * @brief Finishes the current command (if there is one) and sets CurrentCommand to nullptr;
 */
void AGP_Minion::CancelCommand() {
	if (CurrentCommand != nullptr) {
		// LOG("CancelCommand");
		CurrentCommand->FinishCommand(this);
	}
}

AGP_MinionDeath* AGP_Minion::Kill(TSubclassOf<AGP_MinionDeath> DeathType) {

	if (bIsPendingKill) {
		return nullptr;
	}
	//UE_LOG(LogTemp, Log, TEXT("Minion::Kill= %s"), *DeathType->GetName())

	bIsPendingKill = true;
	return DeathComponent->BeginDeath(DeathType);
}

void AGP_Minion::DestroyMinion() {
	Velocity = FVector2D::ZeroVector;
	UGP_SaveGameManager::Get()->AddToTotalMinionsDead();
	Destroy();
}

bool AGP_Minion::ReceivedCommand(TSubclassOf<UGP_MinionCommand_Base> Command, bool ForceCommand) {
	if (ForceCommand == true) {
		if (CurrentCommand != nullptr) {
			CancelCommand();
		}
		CurrentCommand = NewObject<UGP_MinionCommand_Base>(this, Command);
		CurrentCommand->ExecuteCommand(this);
		return true;
	}

	if (CurrentCommand != nullptr) {
		if (!CurrentCommand->bIsOverridableCommand) {
			// LOG("Command not overridable, returning");
			return false;
		}
		// LOG("ReceivedCommand::CancelCommand");
		CancelCommand();
	}
	CurrentCommand = NewObject<UGP_MinionCommand_Base>(this, Command);
	CurrentCommand->ExecuteCommand(this);
	return true;
}

void AGP_Minion::HandleUpInput(const float Value) {
	//UpInput += Value * JumpPower;
}

void AGP_Minion::HandleRightInput(const float Value) {
	if (!IsMovementEnabled) {
		MovementInput = 0;
		return;
	}
	MovementInput = Value;
}

void AGP_Minion::HandleSpaceInput() {
	if (!IsMovementEnabled)
		return;
	if (!IsInputEnabled)
		return;
	Jump();
}

void AGP_Minion::HandleMovement(float DeltaTime) {
	float AirModifier = 1;
	//Timer to see if you can queue a jump
	if (IsJumpQueued)
		PreJumpTimer += DeltaTime;
	//Timer to check if you can jump shortly after leaving the ground

	// UE_LOG(LogTemp,Log,TEXT("Gravity: %f"), Gravity)
	if (!bIsOnGround) {
		AirJumpGraceTimer += DeltaTime;
		AirModifier = AirControl;
	}
	else {
		Velocity.Y = 0;
		bIsFallingToDeath = false;
	}

	if (PreJumpTimer > PreJumpTime) {
		IsJumpQueued = false;
	}
	//Add gravity acceleration to velocity
	Velocity.Y -= Gravity * DeltaTime;


	if (MovementInput != 0)
		//Acceleration if getting input
		Acceleration.X = MovementInput * MovementSpeed / AccelerationTime * AirModifier;

	else
		//Deacceleration if no input
		Acceleration.X = -FMath::Sign(Velocity.X) * MovementSpeed / RetardationTime * AirModifier;

	//accelerate faster if input is oposite of velocity
	if (Acceleration.X != 0 && FMath::Sign(Acceleration.X) != FMath::Sign(Velocity.X))
		Velocity += (Acceleration + Acceleration * ReactivePercentage) * DeltaTime;

	else
		Velocity += Acceleration * DeltaTime;

	if (MovementInput == 0 && FMath::Sign(Velocity.X) == FMath::Sign(Acceleration.X))
		Velocity.X = 0;

	//Limits Movement speed to max speed
	if (FMath::Abs(Velocity.X) > MovementSpeed)
		Velocity.X = FMath::Sign(Velocity.X) * MovementSpeed;
	if (Velocity.X != 0)
		OnMove(Velocity.X);

	FVector MoveDelta;

	if (!bIsOnGround && FallStartHeight - GetActorLocation().Z > FallDamageLengt && !bIsFallingToDeath) {
		bIsFallingToDeath = true;
		OnStartFallingToDeath();
	}

	if (bIsOnGround) {
		FVector SurfaceTangent = FVector::CrossProduct(FVector::ForwardVector, CurrentSurfaceNormal);
		FVector2D XVel = FVector2D(-SurfaceTangent.Y, -SurfaceTangent.Z) * Velocity.X;
		MoveDelta = FVector(0, XVel.X, Velocity.Y + XVel.Y) * DeltaTime;
	}

	else
		MoveDelta = FVector(0, Velocity.X, Velocity.Y) * DeltaTime;

	HandleCollision(MoveDelta, DeltaTime);
	if (bIsOnGround == false && GetActorLocation().Z > FallStartHeight) {
		FallStartHeight = GetActorLocation().Z;
	}

	FVector MinionLocation = GetActorLocation();
	MinionLocation.X = 0;
	SetActorLocation(MinionLocation);

	ExternalGroundCheck();

}

//Main collision function.
void AGP_Minion::HandleCollision(FVector MoveDelta, float DeltaTime) {
	bIsOnGround = false;
	FHitResult SweepHitResults;
	FHitResult SweepStepUpHitResults;
	MoveDelta = MoveDelta * SweepHitResults.Time;

	bool bUseStep = ShouldUseStep();

	//Do a sweep straight up before the movement sweep if use sweep is true.
	if (bUseStep)
		AddActorWorldOffset(FVector::UpVector * StepHeight, true, &SweepStepUpHitResults);

	//Normal collision loop Emil is a Beast
	for (int i = 0; i < 5; i++) {
		AddActorWorldOffset(MoveDelta, true, &SweepHitResults);

		if (SweepHitResults.bBlockingHit) {
			if (SweepHitResults.bStartPenetrating) {
				FVector DepenVector = SweepHitResults.Normal * (SweepHitResults.PenetrationDepth + 0.1f);
				AddActorWorldOffset(DepenVector);
			}

			MoveDelta -= FVector::DotProduct(MoveDelta, SweepHitResults.Normal) * SweepHitResults.Normal;

			InternalGroundCheck(SweepHitResults);

			if (FallStartHeight - GetActorLocation().Z > FallDamageLengt && !bIsOnGround) {
				if (bIsFallingToDeath == false) {
					bIsFallingToDeath = true;
				}
			}
		}
		MoveDelta -= MoveDelta * SweepHitResults.Time;
	}

	FHitResult SweepStepDownHitResults;
	if (bUseStep) {
		AddActorWorldOffset(FVector::DownVector * StepHeight * SweepStepUpHitResults.Time,
		                    true,
		                    &SweepStepDownHitResults);

		InternalGroundCheck(SweepStepDownHitResults);
	}
}

//Returns the External Is grounded value for use in animation and gameplay blueprints.
bool AGP_Minion::GetIsGrounded() {
	return bExternalIsOnGround;
}


//Internal ground check used in collision and movement logic. More accurate than the external check but this also makes
//it more sensitive so it might be bad for animations and other visual stuff.
void AGP_Minion::InternalGroundCheck(FHitResult SweepHitResults) {

	//Checks if you collide with a roof and removes upwards velocity if that is the case.
	if (FVector::DotProduct(SweepHitResults.ImpactNormal, FVector::DownVector) > 0.4 && Velocity.Y > 0)
		Velocity.Y = 0;

	//checks if it's colliding with a surface that is at a walkable angle.
	if (FVector::DotProduct(SweepHitResults.ImpactNormal, FVector::UpVector) >
		FMath::Cos(FMath::DegreesToRadians(WalkableAngle))) {

		//Kill Minion if you have fallen for a certain distance and are not landing on a blob. 
		if (FallStartHeight - GetActorLocation().Z > FallDamageLengt && !bIsOnGround) {

			if (SweepHitResults.Actor->IsA(AGP_PuzzlePiece_Blob::StaticClass()) == false) {
				OnDiedOnFall();
			}
			else {
				OnSurvivedFall();
				bIsFallingToDeath = false;
			}
		}

		bIsOnGround = true;
		AirFrames = 0;
		CurrentSurfaceNormal = SweepHitResults.ImpactNormal;
		AirJumpGraceTimer = 0;
		FallStartHeight = -100000000;
		bIsJumping = false;
		bIsFallingToDeath = false;

		if (IsJumpQueued)
			Jump();
	}
}

//Ground check used for animation and other things that want a less sensitive output. You get consistency for the price
//of accuracy.
void AGP_Minion::ExternalGroundCheck() {

	FHitResult Hit;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	FVector StartPos = GetActorLocation();

	FCollisionShape SweepShape = FCollisionShape::MakeSphere(CapsuleComponent->GetScaledCapsuleRadius());

	GetWorld()->SweepSingleByChannel(Hit,
	                                 StartPos,
	                                 StartPos -
	                                 FVector(
		                                 0, 0, GroundCheckDistance + CapsuleComponent->GetScaledCapsuleHalfHeight()),
	                                 FQuat::Identity,
	                                 GroundCheckCollisionChannels,
	                                 SweepShape,
	                                 QueryParams);

	bExternalIsOnGround = Hit.bBlockingHit;
}

bool AGP_Minion::ShouldUseStep() {
	FVector DisplacedPosition = FVector(0,
	                                    FMath::Sign(Velocity.X) * CapsuleComponent->GetScaledCapsuleRadius(),
	                                    StepHeight) + GetActorLocation();

	FHitResult Hit;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);


	FCollisionShape SweepShape = FCollisionShape::MakeCapsule(CapsuleComponent->GetScaledCapsuleRadius(),
	                                                          CapsuleComponent->GetScaledCapsuleHalfHeight());

	GetWorld()->SweepSingleByChannel(Hit,
	                                 DisplacedPosition,
	                                 DisplacedPosition,
	                                 FQuat::Identity,
	                                 GroundCheckCollisionChannels,
	                                 SweepShape,
	                                 QueryParams);

	return !Hit.bBlockingHit;
}

void AGP_Minion::ResetMinionFallHeight() {
	if (!bIsOnGround) {
		FallStartHeight = GetActorLocation().Z;
	}
}

void AGP_Minion::Jump() {
	if (!IsInputEnabled)
		return;
	if (bIsJumping && !IsJumpQueued) {
		IsJumpQueued = true;
		PreJumpTimer = 0;
	}
	if (bIsJumping)
		return;
	if (!bIsOnGround && AirJumpGraceTimer > AirJumpGraceTime)
		return;

	OnJump();
	bIsJumping = true;
	bIsOnGround = false;
	Velocity.Y = JumpStrength;
}
