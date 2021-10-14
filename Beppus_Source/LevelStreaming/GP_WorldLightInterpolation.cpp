// I like sand
#include "GP4/LevelStreaming/GP_WorldLightInterpolation.h"


#include "Engine/DirectionalLight.h"
#include "Components/DirectionalLightComponent.h"
#include "Engine/ExponentialHeightFog.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Engine/PostProcessVolume.h"
#include "Components/LightComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Engine/SkyLight.h"
#include "GP4/GP4GameModeBase.h"

// Sets default values
AGP_WorldLightInterpolation::AGP_WorldLightInterpolation() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGP_WorldLightInterpolation::BeginPlay() {
	Super::BeginPlay();
	AGP4GameModeBase* GameMode = Cast<AGP4GameModeBase>(GetWorld()->GetAuthGameMode());
	GameMode->WorldLightInterpolation = this;
	
	MasterLight.DirectionalLight->GetLightComponent()->MarkRenderStateDirty();
	MasterLight.SkyLight->GetLightComponent()->MarkRenderStateDirty();
	MasterLight.PostProcess->MarkComponentsRenderStateDirty();
	MasterLight.ExponentialHeightFog->MarkComponentsRenderStateDirty();
}

void AGP_WorldLightInterpolation::LerpLightingData(float DeltaTime) {

	if (LerpToNewLight == true) {
		LightLerpTimer -= DeltaTime;
		float Lerp = (LightLerpTimer) / (LightLerpTime);

		// UE_LOG(LogTemp, Log, TEXT("LERP: %f"), Lerp);
		
		 Lerp = FMath::Lerp(1.0f, 0.0f, Lerp);

		SetDirectionalLightValues(InterpolateDirectionalLight(OldLight.DirectionalLight,
															ReferenceLight.DirectionalLight, Lerp));
		SetSkyLightValues(InterpolateSkyLight(OldLight.SkyLight,
											ReferenceLight.SkyLight, Lerp));

		SetPostProcessValues(InterpolatePostProcess(OldLight.PostProcess, ReferenceLight.PostProcess, Lerp));
		SetExponentialHeightFogValues(
			InterpolateExponentialHeightFog(OldLight.ExponentialHeightFog, ReferenceLight.ExponentialHeightFog,
											Lerp));
		// SetSkyAtmosphereValues(
		// 	InterpolateSkyAtmosphere(OldLight.SkyAtmosphere, ReferenceLight.SkyAtmosphere, Lerp));

		if (LightLerpTimer <= 0.0f) {
			LerpToNewLight = false;
			LightLerpTimer = LightLerpTime;
			float One = 1.0f;
			CopyToOldDirectionalLightValues(InterpolateDirectionalLight(ReferenceLight.DirectionalLight, ReferenceLight.DirectionalLight, One));
			CopyToOldPostProcessValues(InterpolatePostProcess(ReferenceLight.PostProcess, ReferenceLight.PostProcess, One));
			CopyToOldSkyLightValues(InterpolateSkyLight(ReferenceLight.SkyLight, ReferenceLight.SkyLight, One));
			CopyToOldExponentialHeightFogValues(InterpolateExponentialHeightFog(ReferenceLight.ExponentialHeightFog, ReferenceLight.ExponentialHeightFog, One));
		}

	}  
	

}

// Called every frame
void AGP_WorldLightInterpolation::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);


		LerpLightingData(DeltaTime);
}

#if WITH_EDITORONLY_DATA
void AGP_WorldLightInterpolation::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {

	}

#endif

void AGP_WorldLightInterpolation::SetNewLight(FLightComponents Lights) {
	LerpToNewLight = true;
	ReferenceLight = Lights;
	if (ReferenceLight.DirectionalLight == nullptr) {
		LerpToNewLight = false;
	}
}

FDirectionalLightData AGP_WorldLightInterpolation::InterpolateDirectionalLight(ADirectionalLight* DirectionalLightA,
                                                                             ADirectionalLight* DirectionalLightB,
                                                                             float& LerpTime) const {

	FDirectionalLightData DirectionalLightData;

	if (DirectionalLightA == nullptr || DirectionalLightB == nullptr) {
		return DirectionalLightData;
	}

	ULightComponent* DLA = DirectionalLightA->GetLightComponent();
	ULightComponent* DLB = DirectionalLightB->GetLightComponent();

	//Light
	DirectionalLightData.Intensity = FMath::Lerp(DLA->Intensity, DLB->Intensity, LerpTime);
	DirectionalLightData.Color = FMath::Lerp(DLA->GetLightColor(),
	                                         DLB->GetLightColor(), LerpTime);

	DirectionalLightData.Temperature = FMath::Lerp(DLA->Temperature, DLB->Temperature, LerpTime);
	DirectionalLightData.IndirectLighting = FMath::Lerp(DLA->IndirectLightingIntensity, DLB->IndirectLightingIntensity,
	                                                    LerpTime);

	DirectionalLightData.VolumetricScattering = FMath::Lerp(DLA->VolumetricScatteringIntensity,
	                                                        DLB->VolumetricScatteringIntensity, LerpTime);

	//Light Shaft
	DirectionalLightData.BloomScale = FMath::Lerp(DLA->BloomScale, DLB->BloomScale, LerpTime);
	DirectionalLightData.BloomThreshold = FMath::Lerp(DLA->BloomThreshold, DLB->BloomThreshold, LerpTime);;
	DirectionalLightData.BloomTint = FMath::Lerp(DLA->BloomTint.ReinterpretAsLinear(),
	                                             DLB->BloomTint.ReinterpretAsLinear(), LerpTime);
	return DirectionalLightData;
}

FSkyLightData AGP_WorldLightInterpolation::InterpolateSkyLight(ASkyLight* SkyLightA,
                                                             ASkyLight* SkyLightB,
                                                             float& LerpTime) const {

	FSkyLightData SkyLightData;

	if (SkyLightA == nullptr || SkyLightB == nullptr) {
		return SkyLightData;
	}

	USkyLightComponent* DLA = SkyLightA->GetLightComponent();
	USkyLightComponent* DLB = SkyLightB->GetLightComponent();

	//Light
	SkyLightData.IntensityScale = FMath::Lerp(DLA->Intensity, DLB->Intensity, LerpTime);
	SkyLightData.Color = FMath::Lerp(DLA->GetLightColor(),
	                                 DLB->GetLightColor(), LerpTime);
	SkyLightData.VolumetricScattering = FMath::Lerp(DLA->VolumetricScatteringIntensity,
	                                                DLB->VolumetricScatteringIntensity, LerpTime);
	SkyLightData.LowerHemisphereColor = FMath::Lerp(DLA->LowerHemisphereColor, DLB->LowerHemisphereColor, LerpTime);
	return SkyLightData;
}

FPostProcessData AGP_WorldLightInterpolation::InterpolatePostProcess(APostProcessVolume* PostProcessA,
                                                                   APostProcessVolume* PostProcessB,
                                                                   float& LerpTime) const {

	FPostProcessData PostProcessData;

	if (PostProcessA == nullptr || PostProcessB == nullptr) {
		return PostProcessData;
	}

	FPostProcessSettings PPA = PostProcessA->Settings;
	FPostProcessSettings PPB = PostProcessB->Settings;

	//Bloom                                                                                                      
	PostProcessData.BloomIntensity = FMath::Lerp(PPA.BloomIntensity, PPB.BloomIntensity, LerpTime);
	PostProcessData.BloomThreshold = FMath::Lerp(PPA.BloomThreshold, PPB.BloomThreshold, LerpTime);

	//Auto Exposure                                                                                                      
	// PostProcessData.AutoExposureMinBrightness = FMath::Lerp(PPA.AutoExposureMinBrightness,
	//                                                         PPB.AutoExposureMinBrightness, LerpTime);
	// PostProcessData.AutoExposureMaxBrightness = FMath::Lerp(PPA.AutoExposureMaxBrightness,
	//                                                         PPB.AutoExposureMaxBrightness, LerpTime);

	//Ambient Occlusion                                                                                                      
	PostProcessData.AmbientCubemapIntensity = FMath::Lerp(PPA.AmbientCubemapIntensity, PPB.AmbientCubemapIntensity,
	                                                      LerpTime);
	PostProcessData.AmbientOcclusionRadius = FMath::Lerp(PPA.AmbientOcclusionRadius, PPB.AmbientOcclusionRadius,
	                                                     LerpTime);

	//Indirect Lighting                                                                                                      
	PostProcessData.IndirectLightingColor = FMath::Lerp(PPA.IndirectLightingColor,
	                                                    PPB.IndirectLightingColor, LerpTime);
	PostProcessData.IndirectLightingIntensity = FMath::Lerp(PPA.IndirectLightingIntensity,
	                                                        PPB.IndirectLightingIntensity, LerpTime);

	//Color Correction                                                                                                      
	PostProcessData.ColorSaturationShadows = FMath::Lerp(PPA.ColorSaturationShadows, PPB.ColorSaturationShadows,
	                                                     LerpTime);
	PostProcessData.ColorContrastShadows = FMath::Lerp(PPA.ColorContrastShadows, PPB.ColorContrastShadows, LerpTime);
	PostProcessData.ColorGammaShadows = FMath::Lerp(PPA.ColorGammaShadows, PPB.ColorGammaShadows, LerpTime);
	PostProcessData.ColorSaturationMidtones = FMath::Lerp(PPA.ColorSaturationMidtones, PPB.ColorSaturationMidtones,
	                                                      LerpTime);
	PostProcessData.ColorSaturationHighlights = FMath::Lerp(PPA.ColorSaturationHighlights,
	                                                        PPB.ColorSaturationHighlights, LerpTime);
	PostProcessData.ColorContrastHighlights = FMath::Lerp(PPA.ColorContrastHighlights, PPB.ColorContrastHighlights,
	                                                      LerpTime);

	return PostProcessData;
}

F_ExponentialHeightFogData AGP_WorldLightInterpolation::InterpolateExponentialHeightFog(
	AExponentialHeightFog* ExponentialHeightFogA,
	AExponentialHeightFog* ExponentialHeightFogB,
	float& LerpTime) {

	F_ExponentialHeightFogData ExponentialHeightFogData;

	if (ExponentialHeightFogA == nullptr || ExponentialHeightFogB == nullptr) {
		return ExponentialHeightFogData;
	}

	UExponentialHeightFogComponent* EHFA = ExponentialHeightFogA->GetComponent();
	UExponentialHeightFogComponent* EHFB = ExponentialHeightFogB->GetComponent();

	//Fog                                                                                                      
	ExponentialHeightFogData.FogDensity = FMath::Lerp(EHFA->FogDensity, EHFB->FogDensity, LerpTime);
	ExponentialHeightFogData.FogHeightFalloff = FMath::Lerp(EHFA->FogHeightFalloff, EHFB->FogHeightFalloff, LerpTime);
	ExponentialHeightFogData.SecondFogDensity = FMath::Lerp(EHFA->SecondFogData.FogDensity,
	                                                        EHFB->SecondFogData.FogDensity, LerpTime);
	ExponentialHeightFogData.SecondFogHeightFalloff = FMath::Lerp(EHFA->SecondFogData.FogHeightFalloff,
	                                                              EHFB->SecondFogData.FogHeightFalloff, LerpTime);
	ExponentialHeightFogData.SecondFogHeightOffset = FMath::Lerp(EHFA->SecondFogData.FogHeightOffset,
	                                                             EHFB->SecondFogData.FogHeightOffset, LerpTime);

	ExponentialHeightFogData.FogInscatteringColor = FMath::Lerp(EHFA->FogInscatteringColor, EHFB->FogInscatteringColor,
	                                                            SmoothStep(LerpTime));
	ExponentialHeightFogData.FogCutoutDistance =
		FMath::Lerp(EHFA->FogCutoffDistance, EHFB->FogCutoffDistance, LerpTime);
	ExponentialHeightFogData.DirectionalInscatteringExponent = FMath::Lerp(
		EHFA->DirectionalInscatteringExponent, EHFB->DirectionalInscatteringExponent, LerpTime);
	ExponentialHeightFogData.DirectionalInscatteringColor = FMath::Lerp(
		EHFA->DirectionalInscatteringColor, EHFB->DirectionalInscatteringColor, LerpTime);
	ExponentialHeightFogData.VolumetricFogScatteringDistribution = FMath::Lerp(
		EHFA->VolumetricFogScatteringDistribution, EHFB->VolumetricFogScatteringDistribution, LerpTime);

	ExponentialHeightFogData.VolumetricFogAlbedo = FMath::Lerp(
		EHFA->VolumetricFogAlbedo.ReinterpretAsLinear(),
		EHFB->VolumetricFogAlbedo.ReinterpretAsLinear(), LerpTime);

	ExponentialHeightFogData.VolumetricFogExtinctionScale = FMath::Lerp(EHFA->VolumetricFogExtinctionScale,
	                                                                    EHFB->VolumetricFogExtinctionScale, LerpTime);
	ExponentialHeightFogData.VolumetricFogViewDistance = FMath::Lerp(
		EHFA->VolumetricFogDistance, EHFB->VolumetricFogDistance,
		LerpTime);
	ExponentialHeightFogData.VolumetricFogEmissive = FMath::Lerp(EHFA->VolumetricFogEmissive,
	                                                             EHFB->VolumetricFogEmissive, LerpTime);
	return ExponentialHeightFogData;
}

F_SkyAtmosphere AGP_WorldLightInterpolation::InterpolateSkyAtmosphere(ASkyAtmosphere* SkyAtmosphereA,
                                                                    ASkyAtmosphere* SkyAtmosphereB,
                                                                    float& LerpTime) const {

	F_SkyAtmosphere SkyAtmosphereData;

	if (SkyAtmosphereA == nullptr || SkyAtmosphereB == nullptr) {
		return SkyAtmosphereData;
	}

	USkyAtmosphereComponent* SkyA = SkyAtmosphereA->GetComponent();
	USkyAtmosphereComponent* SkyB = SkyAtmosphereB->GetComponent();

	//Light
	SkyAtmosphereData.GroundAlbedo = FLinearColor::LerpUsingHSV(SkyA->GroundAlbedo.ReinterpretAsLinear(),
	                                                            SkyB->GroundAlbedo.ReinterpretAsLinear(),
	                                                            LerpTime).ToFColor(true);
	SkyAtmosphereData.AtmosphereHeight = FMath::Lerp(SkyA->AtmosphereHeight, SkyB->AtmosphereHeight, LerpTime);
	SkyAtmosphereData.RayleighScatteringScale = FMath::Lerp(SkyA->RayleighScatteringScale,
	                                                        SkyB->RayleighScatteringScale, LerpTime);
	SkyAtmosphereData.RayleighScattering = FMath::Lerp(SkyA->RayleighScattering,
	                                                   SkyB->RayleighScattering, LerpTime);
	SkyAtmosphereData.RayleighExponentialDistribution = FMath::Lerp(SkyA->RayleighExponentialDistribution,
	                                                                SkyB->RayleighExponentialDistribution, LerpTime);
	SkyAtmosphereData.MieScatteringScale = FMath::Lerp(SkyA->MieScatteringScale, SkyB->MieScatteringScale, LerpTime);
	SkyAtmosphereData.MieScattering = FMath::Lerp(SkyA->MieScattering, SkyB->MieScattering, LerpTime);
	SkyAtmosphereData.MieAbsorptionScale = FMath::Lerp(SkyA->MieAbsorptionScale, SkyB->MieAbsorptionScale, LerpTime);
	SkyAtmosphereData.MieAbsorption = FMath::Lerp(SkyA->MieAbsorption, SkyB->MieAbsorption, LerpTime);
	SkyAtmosphereData.MieAnisotropy = FMath::Lerp(SkyA->MieAnisotropy, SkyB->MieAnisotropy, LerpTime);
	SkyAtmosphereData.MieExponentialDistribution = FMath::Lerp(SkyA->MieExponentialDistribution,
	                                                           SkyB->MieExponentialDistribution, LerpTime);
	SkyAtmosphereData.SkyLuminanceFactor = FMath::Lerp(SkyA->SkyLuminanceFactor,
	                                                   SkyB->SkyLuminanceFactor, LerpTime);
	SkyAtmosphereData.AerialPespectiveViewDistanceScale = FMath::Lerp(SkyA->AerialPespectiveViewDistanceScale,
	                                                                  SkyB->AerialPespectiveViewDistanceScale,
	                                                                  LerpTime);
	SkyAtmosphereData.HeightFogContribution = FMath::Lerp(SkyA->HeightFogContribution, SkyB->HeightFogContribution,
	                                                      LerpTime);
	SkyAtmosphereData.TransmittanceMinLightElevationAngle = FMath::Lerp(SkyA->TransmittanceMinLightElevationAngle,
	                                                                    SkyB->TransmittanceMinLightElevationAngle,
	                                                                    LerpTime);
	SkyAtmosphereData.AerialPerspectiveStartDepth = FMath::Lerp(SkyA->AerialPerspectiveStartDepth,
	                                                            SkyB->AerialPerspectiveStartDepth, LerpTime);

	return SkyAtmosphereData;
}

void AGP_WorldLightInterpolation::SetDirectionalLightValues(FDirectionalLightData LightData) const {
	auto LightComponent = MasterLight.DirectionalLight->GetLightComponent();

	LightComponent->SetIntensity(LightData.Intensity);
	LightComponent->SetLightColor(LightData.Color);
	LightComponent->SetTemperature(LightData.Temperature);
	//LightComponent->SetIndirectLightingIntensity (LightData.IndirectLighting);
	LightComponent->SetVolumetricScatteringIntensity(LightData.VolumetricScattering);
	LightComponent->SetBloomScale(LightData.BloomScale);
	LightComponent->SetBloomThreshold(LightData.BloomThreshold);
	LightComponent->SetBloomTint(LightData.BloomTint.ToFColor(true));
}

void AGP_WorldLightInterpolation::SetSkyLightValues(FSkyLightData LightData) const {
	auto LightComponent = MasterLight.SkyLight->GetLightComponent();

	LightComponent->SetIntensity(LightData.IntensityScale);
	LightComponent->SetLightColor(LightData.Color);
	LightComponent->SetVolumetricScatteringIntensity(LightData.VolumetricScattering);
	LightComponent->SetLowerHemisphereColor(LightData.LowerHemisphereColor);
	// LightComponent->RecaptureSky();
}

void AGP_WorldLightInterpolation::SetPostProcessValues(FPostProcessData LightData) const {
	FPostProcessSettings& Settings = MasterLight.PostProcess->Settings;

	Settings.BloomIntensity = LightData.BloomIntensity;
	Settings.BloomThreshold = LightData.BloomThreshold;
	Settings.AutoExposureMinBrightness = LightData.AutoExposureMinBrightness;
	Settings.AutoExposureMaxBrightness = LightData.AutoExposureMaxBrightness;
	Settings.ColorSaturationShadows = LightData.ColorSaturationShadows;
	Settings.ColorContrastShadows = LightData.ColorContrastShadows;
	Settings.ColorGammaShadows = LightData.ColorGammaShadows;
	Settings.ColorSaturationMidtones = LightData.ColorSaturationMidtones;
	Settings.ColorSaturationHighlights = LightData.ColorSaturationHighlights;
	Settings.ColorContrastHighlights = LightData.ColorContrastHighlights;
	Settings.AmbientCubemapIntensity = LightData.AmbientCubemapIntensity;
	Settings.AmbientOcclusionRadius = LightData.AmbientOcclusionRadius;
	Settings.IndirectLightingColor = LightData.IndirectLightingColor;
	Settings.IndirectLightingIntensity = LightData.IndirectLightingIntensity;
}

void AGP_WorldLightInterpolation::SetExponentialHeightFogValues(F_ExponentialHeightFogData LightData) const {
	UExponentialHeightFogComponent* Settings = MasterLight.ExponentialHeightFog->GetComponent();

	FExponentialHeightFogData FogData;
	FogData.FogDensity = LightData.SecondFogDensity;
	FogData.FogHeightFalloff = LightData.SecondFogHeightFalloff;
	FogData.FogHeightOffset = LightData.SecondFogHeightOffset;
	Settings->SecondFogData = FogData;

	Settings->SetFogDensity(LightData.FogDensity);
	Settings->SetFogHeightFalloff(LightData.FogHeightFalloff);
	Settings->SetFogInscatteringColor(LightData.FogInscatteringColor);
	Settings->SetFogCutoffDistance(LightData.FogCutoutDistance);
	// Settings->SecondFogData.FogDensity = LightData.SecondFogDensity;
	// Settings->SecondFogData.FogHeightFalloff = LightData.SecondFogHeightFalloff;
	// Settings->SecondFogData.FogHeightOffset = LightData.SecondFogHeightOffset;

	Settings->SetDirectionalInscatteringExponent(LightData.DirectionalInscatteringExponent);
	Settings->SetDirectionalInscatteringColor(LightData.DirectionalInscatteringColor);
	Settings->SetVolumetricFogScatteringDistribution(LightData.VolumetricFogScatteringDistribution);
	Settings->SetVolumetricFogAlbedo(LightData.VolumetricFogAlbedo.ToFColor(true));
	Settings->SetVolumetricFogExtinctionScale(LightData.VolumetricFogExtinctionScale);
	Settings->SetVolumetricFogDistance(LightData.VolumetricFogViewDistance);
	Settings->SetVolumetricFogEmissive(LightData.VolumetricFogEmissive);
}

void AGP_WorldLightInterpolation::CopyToOldDirectionalLightValues(FDirectionalLightData LightData) const {
	auto LightComponent = OldLight.DirectionalLight->GetLightComponent();

	LightComponent->SetIntensity(LightData.Intensity);
	LightComponent->SetLightColor(LightData.Color);
	LightComponent->SetTemperature(LightData.Temperature);
	//LightComponent->SetIndirectLightingIntensity (LightData.IndirectLighting);
	LightComponent->SetVolumetricScatteringIntensity(LightData.VolumetricScattering);
	LightComponent->SetBloomScale(LightData.BloomScale);
	LightComponent->SetBloomThreshold(LightData.BloomThreshold);
	LightComponent->SetBloomTint(LightData.BloomTint.ToFColor(true));
}

void AGP_WorldLightInterpolation::CopyToOldSkyLightValues(FSkyLightData LightData) const {
	auto LightComponent = OldLight.SkyLight->GetLightComponent();

	LightComponent->SetIntensity(LightData.IntensityScale);
	LightComponent->SetLightColor(LightData.Color);
	LightComponent->SetVolumetricScatteringIntensity(LightData.VolumetricScattering);
	LightComponent->SetLowerHemisphereColor(LightData.LowerHemisphereColor);
	// LightComponent->RecaptureSky();
}

void AGP_WorldLightInterpolation::CopyToOldPostProcessValues(FPostProcessData LightData) const {
	FPostProcessSettings& Settings = OldLight.PostProcess->Settings;

	Settings.BloomIntensity = LightData.BloomIntensity;
	Settings.BloomThreshold = LightData.BloomThreshold;
	Settings.AutoExposureMinBrightness = LightData.AutoExposureMinBrightness;
	Settings.AutoExposureMaxBrightness = LightData.AutoExposureMaxBrightness;
	Settings.ColorSaturationShadows = LightData.ColorSaturationShadows;
	Settings.ColorContrastShadows = LightData.ColorContrastShadows;
	Settings.ColorGammaShadows = LightData.ColorGammaShadows;
	Settings.ColorSaturationMidtones = LightData.ColorSaturationMidtones;
	Settings.ColorSaturationHighlights = LightData.ColorSaturationHighlights;
	Settings.ColorContrastHighlights = LightData.ColorContrastHighlights;
	Settings.AmbientCubemapIntensity = LightData.AmbientCubemapIntensity;
	Settings.AmbientOcclusionRadius = LightData.AmbientOcclusionRadius;
	Settings.IndirectLightingColor = LightData.IndirectLightingColor;
	Settings.IndirectLightingIntensity = LightData.IndirectLightingIntensity;
}

void AGP_WorldLightInterpolation::CopyToOldExponentialHeightFogValues(F_ExponentialHeightFogData LightData) const {
	UExponentialHeightFogComponent* Settings = OldLight.ExponentialHeightFog->GetComponent();

	FExponentialHeightFogData FogData;
	FogData.FogDensity = LightData.SecondFogDensity;
	FogData.FogHeightFalloff = LightData.SecondFogHeightFalloff;
	FogData.FogHeightOffset = LightData.SecondFogHeightOffset;
	Settings->SecondFogData = FogData;

	Settings->SetFogDensity(LightData.FogDensity);
	Settings->SetFogHeightFalloff(LightData.FogHeightFalloff);
	Settings->SetFogInscatteringColor(LightData.FogInscatteringColor);
	Settings->SetFogCutoffDistance(LightData.FogCutoutDistance);
	// Settings->SecondFogData.FogDensity = LightData.SecondFogDensity;
	// Settings->SecondFogData.FogHeightFalloff = LightData.SecondFogHeightFalloff;
	// Settings->SecondFogData.FogHeightOffset = LightData.SecondFogHeightOffset;

	Settings->SetDirectionalInscatteringExponent(LightData.DirectionalInscatteringExponent);
	Settings->SetDirectionalInscatteringColor(LightData.DirectionalInscatteringColor);
	Settings->SetVolumetricFogScatteringDistribution(LightData.VolumetricFogScatteringDistribution);
	Settings->SetVolumetricFogAlbedo(LightData.VolumetricFogAlbedo.ToFColor(true));
	Settings->SetVolumetricFogExtinctionScale(LightData.VolumetricFogExtinctionScale);
	Settings->SetVolumetricFogDistance(LightData.VolumetricFogViewDistance);
	Settings->SetVolumetricFogEmissive(LightData.VolumetricFogEmissive);
}

// void AGP_WorldLightInterpolation::SetSkyAtmosphereValues(F_SkyAtmosphere LightData) const {
// 	USkyAtmosphereComponent* Settings = MasterLight.SkyAtmosphere->GetComponent();
//
// 	Settings->GroundAlbedo = LightData.GroundAlbedo;
// 	Settings->AtmosphereHeight = LightData.AtmosphereHeight;
//
// 	Settings->SetRayleighScatteringScale(LightData.RayleighScatteringScale);
// 	Settings->SetRayleighScattering(LightData.RayleighScattering);
// 	Settings->SetRayleighExponentialDistribution(LightData.RayleighExponentialDistribution);
// 	Settings->SetMieScatteringScale(LightData.MieScatteringScale);
// 	Settings->SetMieScattering(LightData.MieScattering);
// 	Settings->SetMieAbsorptionScale(LightData.MieAbsorptionScale);
// 	Settings->SetMieAbsorption(LightData.MieAbsorption);
// 	Settings->SetMieAnisotropy(LightData.MieAnisotropy);
// 	Settings->SetMieExponentialDistribution(LightData.MieExponentialDistribution);
// 	Settings->SetSkyLuminanceFactor(LightData.SkyLuminanceFactor);
// 	Settings->SetAerialPespectiveViewDistanceScale(LightData.AerialPespectiveViewDistanceScale);
// 	Settings->SetHeightFogContribution(LightData.HeightFogContribution);
// 	Settings->TransmittanceMinLightElevationAngle = LightData.TransmittanceMinLightElevationAngle;
// 	Settings->AerialPerspectiveStartDepth = LightData.AerialPerspectiveStartDepth;
// }

float AGP_WorldLightInterpolation::SmoothStep(float X) {
	return X * X * (3.f - 2.f * X);
}

float AGP_WorldLightInterpolation::GetCurrentTimeInDegrees(float CurrentTime) const {
	const float X = CurrentTime / 1000.0f;
	//const float InvSmooth = X + (X - (X * X * (3.f - 2.f * X)));
	const float InvSmooth = InverseSmoothStep(X);
	return FMath::Lerp(0.f, 359.9f, InvSmooth);
}

float AGP_WorldLightInterpolation::InverseSmoothStep(const float X) const {
	return .5f - sinf(asinf(1.f - 2.f * X) / 3.f);
}
