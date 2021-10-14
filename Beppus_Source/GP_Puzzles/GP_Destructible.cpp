// Fill out your copyright notice in the Description page of Project Settings.


#include "GP_Destructible.h"
#include "DestructibleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GP_DestructibleMesh.h"
#include "Components/BoxComponent.h"
#include "GP4/GP4.h"


// Sets default values
AGP_Destructible::AGP_Destructible() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;
	// BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	// BoxCollider->SetupAttachment(SceneComponent);
	
	DestructibleMeshComp = CreateDefaultSubobject<UDestructibleComponent>(TEXT("DestructibleMesh"));
	DestructibleMeshComp->SetupAttachment(SceneComponent);
}

// Called when the game starts or when spawned
void AGP_Destructible::BeginPlay() {
	Super::BeginPlay();
	
}

// Called every frame
void AGP_Destructible::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	
}

void AGP_Destructible::Explode(float BaseDamage, FVector HitLocation, float DamageRadius, float ImpulseStrength, bool FullDamage) {
	if (IsDestroyed == true) {
		return;
	}
	AGP_DestructibleMesh* Mesh = GetWorld()->SpawnActorDeferred<AGP_DestructibleMesh>(DestructibleMeshClass, SceneComponent->GetComponentTransform(), this);
	Mesh->DestructibleMeshComp->SetDestructibleMesh(DestructibleMeshComp->GetDestructibleMesh());
	Mesh->DestructibleMeshComp->SetWorldTransform(DestructibleMeshComp->GetComponentTransform());
	UGameplayStatics::FinishSpawningActor(Mesh, SceneComponent->GetComponentTransform());

	if (UseCustomImpulse == true) {
		ImpulseStrength = CustomImpulse;
	}
	
	Mesh->Explode(BaseDamage, HitLocation, DamageRadius, ImpulseStrength, FullDamage);
	
	// DestructibleMeshComp->ApplyRadiusDamage(BaseDamage, HitLocation, DamageRadius, ImpulseStrength, FullDamage);
	// BoxCollider->SetCollisionProfileName("NoCollision");
	OnExplode();
	OnExploded.Broadcast();
	IsDestroyed = true;
}