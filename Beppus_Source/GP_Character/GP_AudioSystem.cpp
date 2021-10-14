// Fill out your copyright notice in the Description page of Project Settings.


#include "GP_AudioSystem.h"

#include "Sound/ReverbEffect.h"


// Sets default values for this component's properties
UGP_AudioSystem::UGP_AudioSystem() {
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.2f;
}


// Called when the game starts
void UGP_AudioSystem::BeginPlay() {
	Super::BeginPlay();
	CameraPawn = GetOwner<AGP_CameraPawn>();
	MinZoom = CameraPawn->ZoomMinDistance;
	MaxZoom = CameraPawn->ZoomMaxDistance;
}


// Called every frame
void UGP_AudioSystem::TickComponent(float DeltaTime, ELevelTick TickType,
                                    FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ZoomValue = CameraPawn->CameraDistance;
	const float Alpha = FMath::Lerp(0.0f, 1.0f, (ZoomValue - MinZoom) / (MaxZoom-MinZoom));
	LerpReverbVolume(Alpha);
	
}

void UGP_AudioSystem::LerpReverbVolume(float Alpha) const {
	const float ReflectionsDelay = FMath::Lerp(CloseReverbEffect->ReflectionsDelay, FarReverbEffect->ReflectionsDelay, Alpha);
	const float GainHF = FMath::Lerp(CloseReverbEffect->GainHF, FarReverbEffect->GainHF, Alpha);
	const float ReflectionsGain = FMath::Lerp(CloseReverbEffect->ReflectionsGain, FarReverbEffect->ReflectionsGain, Alpha);
	
	const float LateDelay = FMath::Lerp(CloseReverbEffect->LateDelay, FarReverbEffect->LateDelay, Alpha);
	const float DecayTime = FMath::Lerp(CloseReverbEffect->DecayTime, FarReverbEffect->DecayTime, Alpha);
	const float Density = FMath::Lerp(CloseReverbEffect->Density, FarReverbEffect->Density, Alpha);
	const float Diffusion = FMath::Lerp(CloseReverbEffect->Diffusion, FarReverbEffect->Diffusion, Alpha);
	const float AirAbsorptionGainHF = FMath::Lerp(CloseReverbEffect->AirAbsorptionGainHF, FarReverbEffect->AirAbsorptionGainHF, Alpha);
	const float DecayHFRatio = FMath::Lerp(CloseReverbEffect->DecayHFRatio, FarReverbEffect->DecayHFRatio, Alpha);
	const float LateGain = FMath::Lerp(CloseReverbEffect->LateGain, FarReverbEffect->LateGain, Alpha);
	const float Gain = FMath::Lerp(CloseReverbEffect->Gain, FarReverbEffect->Gain, Alpha);

	MasterReverbEffect->ReflectionsDelay = ReflectionsDelay;
	MasterReverbEffect->GainHF = GainHF;
	MasterReverbEffect->ReflectionsGain = ReflectionsGain;
	
	MasterReverbEffect->LateDelay = LateDelay;
	MasterReverbEffect->DecayTime = DecayTime;
	MasterReverbEffect->Density = Density;
	MasterReverbEffect->Diffusion = Diffusion;
	MasterReverbEffect->AirAbsorptionGainHF = AirAbsorptionGainHF;
	MasterReverbEffect->DecayHFRatio = DecayHFRatio;
	MasterReverbEffect->LateGain = LateGain;
	MasterReverbEffect->Gain = Gain;
}

