// Fill out your copyright notice in the Description page of Project Settings.


#include "GP_DestructibleMesh.h"
#include "DestructibleComponent.h"


// Sets default values
AGP_DestructibleMesh::AGP_DestructibleMesh() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;
	// BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	// BoxCollider->SetupAttachment(SceneComponent);
	
	DestructibleMeshComp = CreateDefaultSubobject<UDestructibleComponent>(TEXT("DestructibleMesh"));
	DestructibleMeshComp->SetupAttachment(SceneComponent);
}

// Called when the game starts or when spawned
void AGP_DestructibleMesh::BeginPlay() {
	Super::BeginPlay();
}

// Called every frame
void AGP_DestructibleMesh::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AGP_DestructibleMesh::Explode(float BaseDamage, FVector HitLocation, float DamageRadius, float ImpulseStrength,
	bool FullDamage) {
	DestructibleMeshComp->Activate();
	DestructibleMeshComp->ApplyRadiusDamage(BaseDamage, HitLocation, DamageRadius, ImpulseStrength, FullDamage);
}

