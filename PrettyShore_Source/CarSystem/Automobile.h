#pragma once
#include <stdbool.h>

#include "GameFramework/Actor.h"
#include "Automobile.generated.h"

class UStaticMeshComponent;
class USkeletalMeshComponent;
class USplineComponent;
class ARoadSegment;
class AAutomobileSpawner;

UCLASS()
class AAutomobile : public AActor
{
	GENERATED_BODY()
public:
	AAutomobile();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void Setup(AAutomobileSpawner* SpawnerRef);
	void SetTargetSegment(ARoadSegment* NewTargetSegment);
	void ReleaseAutomobile();

	UPROPERTY(VisibleDefaultsOnly)
	USceneComponent* SceneComp;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* CarMeshComp;

	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* PeepMeshComp;

	UPROPERTY(EditAnywhere)
	ARoadSegment* TargetSegment;

	UPROPERTY(EditAnywhere)
	float Speed = 1000.f;

	UPROPERTY(EditAnywhere)
	float Responsiveness = .5f;

private:

	UPROPERTY()
	USplineComponent* TargetSpline;

	UPROPERTY()
	AAutomobileSpawner* Spawner;

	float DistanceAlongSpline = 0.f;

	bool bIsReleased = false;
};
