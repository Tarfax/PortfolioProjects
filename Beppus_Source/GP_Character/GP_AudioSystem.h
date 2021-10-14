// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GP_CameraPawn.h"
#include "Components/ActorComponent.h"
#include "GP_AudioSystem.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GP4_API UGP_AudioSystem : public UActorComponent {
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGP_AudioSystem();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	void LerpReverbVolume(float Alpha) const;

	UPROPERTY()
	AGP_CameraPawn* CameraPawn;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Camera Settings")
	UReverbEffect* CloseReverbEffect;	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Camera Settings")
	UReverbEffect* FarReverbEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Camera Settings")
	UReverbEffect* MasterReverbEffect;

	float MinZoom;
	float MaxZoom;
	float ZoomValue;
	
};
