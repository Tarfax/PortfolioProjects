// Fill out your copyright notice in the Description page of Project Settings.


#include "GP_PuzzlePiece_Portal.h"
#include "Components/BoxComponent.h"

#include "GP4/GP4.h"
#include "GP4/GP_GameStateBase.h"
#include "GP4/GP_Minions/GP_Minion.h"
#include "GP4/LevelStreaming/GP_RestartComp.h"


// Sets default values
AGP_PuzzlePiece_Portal::AGP_PuzzlePiece_Portal() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	SetRootComponent(BoxCollider);

	//BoxCollider->SetupAttachment(SceneComponent);

	// Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	// Mesh->SetupAttachment(BoxCollider);

	TeleportPosition = CreateDefaultSubobject<USceneComponent>("TeleportPosition");
	TeleportPosition->SetupAttachment(BoxCollider);
	RestartComp = CreateDefaultSubobject<UGP_RestartComp>(TEXT("RestartComp"));

}

// Called when the game starts or when spawned
void AGP_PuzzlePiece_Portal::BeginPlay() {
	Super::BeginPlay();
	AGP_GameStateBase* GameState = Cast<AGP_GameStateBase>(GetWorld()->GetGameState());
	if (GameState != nullptr) {
		GameState->AddPortal(this);
	}

	// UE_LOG(LogTemp, Log, TEXT("10 = %f, 20 = %f, 45 = %f, 60 = %f, 90 = %f"), FMath::Cos(FMath::DegreesToRadians(10)), FMath::Cos(FMath::DegreesToRadians(20)),
	//FMath::Cos(FMath::DegreesToRadians(45)), FMath::Cos(FMath::DegreesToRadians(60)), FMath::Cos(FMath::DegreesToRadians(90)))
	// BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AGP_PuzzlePiece_Portal::OnBeginOverlap);
	// BoxCollider->Bind
}

void AGP_PuzzlePiece_Portal::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	// BoxCollider->OnComponentBeginOverlap.RemoveDynamic(this, &AGP_PuzzlePiece_Portal::OnBeginOverlap);
}

// Called every frame
void AGP_PuzzlePiece_Portal::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	if (IsMarkedForDestroy == false) {

		// if (CanTeleport == false == CanEverMove == false) {
		// 	TeleportCooldown -= DeltaTime;
		// 	if (TeleportCooldown <= 0.0f) {
		// 		CanTeleport = true;
		// 	}
		// }

			Velocity.Z -= Gravity * DeltaTime;

			FVector MoveDelta = FVector(0, 0, Velocity.Z) * DeltaTime;

			FHitResult HitResults;

			AddActorWorldOffset(MoveDelta, true, &HitResults);

			MoveDelta -= MoveDelta * HitResults.Time;

			for (int i = 0; i < 4; i++)
			{
				if(HitResults.bBlockingHit)
				{
				
					MoveDelta -= FVector::DotProduct(MoveDelta, HitResults.Normal) * HitResults.Normal;
				
					if ( HitResults.bStartPenetrating ){
				
						FVector DepenVector = HitResults.Normal * (HitResults.PenetrationDepth + 0.1f);
						AddActorWorldOffset(DepenVector);
					}
			
					if( FVector::DotProduct(HitResults.ImpactNormal, FVector::UpVector) >
						FMath::Cos(FMath::DegreesToRadians(AngleToGlideAt)))
					{
						Velocity.Z = 0;
					}
					CollisionResultHandling(HitResults.Actor.Get());
				}

				MoveDelta.X = 0.0f;
				MoveDelta.Y = 0.0f;
				AddActorWorldOffset(MoveDelta, true, &HitResults);
				MoveDelta -= MoveDelta * HitResults.Time;
			}
	}

	// if (IsMarkedForDestroy == true) {
	// 	DestroyPortal();
	// }

}

void AGP_PuzzlePiece_Portal::DestroyPortal() {

	Destroy();
}

void AGP_PuzzlePiece_Portal::MarkForDestroy() {
	if (IsMarkedForDestroy == false) {
		IsMarkedForDestroy = true;
		OnBeingDestroyed();
		RemovePortalConnection();
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AGP_PuzzlePiece_Portal::DestroyPortal, 2.0f, false, -1.0);
	}	
}

void AGP_PuzzlePiece_Portal::Explode() {
	IsExploding = true;
	AGP_GameStateBase* GameState = Cast<AGP_GameStateBase>(GetWorld()->GetGameState());
	if (GameState != nullptr) {
		GameState->RemovePortal(this);
	}
	MarkForDestroy();
	OnExplode();
}

void AGP_PuzzlePiece_Portal::RemovePortalConnection() {
	ConnectedPortal = nullptr;
	HasConnection = false;
}


void AGP_PuzzlePiece_Portal::SetPortalConnection(AGP_PuzzlePiece_Portal* Portal) {
	ConnectedPortal = Portal;
	HasConnection = true;
}

void AGP_PuzzlePiece_Portal::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                            UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
                                            const FHitResult& Hit) {

	if (HasConnection == true && CanTeleport == true) {
		OtherActor->SetActorLocation(ConnectedPortal->TeleportPosition->GetComponentLocation());

		UE_LOG(LogTemp, Log, TEXT("poooortsl"))

		AGP_Minion* Minion = Cast<AGP_Minion>(OtherActor);
		if(Minion != nullptr)
		{
			Minion->ResetMinionFallHeight();
		}
	}
}

AGP_Minion* AGP_PuzzlePiece_Portal::OnMinionBeginOverlap(AGP_Minion* OverlappingMinion) {

	// if (HasConnection == true && CanTeleport == true) {
	// 	OverlappingMinion->SetActorLocation(ConnectedPortal->TeleportPosition->GetComponentLocation());
	// }

	return OverlappingMinion;
}
