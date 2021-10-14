// Fill out your copyright notice in the Description page of Project Settings.

#include "GP_AIMinionController.h"

#include "GP_Minion.h"

#include "GP4/GP4.h"
#include "GP4/GP4GameModeBase.h"
#include "GP4/GP_GameStateBase.h"

#include "GP_MinionCommands/GP_MinionCommand_Base.h"

#include "Tasks/AITask_MoveTo.h"

#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AGP_AIMinionController::AGP_AIMinionController(){
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bAttachToPawn = true;
}

void AGP_AIMinionController::OnPossess( APawn* InPawn ){
	Super::OnPossess(InPawn);
}
void AGP_AIMinionController::OnUnPossess(){
	Super::OnUnPossess();

}

// Called when the game starts or when spawned
void AGP_AIMinionController::BeginPlay(){
	Super::BeginPlay();
	// MasterMind = GetWorld()->GetGameState<AGP_GameStateBase>()->MasterMind;

	// MasterMind->AllMinionsExplodeDelegate.AddDynamic(this, &AGP_AIMinionController::ExplodeMinion);


}

void AGP_AIMinionController::EndPlay( const EEndPlayReason::Type EndPlayReason ){
	Super::EndPlay(EndPlayReason);

	// MasterMind->AllMinionsExplodeDelegate.RemoveDynamic(this, &AGP_AIMinionController::ExplodeMinion);
}

// Called every frame
void AGP_AIMinionController::Tick( float DeltaTime ){
	Super::Tick(DeltaTime);
	if ( GetPawn() != nullptr ){
		if ( bHasMoveCommandIssued ){
			MoveCommandTimer -= DeltaTime;
			HandleMove();
		}
		else{
			GetPawn<AGP_Minion>()->HandleRightInput(0);
		}
	}
	else{
		bHasMoveCommandIssued = false;
	}
}

void AGP_AIMinionController::ExplodeMinion( TSubclassOf<UGP_MinionCommand_Base> ExplodeCommand ){
	if ( GetPawn() != nullptr ){
		GetPawn<AGP_Minion>()->ReceivedCommand(ExplodeCommand);
	}
}
void AGP_AIMinionController::MinionMoveTo( FVector TargetLocation ){
	if ( GetPawn() == nullptr )
		return;
	TargetMoveLocation = TargetLocation;
	AddTargetLocationOffset();
	bHasMoveCommandIssued = true;
	bIsAtTargetLocation = false;
	MoveCommandTimer = MoveCommandTime;
}
void AGP_AIMinionController::AddTargetLocationOffset(){
	float Rand = FMath::RandRange(MoveToRange * -1.f, MoveToRange);
	UE_LOG(LogTemp, Log, TEXT("TargetLocation: %s"), *TargetMoveLocation.ToString())
	TargetMoveLocation.Y += Rand;
	UE_LOG(LogTemp, Log, TEXT("Offset TargetLocation: %s"), *TargetMoveLocation.ToString())

}
void AGP_AIMinionController::HandleMove(){

	if ( GetPawn() == nullptr ){
		bHasMoveCommandIssued = false;
		bIsAtTargetLocation = false;
		return;
	}
	if ( MoveCommandTimer <= 0.f ){
		EndMoveEarly();
	}
	if ( !bIsAtTargetLocation ){
		FVector Direction = (TargetMoveLocation - GetPawn()->GetActorLocation()).GetSafeNormal();

		//TODO check that this actually works. Could get weird results with negative values.
		Direction *= AIMovementSpeed;

		Direction.X = 0.f;
		//	Direction.Z = 0.f;
		FVector MinionLocation = GetPawn()->GetActorLocation();
		FVector TempDirection1 = MinionLocation;
		FVector TempDirection2 = TargetMoveLocation;
		TempDirection1.X = 0;
		TempDirection2.X = 0;
		float tempY = TempDirection1.Y;
		float tempY2 = TempDirection2.Y;
		TempDirection1.Y = 0;
		TempDirection2.Y = 0;
		float DistanceZ = FVector::Dist(TempDirection1, TempDirection2);
		TempDirection1.Y = tempY;
		TempDirection2.Y = tempY2;
		TempDirection1.Z = 0;
		TempDirection2.Z = 0;
		float DistanceY = FVector::Dist(TempDirection1, TempDirection2);
		//Are we close enough to be done?
		if ( DistanceY <= MoveToRange ){
			FinishMove();
			return;
		}
		FHitResult Hit;


		//Are we hitting a wall or obstacle?
		if ( GetWorld()->LineTraceSingleByChannel(Hit, MinionLocation, MinionLocation + Direction * 50.f, CollisionChannel) ){
			if ( !Cast<AGP_Minion>(Hit.GetActor()) ){
				EndMoveEarly();
				return;
			}
		}
		//Stop moving if we are falling, so that we fall straight down.
		if ( !GetPawn<AGP_Minion>()->GetIsGrounded() && !bIsPausing ){
			PauseMove(Direction);
			return;
		}
		// if ( !GetPawn<AGP_Minion>()->GetIsGrounded() && bIsPausing ){
		// 	UKismetSystemLibrary::LineTraceSingle(this, MinionLocation, MinionLocation + FVector::DownVector * 100.f, TraceTypeQuery, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, Hit, true);
		// 	if ( Hit.bBlockingHit ){
		// 		GetPawn<AGP_Minion>()->HandleRightInput(Direction.Y);
		// 	}
		// 	return;
		//}
		//Continue moving again if
		if ( GetPawn<AGP_Minion>()->GetIsGrounded() && bIsPausing ){
			ResumeMove(Direction);
			return;
		}
		// if ( DistanceZ > 100.f ){
		// 	GetPawn<AGP_Minion>()->HandleRightInput(Direction.Z);
		// }
		GetPawn<AGP_Minion>()->HandleRightInput(Direction.Y);
	}
}


void AGP_AIMinionController::ResumeMove( FVector Direction ){
	AGP_Minion* Minion = GetPawn<AGP_Minion>();
	Minion->HandleRightInput(Direction.Y);
	bIsPausing = false;
}
void AGP_AIMinionController::PauseMove( FVector Direction ){
	AGP_Minion* Minion = GetPawn<AGP_Minion>();
	Minion->HandleRightInput(Direction.Y);
	bIsPausing = true;
}
void AGP_AIMinionController::EndMoveEarly(){
	AGP_Minion* Minion = GetPawn<AGP_Minion>();
	Minion->HandleRightInput(0.f);
	bIsAtTargetLocation = false;
	bHasMoveCommandIssued = false;

}
void AGP_AIMinionController::FinishMove(){
	AGP_Minion* Minion = GetPawn<AGP_Minion>();
	Minion->HandleRightInput(0.f);
	bIsAtTargetLocation = true;
	bHasMoveCommandIssued = false;
}
