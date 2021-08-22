// I like sand

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "World_LightInterpolation.generated.h"

class ADirectionalLight;
class AExponentialHeightFog;
class APostProcessVolume;
class ASkyAtmosphere;
class ASkyLight;
class UVolumetricCloudComponent;
class AWorld_Time;
class AWorld_Light;
class UDirectionalLightComponent;

USTRUCT(BlueprintType)
struct FLightComponents {
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, Category=Lighting)
	ADirectionalLight* DirectionalLight;
	UPROPERTY(EditAnywhere, Category=Lighting)
	AExponentialHeightFog* ExponentialHeightFog;
	UPROPERTY(EditAnywhere, Category=Lighting)
	APostProcessVolume* PostProcess;
	UPROPERTY(EditAnywhere, Category=Lighting)
	ASkyAtmosphere* SkyAtmosphere;
	UPROPERTY(EditAnywhere, Category=Lighting)
	ASkyLight* SkyLight;
};

USTRUCT()
struct FDirectionalLightData {
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, Category=Lighting)
	float Intensity = 0;
	UPROPERTY(EditAnywhere, Category=Lighting)
	FLinearColor Color = FLinearColor::Black;
	UPROPERTY(EditAnywhere, Category=Lighting)
	float Temperature;
	UPROPERTY(EditAnywhere, Category=Lighting)
	float IndirectLighting;
	UPROPERTY(EditAnywhere, Category=Lighting)
	float VolumetricScattering = 0;
	UPROPERTY(EditAnywhere, Category=Lighting)
	float BloomScale = 0;
	UPROPERTY(EditAnywhere, Category=Lighting)
	float BloomThreshold = 0;
	UPROPERTY(EditAnywhere, Category=Lighting)
	FLinearColor BloomTint;
};

USTRUCT()
struct FSkyLightData {
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, Category=Lighting)
	float IntensityScale = 0;
	UPROPERTY(EditAnywhere, Category=Lighting)
	FLinearColor Color = FLinearColor::Black;
	UPROPERTY(EditAnywhere, Category=Lighting)
	float VolumetricScattering = 0;
	UPROPERTY(EditAnywhere, Category=Lighting)
	FLinearColor LowerHemisphereColor;
	// UPROPERTY(EditAnywhere, Category=Lighting)
	// float BloomThreshold = 0;
};

USTRUCT()
struct FPostProcessData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category=Lighting)
	float BloomIntensity;
	UPROPERTY(EditAnywhere, Category=Lighting)
	float BloomThreshold;
	UPROPERTY(EditAnywhere, Category=Lighting)
	float AutoExposureMinBrightness;
	UPROPERTY(EditAnywhere, Category=Lighting)
	float AutoExposureMaxBrightness;
	UPROPERTY(EditAnywhere, Category=Lighting)
	float AmbientCubemapIntensity;
	UPROPERTY(EditAnywhere, Category=Lighting)
	float AmbientOcclusionRadius;

	UPROPERTY(EditAnywhere, Category=Lighting)
	FLinearColor IndirectLightingColor;
	UPROPERTY(EditAnywhere, Category=Lighting)
	float IndirectLightingIntensity;

	UPROPERTY(EditAnywhere, Category=Lighting)
	FVector4 ColorSaturationShadows;
	UPROPERTY(EditAnywhere, Category=Lighting)
	FVector4 ColorContrastShadows;
	UPROPERTY(EditAnywhere, Category=Lighting)
	FVector4 ColorGammaShadows;
	UPROPERTY(EditAnywhere, Category=Lighting)
	FVector4 ColorSaturationMidtones;
	UPROPERTY(EditAnywhere, Category=Lighting)
	FVector4 ColorSaturationHighlights;
	UPROPERTY(EditAnywhere, Category=Lighting)
	FVector4 ColorContrastHighlights;
};

USTRUCT()
struct F_ExponentialHeightFogData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category=Lighting)
	float FogDensity;
	UPROPERTY(EditAnywhere, Category=Lighting)
	float FogHeightFalloff;
	UPROPERTY(EditAnywhere, Category=Lighting)
	float SecondFogDensity;
	UPROPERTY(EditAnywhere, Category=Lighting)
	float SecondFogHeightFalloff;
	UPROPERTY(EditAnywhere, Category=Lighting)
	float SecondFogHeightOffset;
	UPROPERTY(EditAnywhere, Category=Lighting)
	FLinearColor FogInscatteringColor;
	UPROPERTY(EditAnywhere, Category=Lighting)
	float FogCutoutDistance;
	UPROPERTY(EditAnywhere, Category=Lighting)
	float DirectionalInscatteringExponent;
	UPROPERTY(EditAnywhere, Category=Lighting)
	FLinearColor DirectionalInscatteringColor;
	UPROPERTY(EditAnywhere, Category=Lighting)
	float VolumetricFogScatteringDistribution;
	UPROPERTY(EditAnywhere, Category=Lighting)
	FLinearColor VolumetricFogAlbedo;
	UPROPERTY(EditAnywhere, Category=Lighting)
	float VolumetricFogExtinctionScale;
	UPROPERTY(EditAnywhere, Category=Lighting)
	float VolumetricFogViewDistance;
	UPROPERTY(EditAnywhere, Category=Lighting)
	FLinearColor VolumetricFogEmissive;

};

USTRUCT()
struct F_SkyAtmosphere {
	GENERATED_BODY()

	FColor		 GroundAlbedo;
	float 		 AtmosphereHeight;
	float 		 RayleighScatteringScale;
	FLinearColor RayleighScattering;
	float 		 RayleighExponentialDistribution;
	float 		 MieScatteringScale;
	FLinearColor MieScattering;
	float		 MieAbsorptionScale;
	FLinearColor MieAbsorption;
	float 		 MieAnisotropy;
	float 		 MieExponentialDistribution;
	FLinearColor SkyLuminanceFactor;
	float 		 AerialPespectiveViewDistanceScale;
	float 		 HeightFogContribution;
	float 		 TransmittanceMinLightElevationAngle;
	float 		 AerialPerspectiveStartDepth;

};

UCLASS()
class TEAM9ASSEMBLE_API AWorld_LightInterpolation : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWorld_LightInterpolation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void LerpLightingData(float CurrentHour);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
#if WITH_EDITORONLY_DATA
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	UPROPERTY()
	AWorld_Time* WorldTime;

	FDirectionalLightData InterpolateDirectionalLight(ADirectionalLight* DirectionalLightA,
	                                                  ADirectionalLight* DirectionalLightB,
	                                                  float& LerpTime) const;

	FSkyLightData InterpolateSkyLight(ASkyLight* SkyLightA, ASkyLight* SkyLightB, float& LerpTime) const;
	FPostProcessData InterpolatePostProcess(APostProcessVolume* PostProcessA, APostProcessVolume* PostProcessB,
	                                        float& LerpTime) const;
	F_ExponentialHeightFogData InterpolateExponentialHeightFog(AExponentialHeightFog* ExponentialHeightFogA,
	                                                           AExponentialHeightFog* ExponentialHeightFogB,
	                                                           float& LerpTime);
	F_SkyAtmosphere InterpolateSkyAtmosphere(ASkyAtmosphere* SkyAtmosphereA, ASkyAtmosphere* SkyAtmosphereB,
	                                         float& LerpTime) const;


	void SetDirectionalLightValues(FDirectionalLightData LightData) const;
	void SetSkyLightValues(FSkyLightData LightData) const;
	void SetPostProcessValues(FPostProcessData LightData) const;
	void SetExponentialHeightFogValues(F_ExponentialHeightFogData LightData) const;
	void SetSkyAtmosphereValues(F_SkyAtmosphere LightData) const;
	float SmoothStep(float X);
	float GetCurrentTimeInDegrees(float CurrentTime) const;
	float InverseSmoothStep(float X) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Master Lighting")
	FLightComponents MasterLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Lighting")
	FLightComponents MorningLight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Lighting")
	float MorningBeginLerp = 4.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Lighting")
	float MorningStart = 6.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Lighting")
	float MorningEnd = 9.0f;
	UPROPERTY(VisibleAnywhere, Category="Lighting")
	float MorningLerp;
	UPROPERTY(VisibleAnywhere, Category="Lighting")
	bool DoMorningLerp;
	UPROPERTY(EditAnywhere, Category="Lighting")
	bool IsMorningTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Lighting")
	FLightComponents DayLight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Lighting")
	float DaytimeBeginLerp = 7.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Lighting")
	float DaytimeStart = 9.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Lighting")
	float DaytimeEnd = 18.0f;
	UPROPERTY(VisibleAnywhere, Category="Lighting")
	float DayLerp;
	UPROPERTY(VisibleAnywhere, Category="Lighting")
	bool DoDayLerp;
	UPROPERTY(EditAnywhere, Category="Lighting")
	bool IsDayTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Lighting")
	FLightComponents EveningLight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Lighting")
	float EveningBeginLerp = 16.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Lighting")
	float EveningStart = 18.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Lighting")
	float EveningEnd = 21.0f;
	UPROPERTY(VisibleAnywhere, Category="Lighting")
	float EveningLerp;
	UPROPERTY(VisibleAnywhere, Category="Lighting")
	bool DoEveningLerp;
	UPROPERTY(EditAnywhere, Category="Lighting")
	bool IsEveningTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Lighting")
	FLightComponents NightLight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Lighting")
	float NightBeginLerp = 19.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Lighting")
	float NightStart = 21.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Lighting")
	float NightEnd = 6.0f;
	UPROPERTY(VisibleAnywhere, Category="Lighting")
	float NightLerp;
	UPROPERTY(VisibleAnywhere, Category="Lighting")
	bool DoNightLerp;
	UPROPERTY(EditAnywhere, Category="Lighting")
	bool IsNightTime;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category=Lighting)
	ADirectionalLight* NightTimeDirectionalLight;

	UPROPERTY(EditAnywhere)
	bool bUseDebugTime;
	UPROPERTY(EditAnywhere)
	bool bStepNextTime;
	UPROPERTY(EditAnywhere)
	float SetTime;
	UPROPERTY(EditAnywhere)
	AWorld_Light* WorldLightMaster;
	UPROPERTY(EditAnywhere)
	AWorld_Light* WorldLightNight;
	// float NowTime;


};
