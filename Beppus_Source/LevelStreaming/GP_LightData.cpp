// Fill out your copyright notice in the Description page of Project Settings.


#include "GP_LightData.h"

#include "Components/ExponentialHeightFogComponent.h"
#include "Components/LightComponent.h"
#include "Components/SkyLightComponent.h"
#include "Engine/DirectionalLight.h"
#include "Engine/ExponentialHeightFog.h"
#include "Engine/PostProcessVolume.h"
#include "Engine/SkyLight.h"


// Sets default values
AGP_LightData::AGP_LightData() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGP_LightData::BeginPlay() {
	Super::BeginPlay(); 
	LightComponents.DirectionalLight->GetLightComponent()->bAffectsWorld = false;
	LightComponents.PostProcess ->bEnabled = false;
	LightComponents.ExponentialHeightFog->GetComponent()->SetVisibility(false);
	LightComponents.SkyLight->GetLightComponent()->bAffectsWorld = false;
	LightComponents.DirectionalLight->GetLightComponent()->MarkRenderStateDirty();
} 

// Called every frame 
void AGP_LightData::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

void AGP_LightData::DeactivateLight() {
	LightComponents.DirectionalLight->GetLightComponent()->bAffectsWorld = false;
	LightComponents.PostProcess ->bEnabled = false;
	LightComponents.ExponentialHeightFog->GetComponent()->SetVisibility(false);
	LightComponents.SkyLight->GetLightComponent()->bAffectsWorld = false;
	LightComponents.DirectionalLight->GetLightComponent()->MarkRenderStateDirty();
}

void AGP_LightData::ActivateLight() {
	LightComponents.DirectionalLight->GetLightComponent()->bAffectsWorld = true;
	LightComponents.PostProcess ->bEnabled = true;
	LightComponents.ExponentialHeightFog->GetComponent()->SetVisibility(true);
	LightComponents.SkyLight->GetLightComponent()->bAffectsWorld = true;
	LightComponents.DirectionalLight->GetLightComponent()->MarkRenderStateDirty();
}

