// Fill out your copyright notice in the Description page of Project Settings.


#include "GP_PuzzlePiece_Blob.h"
#include "Components/BoxComponent.h"
#include "DestructibleComponent.h"
#include "GP4/GP4.h"
#include "GP4/LevelStreaming/GP_RestartComp.h"


// Sets default values
AGP_PuzzlePiece_Blob::AGP_PuzzlePiece_Blob() {
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Physic"));
	SetRootComponent(BoxCollider);

	//BoxCollider->SetupAttachment(SceneComponent);

	// Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	// Mesh->SetupAttachment(BoxCollider);

	DestructibleMesh = CreateDefaultSubobject<UDestructibleComponent>(TEXT("DestructibleMesh"));
	DestructibleMesh->SetupAttachment(BoxCollider);

	RestartComp = CreateDefaultSubobject<UGP_RestartComp>(TEXT("RestartComp"));

}

// Called when the game starts or when spawned
void AGP_PuzzlePiece_Blob::BeginPlay() {
	Super::BeginPlay();

}

// Called every frame
void AGP_PuzzlePiece_Blob::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	Velocity.Z -= Gravity * DeltaTime;

	FVector MoveDelta = FVector(0, 0, Velocity.Z) * DeltaTime;

	FHitResult HitResults;

	AddActorWorldOffset(MoveDelta, true, &HitResults);

	MoveDelta -= MoveDelta * HitResults.Time;


	for (int i = 0; i < 4; i++) {
		if (HitResults.bBlockingHit) {

			MoveDelta -= FVector::DotProduct(MoveDelta, HitResults.Normal) * HitResults.Normal;

			if (HitResults.bStartPenetrating) {

				FVector DepenVector = HitResults.Normal * (HitResults.PenetrationDepth + 0.1f);
				AddActorWorldOffset(DepenVector);
				// IsGrounded = false;
			}

			if (FVector::DotProduct(HitResults.ImpactNormal, FVector::UpVector) >
				FMath::Cos(FMath::DegreesToRadians(AngleToGlideAt))) {
				if (IsGrounded == false && Velocity.Z < 100.0f) {
					OnLanding(HitResults);
				}
				
				Velocity.Z = 0;
				IsGrounded = true;
				// LOG("Grounded!")


				CollisionResultHandling(HitResults.Actor.Get());

			} 
		} else if (Velocity.Z < 0) {
			IsGrounded = false;
			// LOG("Not Grounded!")
		}
	
		
		
		MoveDelta.X = 0.0f;
		MoveDelta.Y = 0.0f;
		AddActorWorldOffset(MoveDelta, true, &HitResults);
		MoveDelta -= MoveDelta * HitResults.Time;
	}

	
	// FHitResult Result = BoxCastDown();
	//
	// if (Result.bBlockingHit == true) {
	// 	IsGrounded = true;
	// 	FVector Location = GetActorLocation();
	// 	Location.Z = CollidePosition.Z + BoxCollider->Bounds.BoxExtent.Z;
	// 	SetActorLocation(Location);
	// }
	// else {
	// 	IsGrounded = false;
	// }
}

void AGP_PuzzlePiece_Blob::Explode(float BaseDamage, FVector HitLocation, float DamageRadius, float ImpulseStrength,
                                   bool FullDamage) {
	DestructibleMesh->Activate();
	DestructibleMesh->ApplyRadiusDamage(BaseDamage, HitLocation, DamageRadius, ImpulseStrength, FullDamage);

	OnExplode();
}
