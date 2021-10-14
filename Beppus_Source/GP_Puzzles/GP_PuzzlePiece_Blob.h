#pragma once

//Dsiables a deprication warning that is annoying

#pragma warning(disable : 4996)



#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "GP_PuzzlePiece_Blob.generated.h"

class UBoxComponent;

class UDestructibleComponent;

class UGP_RestartComp;

UCLASS()

class GP4_API AGP_PuzzlePiece_Blob : public AActor {

	GENERATED_BODY()





public:
	// Sets default values for this actor's properties

	AGP_PuzzlePiece_Blob();



	virtual void BeginPlay() override;

	virtual void Tick( float DeltaTime ) override;



	UFUNCTION(BlueprintCallable)

	void Explode( float BaseDamage, FVector HitLocation, float DamageRadius, float ImpulseStrength, bool FullDamage );

	UFUNCTION(BlueprintImplementableEvent)

	void OnExplode();



	UPROPERTY(BlueprintReadOnly, EditAnywhere)

	USceneComponent* SceneComponent;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)

	UBoxComponent* BoxCollider;

	// UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)

	// UBoxComponent* BoxCollider;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)

	UDestructibleComponent* DestructibleMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Level Handling")

	UGP_RestartComp* RestartComp;





	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ExposeOnSpawn = true))

	FVector BoxColliderExtent;



	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)

	FVector Velocity;



	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)

	float Gravity;



	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)

	bool IsGrounded;



	UFUNCTION(BlueprintImplementableEvent)

	void OnLanding( FHitResult Result );



	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)

	bool IsOnElevator;



	UPROPERTY(BlueprintReadWrite, EditAnywhere)

	bool CanEverMove;



	UPROPERTY(EditAnywhere)

	float AngleToGlideAt = 45;



	UFUNCTION(BlueprintImplementableEvent)

	FHitResult BoxCastDown();

	UFUNCTION(BlueprintImplementableEvent)

	FHitResult CollisionResultHandling( AActor* HitActor );



	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)

	FVector CollidePosition;



};
