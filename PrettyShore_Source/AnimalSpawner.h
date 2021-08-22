// I like sand
#pragma once

#include "CoreMinimal.h"

#include "CosmeticAnimals.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "AnimalSpawner.generated.h"

class ACosmeticAnimals;
class USceneComponent;

UCLASS()
class TEAM9ASSEMBLE_API AAnimalSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAnimalSpawner();

	UPROPERTY(BlueprintReadOnly)
	USceneComponent* RootObject;
	
	UPROPERTY(BlueprintReadOnly)
	USceneComponent* SeagullPathsParent;

	UPROPERTY(BlueprintReadOnly)
	USceneComponent* CrabPathsParent;

	UPROPERTY(BlueprintReadOnly)
	USceneComponent* FishPathParent;
	
	UFUNCTION(BlueprintCallable, Category="Movement Splines")
	static void GetAllSplines(USceneComponent* SceneComponent, TArray<USplineComponent*>& ReturnValue);
};
