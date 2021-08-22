// I like sand

#pragma once

#include "CoreMinimal.h"

#include "AILifeguard/SensingComponents/VisionSensing.h"
#include "GameFramework/Character.h"
#include "AI_BeachCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetIsInWater, bool, InWater);

class UClickableObject;

UCLASS(BlueprintType)
class AAI_BeachCharacter : public ACharacter {
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAI_BeachCharacter();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY(EditAnywhere)
    UClickableObject* ClickableComp;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FSetIsInWater SetInWater;
	bool bIsInWater;

	UFUNCTION(BlueprintImplementableEvent)
	void OnSetInWater(bool IsInWater);

	UPROPERTY(BlueprintReadWrite)
	bool IsLifeGuard;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UVisionSensing* Vision;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool IsWet;


	UPROPERTY()
	UCharacterMovementComponent* MovementComponent;

	
	UFUNCTION()
	void HandleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 BodyIndex, bool bFromSweep, const FHitResult& SweepHit);

	UFUNCTION()
	void HandleEndOverlap(UPrimitiveComponent* OnComponentEndOverlap, AActor* OtherActor,UPrimitiveComponent* OverlappedComponent, int32 OtherBodyIndex);

};
