// Fill out your copyright notice in the Description page of Project Settings.


#include "GP_PlatformerCharacter.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AGP_PlatformerCharacter::AGP_PlatformerCharacter()
{
	CapsuleCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("capsuleCollider"));
	SetRootComponent(CapsuleCollider);

 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGP_PlatformerCharacter::BeginPlay()
{
	Super::BeginPlay();

	InputComponent->BindAxis(TEXT("Right"), this, &AGP_PlatformerCharacter::MoveRight);
	InputComponent->BindAction(TEXT("SpaceBar"), IE_Pressed, this, &AGP_PlatformerCharacter::Jump);

	CurrentSurfaceNormal = GetActorUpVector();
}

// Called every frame
void AGP_PlatformerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float AirModifier = 1;

	if(IsJumpQueued)
	{
		PreJumpTimer += DeltaTime;
	}
	
	if(!bIsOnGround)
	{
		AirJumpGraceTimer += DeltaTime;
	}

	if(PreJumpTimer > PreJumpTime)
	{
		IsJumpQueued = false;
	}

	if(!bIsOnGround)
	{
		AirModifier = AirControl;
	}

	if(bIsOnGround == false)
	{
		Velocity.Y -= Gravity * DeltaTime;
	}
	else
	{
		Velocity.Y = 0;
	}

	if(MovementInput != 0)
	{
		Acceleration.X = MovementInput * MovementSpeed / AccelerationTime * AirModifier;
	}
	else
	{
		Acceleration.X = -FMath::Sign(Velocity.X) * MovementSpeed / RetardationTime * AirModifier;
	}

	if(Acceleration.X != 0 && FMath::Sign(Acceleration.X) != FMath::Sign(Velocity.X))
	{
		Velocity += (Acceleration + Acceleration * ReactivePercentage) * DeltaTime;
	}
	else
	{
		Velocity += Acceleration * DeltaTime;
		
	}

	if(MovementInput == 0 && FMath::Sign(Velocity.X) == FMath::Sign(Acceleration.X))
	{
		Velocity.X = 0;
	}

	if(FMath::Abs(Velocity.X) > MovementSpeed)
	{
		Velocity.X = FMath::Sign(Velocity.X) * MovementSpeed;
	}

	UE_LOG(LogTemp, Log, TEXT("Accel: %f"), Acceleration.X)



	FHitResult SweepHitResults;
	FVector MoveDelta;
	if(bIsOnGround)
	{
		 MoveDelta = FVector::CrossProduct(CurrentSurfaceNormal, GetActorForwardVector())
		 + FVector(0,0,Velocity.Y);
		//MoveDelta = FVector(0, Velocity.X, Velocity.Y) * DeltaTime;
	}
	else
	{
		MoveDelta = FVector(0, Velocity.X, Velocity.Y) * DeltaTime;
	}

	//AddActorWorldOffset(MoveDelta, true, &SweepHitResults);
	
	MoveDelta = MoveDelta * SweepHitResults.Time;

	bIsOnGround = false;
	for (int i = 0; i<5; i++)
	{
		AddActorWorldOffset(MoveDelta, true, &SweepHitResults);
	
		if(SweepHitResults.bBlockingHit)
		{
			if(SweepHitResults.bStartPenetrating)
			{
				FVector DepenVector = SweepHitResults.Normal * (SweepHitResults.PenetrationDepth + 0.1f);
				AddActorWorldOffset(DepenVector);
			}
			MoveDelta -= FVector::DotProduct(MoveDelta, SweepHitResults.Normal) * SweepHitResults.Normal;

			CurrentSurfaceNormal = SweepHitResults.Normal;
			if (FVector::DotProduct(SweepHitResults.ImpactNormal, FVector(0,0,1)) > 0.8)
			{
				bIsOnGround = true;
				AirJumpGraceTimer = 0;
				bIsJumping = false;
				if(IsJumpQueued)
				{
					Jump();
				}
			}
		}
	}

}

void AGP_PlatformerCharacter::MoveRight(float value)
{
	MovementInput = value;
}

void AGP_PlatformerCharacter::Jump()
{
	if(bIsJumping && !IsJumpQueued)
	{
		IsJumpQueued = true;
		PreJumpTimer = 0;
	}
	if(bIsJumping) return;

	if(!bIsOnGround && AirJumpGraceTimer > AirJumpGraceTime) return;
	bIsJumping = true;
	bIsOnGround = false;
	Velocity.Y = JumpStrength;
}

// Called to bind functionality to input
void AGP_PlatformerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

