#include "GP_PatrollingAIController.h"

#include "DrawDebugHelpers.h"
#include "GP_AIMinionController.h"
#include "GP_Minion.h"

#include "GP4/GP4.h"
#include "GP4/GP_GameStateBase.h"
#include "GP4/GP_Puzzles/GP_PuzzlePiece_Portal.h"

#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AGP_PatrollingAIController::AGP_PatrollingAIController(){
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bAttachToPawn = true;
	
}

void AGP_PatrollingAIController::OnPossess( APawn* InPawn ){
	Super::OnPossess(InPawn);

	Minion = Cast<AGP_Minion>(InPawn);
	ensure(Minion!=nullptr);
	CurrentDirection = Minion->GetActorRightVector();
	bShouldBeMoving = Minion->bStartWithAIMovementEnabled;

	SetCollisionQueryParams();
}

void AGP_PatrollingAIController::OnUnPossess(){
	Super::OnUnPossess();
	MasterMind = AGP_MinionMasterMindBigBrainController::Get();

	if ( bDebug ) LOG("AIController OnUnPossess");
}
void AGP_PatrollingAIController::Reset(){
	Super::Reset();
	if ( GetPawn() == nullptr ){
		Destroy(true, true);
	}
	else{
		MarkPendingKill();
	}
}

void AGP_PatrollingAIController::BeginPatrolMove(){
	bShouldBeMoving = true;
}

void AGP_PatrollingAIController::EndPatrolMove(){
	bShouldBeMoving = false;
	Minion->HandleRightInput(0.f);
}

// Called when the game starts or when spawned
void AGP_PatrollingAIController::BeginPlay(){
	Super::BeginPlay();
	SecondTraceHeightOffset = 50.f;

}

void AGP_PatrollingAIController::EndPlay( const EEndPlayReason::Type EndPlayReason ){
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void AGP_PatrollingAIController::Tick( float DeltaTime ){
	AAIController::Tick(DeltaTime);
	if ( GetPawn() != nullptr ){
		if ( bShouldBeMoving ){
			HandleMove();
		}
	}
}

void AGP_PatrollingAIController::HandleMove(){
	CurrentDirection.X = 0.f;
	if ( !bIsPausing ){
		Minion->HandleRightInput(CurrentDirection.Y);
		FVector TraceStart = GetPawn()->GetPawnViewLocation() + CurrentDirection * 20.f;
		FVector TraceEnd = TraceStart + CurrentDirection * TraceLength;
		FHitResult Hit;
		FColor Color;
		if ( GetWorld()->LineTraceSingleByObjectType(Hit, TraceStart, TraceEnd, COQP) ){
			if ( bDebug ){
				Color = Hit.bBlockingHit ? FColor::Red : FColor::Green;
				UE_LOG(LogTemp, Log, TEXT("OBJECT TRACE HIT: %s"), *Hit.GetActor()->GetName().ToUpper())
				DrawDebugLine(this->GetWorld(), TraceStart, TraceEnd, Color, false, 0.3f, 1, 3);
			}
			if ( Hit.bBlockingHit ){
				AGP_PuzzlePiece_Portal* Portal = Cast<AGP_PuzzlePiece_Portal>(Hit.GetActor());
				if ( Portal || Minion->IsTeleporting == true )
					return;
				TurnAround();
				return;
			}
		}
		else{
			TraceStart.Z -= SecondTraceHeightOffset;
			TraceEnd = TraceStart + CurrentDirection * (TraceLength * 0.4f);
			if ( GetWorld()->LineTraceSingleByObjectType(Hit, TraceStart, TraceEnd, COQP) ){
				if ( bDebug ){
					Color = Hit.bBlockingHit ? FColor::Red : FColor::Green;
					UE_LOG(LogTemp, Log, TEXT("OBJECT TRACE HIT: %s"), *Hit.GetActor()->GetName().ToUpper())
					DrawDebugLine(this->GetWorld(), TraceStart, TraceEnd, Color, false, 0.3f, 1, 3);
				}
				if ( Hit.bBlockingHit ){
					AGP_PuzzlePiece_Portal* Portal = Cast<AGP_PuzzlePiece_Portal>(Hit.GetActor());
					if ( Portal )
						return;
					TurnAround();
					return;
				}
			}
		}
	}
	if ( !GetPawn<AGP_Minion>()->GetIsGrounded() && !bIsPausing ){
		PauseMove(CurrentDirection);
		return;
	}
	if
	( GetPawn<AGP_Minion>()->GetIsGrounded() && bIsPausing ){
		ResumeMove(CurrentDirection);
	}
}

void AGP_PatrollingAIController::ResumeMove( FVector Direction ){
	CurrentDirection = Minion->GetActorRightVector();

	bIsPausing = false;
}

void AGP_PatrollingAIController::PauseMove( FVector Direction ){
	Minion->HandleRightInput(0);
	CurrentDirection.Y = 0;
	bIsPausing = true;
}

void AGP_PatrollingAIController::TurnAround(){

	if ( CurrentDirection.Y > 0 ){
		CurrentDirection.Y = -1.f;
	}
	else{
		CurrentDirection.Y = 1.f;
	}

}

void AGP_PatrollingAIController::SetCollisionQueryParams(){

	for ( auto Channel : CollisionChannels ){
		COQP.AddObjectTypesToQuery(Channel);
	}
	ensure(COQP.IsValid());


}

void AGP_PatrollingAIController::FinishMove(){
	Minion->HandleRightInput(0.f);
}
