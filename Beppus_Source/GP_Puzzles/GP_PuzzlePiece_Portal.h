// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "GP_PuzzlePiece_Portal.generated.h"

class UBoxComponent;
class AGP_Minion;
class UGP_RestartComp;
UCLASS()
class GP4_API AGP_PuzzlePiece_Portal : public AActor {
	GENERATED_BODY()
private:
	void DestroyPortal();
	
public:
	AGP_PuzzlePiece_Portal();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnBeingDestroyed();
	UFUNCTION(BlueprintCallable)
	void MarkForDestroy();
	UFUNCTION(BlueprintCallable)
	void Explode();
	UFUNCTION(BlueprintImplementableEvent)
	void OnExplode();
	void RemovePortalConnection();
	void SetPortalConnection(AGP_PuzzlePiece_Portal* Portal); 	

	UPROPERTY(VisibleAnywhere)
	USceneComponent* SceneComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UBoxComponent* BoxCollider;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USceneComponent* TeleportPosition;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Level Handling")
	UGP_RestartComp* RestartComp;

	UPROPERTY(VisibleInstanceOnly)
	bool HasConnection;
	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly)
	bool CanTeleport;
	UPROPERTY(EditAnywhere)
	float TeleportCooldown = 1.0f;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	AGP_PuzzlePiece_Portal* ConnectedPortal;

	UFUNCTION()
	 void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable)
	AGP_Minion* OnMinionBeginOverlap(AGP_Minion* OverlappingMinion);

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FVector Velocity;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float Gravity;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool IsGrounded;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool IsOnElevator;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool CanEverMove;

	UPROPERTY(EditAnywhere)
	float AngleToGlideAt = 45;


	UFUNCTION(BlueprintImplementableEvent)
	FHitResult CollisionResultHandling(AActor* HitActor);
	UFUNCTION(BlueprintImplementableEvent)
	FHitResult BoxCastDown();

	bool IsMarkedForDestroy;
	bool IsExploding;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FVector CollidePosition;

	FTimerHandle TimerHandle;
};
