#pragma once

#include "CoreMinimal.h"

#include "Curves/CurveLinearColor.h"
#include "GameFramework/Actor.h"
#include "World_Light.generated.h"

class AWorld_Time;
class ADirectionalLight;
class AExponentialHeightFog;
class APostProcessVolume;
class ASkyAtmosphere;
class ASkyLight;
class UVolumetricCloudComponent;
class AWorld_LightInterpolation;

UCLASS()
class TEAM9ASSEMBLE_API AWorld_Light : public AActor
{
	GENERATED_BODY()
public:
	AWorld_Light();


	virtual void BeginPlay() override;
	void SetDirectionalLightRotation(float hours, float TimeInDegrees);
	virtual void Tick(float DeltaTime) override;

	void CompleteSetup(AWorld_Time* worldTime);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Lighting)
	ADirectionalLight* DirectionalLight;
	// UPROPERTY(EditAnywhere, Category=Lighting)
	// AExponentialHeightFog* ExponentialHeightFog;
	// UPROPERTY(EditAnywhere, Category=Lighting)
	// APostProcessVolume* PostProcess;
	// UPROPERTY(EditAnywhere, Category=Lighting)
	// ASkyAtmosphere* SkyAtmosphere;
	// UPROPERTY(EditAnywhere, Category=Lighting)
	// ASkyLight* SkyLight;
	// UPROPERTY(EditAnywhere, Category=Lighting)
	// AActor* VolumetricCloud;
	
	UPROPERTY(EditAnywhere)
	AWorld_Time* WorldTime;
	UPROPERTY(EditAnywhere)
	AWorld_LightInterpolation* WorldLightInterpolation;

	UPROPERTY(EditAnywhere)
	FLinearColor DefaultColor;
	
	UPROPERTY(EditAnywhere)
	FLinearColor SunsetColor;

	UPROPERTY(EditAnywhere)
	float SunsetStart = 18.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRuntimeFloatCurve SunPositionX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRuntimeFloatCurve SunPositionY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRuntimeFloatCurve SunPositionZ;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRuntimeFloatCurve MoonPositionX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRuntimeFloatCurve MoonPositionY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRuntimeFloatCurve MoonPositionZ;
	
	UPROPERTY(EditAnywhere, Category=Lighting)
	float StartLight;
	UPROPERTY(EditAnywhere, Category=Lighting)
	float EndLight;
	UPROPERTY(EditAnywhere, Category=Lighting)
	bool bIsOn;
	UFUNCTION(BlueprintCallable)
	void ToggleLights(bool Toggle);
	// void SetDirectionalLightValues();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Lighting)
	bool bIsNightLight = false;
private:

	float GetCurrentTimeInDegrees() const;
	float InverseSmoothStep(float X) const;
	
	bool bHasDefaultColor = true;
	float SunsetStartNormalized;
};
