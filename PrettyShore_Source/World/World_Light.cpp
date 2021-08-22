#include "World_Light.h"
#include "BeachGameState.h"
#include "World_Time.h"
// #include "World_LightInterpolation.h"
// #include "Components/ExponentialHeightFogComponent.h"
#include "Components/LightComponent.h"
#include "Components/SkyLightComponent.h"
#include "Engine/DirectionalLight.h"
// #include "Components/SkyAtmosphereComponent.h"
// #include "Engine/PostProcessVolume.h"
// #include "Engine/SkyLight.h"
// #include "Engine/ExponentialHeightFog.h"
// #include "Components/VolumetricCloudComponent.h"

AWorld_Light::AWorld_Light() {
	PrimaryActorTick.bCanEverTick = true;
}

void AWorld_Light::BeginPlay() {
	Super::BeginPlay();

	// auto* BeachGameState = Cast<ABeachGameState>(GetWorld()->GetGameState());
	// if (BeachGameState == nullptr) {
	// 	UE_LOG(LogTemp, Log, TEXT("World_Light.cpp; BeginPlay: GameState was a nullptr!"))
	// }
	// else {
	// 	BeachGameState->SetWorldLight(this);
	// }

	ensureMsgf(DirectionalLight != nullptr,
	           TEXT(
		           "AWorld_Light.cpp; BeginPlay: DirectionalLight was a nullptr. World_Light needs a reference to a directional light."
	           ));

	SunsetStartNormalized = SunsetStart / 24.f;

	// ToggleLights(false);
}

void AWorld_Light::SetDirectionalLightRotation(float hours, float TimeInDegrees) {
	if (bIsOn == false && hours > StartLight &&
		hours < EndLight) {
		ToggleLights(true);
		bIsOn = true;
	}
	else if (bIsOn == false && bIsNightLight == true && hours > StartLight) {
		ToggleLights(true);
		bIsOn = true;
	}

	if (bIsOn == true && hours > EndLight && bIsNightLight == false) {
		ToggleLights(false);
		bIsOn = false;
	}
	else if (bIsOn == true && StartLight > hours &&
		EndLight < hours) {
		ToggleLights(false);
		bIsOn = false;
	}

	float X = 0;
	float Y = 0;
	float Z = 0;

	if (bIsNightLight == true) {
		X = MoonPositionX.GetRichCurve()->Eval(TimeInDegrees);
		Y = MoonPositionY.GetRichCurve()->Eval(TimeInDegrees);
		Z = MoonPositionZ.GetRichCurve()->Eval(TimeInDegrees);
	}
	else {
		X = SunPositionX.GetRichCurve()->Eval(TimeInDegrees);
		Y = SunPositionY.GetRichCurve()->Eval(TimeInDegrees);
		Z = SunPositionZ.GetRichCurve()->Eval(TimeInDegrees);
	}

	const FRotator LightRotation = FRotator(FRotator::MakeFromEuler(FVector(X, Y, Z)));

	// const FRotator LightRotation = FRotator(FRotator::MakeFromEuler(FVector( GetCurrentTimeInDegrees() + rotation, 80.f, 20.f)));

	DirectionalLight->SetActorRotation(LightRotation);
}

void AWorld_Light::Tick(float DeltaTime) {

	if (WorldTime != nullptr) {
		float hours = WorldTime->GetCurrentHours().houers;
		float TimeInDegrees = GetCurrentTimeInDegrees();

		SetDirectionalLightRotation(hours, TimeInDegrees);

		// if (WorldTime != nullptr)
		// {
		// 	const float NormalizedTime = WorldTime->GetCurrentTimeNormalized();
		// 	if (NormalizedTime >= SunsetStartNormalized)
		// 	{
		// 		// Converts from (0.X --> 0.83) to (0.0 --> 1.0).
		// 		const float RemappedValue = (NormalizedTime - SunsetStartNormalized) / (0.83f - SunsetStartNormalized);
		// 	
		// 		const FLinearColor DisplayColor = FMath::Lerp(DefaultColor, SunsetColor, RemappedValue);
		// 		DirectionalLight->SetLightColor(DisplayColor);
		// 		bHasDefaultColor = false;
		// 	}
		// 	else if(bHasDefaultColor == false)
		// 	{
		// 		DirectionalLight->SetLightColor(DefaultColor);
		// 		bHasDefaultColor = true;
		// 	}
		// }
		// else
		// {
		// 	UE_LOG(LogTemp, Log, TEXT("AWorld_Light::Tick: WorldTime was a null. <---"))
		// }
	} else {
		auto* BeachGameState = Cast<ABeachGameState>(GetWorld()->GetGameState());
		if (BeachGameState == nullptr) {
			UE_LOG(LogTemp, Log, TEXT("World_Light.cpp; BeginPlay: GameState was a nullptr!"))
		}
		else {
			WorldTime = BeachGameState->WorldTime;
		}
	}

}

void AWorld_Light::CompleteSetup(AWorld_Time* worldTime) {
	if (worldTime != nullptr) {
		WorldTime = worldTime;
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("AWorld_Light.cpp; CompleteSetup: Parameter 'worldTime' was a nullptr!"))
	}
}

float AWorld_Light::GetCurrentTimeInDegrees() const {
	if (WorldTime == nullptr) {
		UE_LOG(LogTemp, Log, TEXT("WorldTime is null!"))
		return 0.f;
	}

	const float X = WorldTime->GetCurrentTimeNormalized();
	//const float InvSmooth = X + (X - (X * X * (3.f - 2.f * X)));
	const float InvSmooth = InverseSmoothStep(X);
	return FMath::Lerp(0.f, 359.9f, InvSmooth);
}

float AWorld_Light::InverseSmoothStep(const float X) const {
	return .5f - sinf(asinf(1.f - 2.f * X) / 3.f);
}

// float SmoothStep (float X)
// {
// 	return X * X * (3.f - 2.f * X);
// }


void AWorld_Light::ToggleLights(bool Toggle) {
	// if (bIsNightLight == false) {
	// 	UE_LOG(LogTemp, Log, TEXT("Toggle"));
	// }

	DirectionalLight->GetLightComponent()->bAffectsWorld = Toggle;
	DirectionalLight->GetLightComponent()->SetCastShadows(Toggle);

	// if (bIsNightLight == true && Toggle == false) {
	// 	UE_LOG(LogTemp, Log, TEXT("Toggle nights off"));
	//
	// }
	// else if (bIsNightLight == true && Toggle == true) {
	// 	UE_LOG(LogTemp, Log, TEXT("Toggle nights on"));
	// }
	// SkyLight->GetLightComponent()->bAffectsWorld = Toggle;
	// SkyLight->GetLightComponent()->SetCastShadows(Toggle);
	// SkyAtmosphere->GetComponent()->SetVisibility(Toggle);
	// PostProcess->bEnabled = Toggle;
	// ExponentialHeightFog->GetComponent()->SetVisibility(Toggle);
	// VolumetricCloud->SetVisibility(Toggle);
}

// void AWorld_Light::SetDirectionalLightValues() {
// 	FDirectionalLightData DirectionalLightData = WorldLightInterpolation->InterpolateDirectionalLight();
// }
