// Fill out your copyright notice in the Description page of Project Settings.

#include "GP_MinionDeathTrap.h"

#include "GP4/GP_Minions/GP_Minion.h"

// Sets default values
AGP_MinionDeathTrap::AGP_MinionDeathTrap() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = BoxComponent;
	StaticMesh->SetupAttachment(BoxComponent);
}

// Called when the game starts or when spawned
void AGP_MinionDeathTrap::BeginPlay() { Super::BeginPlay(); }

// Called every frame
void AGP_MinionDeathTrap::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

void AGP_MinionDeathTrap::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp,
                                    bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse,
                                    const FHitResult& Hit) {
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	if (MyComp == RootComponent) {
		AGP_Minion* Minion = Cast<AGP_Minion>(Other);
		if (Minion) { MinionHit(Minion); }
	}
}

void AGP_MinionDeathTrap::MinionHit(AGP_Minion* Minion) {
	OnMinionHit(Minion);
	Minion->Kill(TrapDeathTypeClass);
}
