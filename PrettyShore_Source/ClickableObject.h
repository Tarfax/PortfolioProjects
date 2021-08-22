// I like sand

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ClickableObject.generated.h"

class U_Building_DataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInteractEvent, AActor*, ActorReference, U_Building_DataAsset*, DataAsset);

UENUM(Blueprintable)
enum ClickType { Building, Human };

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAM9ASSEMBLE_API UClickableObject : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UClickableObject();

	UFUNCTION(BlueprintCallable)
	void CallEvent(AActor* ActorReference, U_Building_DataAsset* DataAsset);

	UFUNCTION(BlueprintCallable)
	void UpdateUIElementPosition();

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<ClickType> typeOfClickable;

	UPROPERTY(BlueprintAssignable)
	FInteractEvent TriggerInteractEvent;

	UPROPERTY(BlueprintAssignable)
	FInteractEvent ClearUI;
};
