// I like sand

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AI_WaterArea.generated.h"

class AAIVisitor_Controller;

UCLASS()
class AAI_WaterArea : public AActor {
	GENERATED_BODY()

public:
	AAI_WaterArea();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
#if WITH_EDITORONLY_DATA
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif


public:
	UFUNCTION(BlueprintCallable)
	FVector FindRandomLocation();

public: //Variables
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBoxComponent* Area;

	UPROPERTY(EditInstanceOnly, Category="Beach Area")
	int SizeX = 2000;
	UPROPERTY(EditInstanceOnly, Category="Beach Area")
	int SizeY = 11000;

};
