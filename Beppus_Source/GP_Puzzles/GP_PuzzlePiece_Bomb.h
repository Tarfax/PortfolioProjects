#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "GP_PuzzlePiece_Bomb.generated.h"

class UBoxComponent;
class AGP_Minion;
class UGP_RestartComp;
UCLASS()
class AGP_PuzzlePiece_Bomb  : public AActor {
	GENERATED_BODY()
public:
	AGP_PuzzlePiece_Bomb();
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void ThrowToMinion(AGP_Minion* Minion);
	UFUNCTION(BlueprintImplementableEvent)
	void OnThrowToMinion(AGP_Minion* Minion);
	UFUNCTION(BlueprintImplementableEvent)
	void OnExplodeBegin();
	
	UPROPERTY(VisibleAnywhere)
	USceneComponent* SceneComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UGP_RestartComp* RestartComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool StartBombCountdown;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float LerpToMinionTime;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float LerpToMinionTimer;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float BombExplosionTime;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float BombExplosionTimer;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float AlphaTime;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool StartThrow;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AGP_Minion* MinionToExplode;
	
	
};
