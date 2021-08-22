// I like sand

#pragma once

#include "CoreMinimal.h"

#include "BeachWork.h"
#include "GameFramework/Actor.h"
#include "Team9Assemble/AI/Interfaces/AI_Workable.h"

#include "AI_Trash.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class TEAM9ASSEMBLE_API AAI_Trash : public AActor, public IAI_Workable {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAI_Trash();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void FinishWork(UBeachWork* Work) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USceneComponent* SceneComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UBoxComponent* BoxCollider;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* Trash;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UBeachWork* BeachWork;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeToClean = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int DecorationValue = -10;
};
