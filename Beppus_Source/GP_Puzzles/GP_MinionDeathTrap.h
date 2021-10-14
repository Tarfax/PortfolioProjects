// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "GP4/GP_Minions/GP_Minion.h"
#include "GP_MinionDeathTrap.generated.h"

UCLASS()
class GP4_API AGP_MinionDeathTrap : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGP_MinionDeathTrap();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* BoxComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	TSubclassOf<AGP_MinionDeath> TrapDeathTypeClass;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved,
	                       FVector HitLocation, FVector HitNormal, FVector NormalImpulse,
	                       const FHitResult& Hit) override;

	UFUNCTION()
	void MinionHit(AGP_Minion* Minion);
	UFUNCTION(BlueprintImplementableEvent)
	void OnMinionHit(AGP_Minion* Minion);
};
