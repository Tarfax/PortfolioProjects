// I like sand
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CosmeticAnimals.generated.h"

class AActor;
class USkeletalMesh;
class USplineComponent;
class USceneComponent;

UCLASS()
class TEAM9ASSEMBLE_API ACosmeticAnimals : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ACosmeticAnimals();
	
	UPROPERTY(BlueprintReadWrite)
	USceneComponent* SceneComponent;

	UPROPERTY(BlueprintReadWrite)
	TArray<USplineComponent*> AnimalPaths;

	UPROPERTY(EditAnywhere)
	float TravelSpeed = 200.0f;

	UPROPERTY(EditAnywhere)
	bool loopMotion;

	UPROPERTY(EditAnywhere)
	bool IsMegaCrab = false;

	virtual void Tick(float DeltaTime) override;

private:
	USplineComponent* activePath;
	int RandomPath = 0;
	float traveledDistance = 0.0f;
};
