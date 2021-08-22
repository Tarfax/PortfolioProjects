// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/PlayerController.h"
#include "BeachPlayerController.generated.h"

class ADummyBuilding;
class ABuildingBase;
class AAILifeGuard_Controller;
class AAI_BeachCharacter;

UCLASS()
class ABeachPlayerController : public APlayerController {
	GENERATED_BODY()

public:
	ABeachPlayerController();

	void SetLockHideMouseCursor(const bool bLockHideMouse);
	FVector2D RetrieveMouseDelta() const;

	UPROPERTY(BlueprintReadWrite, Category=CPP)
	bool IsInBuildMode;
	UPROPERTY(BlueprintReadWrite, Category=CPP)
	bool IsBuildMenuOpen;
	UPROPERTY(BlueprintReadWrite, Category=CPP)
	bool IsDecorMenuOpen;
	UPROPERTY(BlueprintReadWrite, Category=CPP)
	bool IsStaffMenuOpen;
	UPROPERTY(BlueprintReadWrite, Category=CPP)
	bool IsEconomyMenuOpen;
	UPROPERTY(BlueprintReadWrite, Category=CPP)
	bool IsInDestructionMode;
	UPROPERTY(BlueprintReadWrite, Category=CPP)
	bool HasMenuOpen;
	UPROPERTY(BlueprintReadWrite, Category=CPP)
	bool IsHoldingLifeguard;
	UPROPERTY(BlueprintReadWrite, Category=CPP)
	bool IsLifeguardPickedUp;
	UPROPERTY(BlueprintReadWrite, Category=CPP)
	bool IsHoldingLeftMouseForPlacement;
	UPROPERTY(BlueprintReadWrite, Category=CPP)
	bool HasPopupOpen;
	UPROPERTY(BlueprintReadWrite, Category=CPP)
	bool HasEscapeMenuOpen;

	UPROPERTY(BlueprintReadWrite, Category=CPP)
	AActor* SelectedObject;
	UPROPERTY(BlueprintReadWrite, Category=CPP)
	ADummyBuilding* DummyReference;
	UPROPERTY(BlueprintReadWrite, Category=CPP)
	AAILifeGuard_Controller* HoldingLifeguard;

	UFUNCTION(BlueprintCallable, Category="Events")
	void HandleLeftClick_Pressed();
	UFUNCTION(BlueprintCallable, Category="Events")
	void HandleLeftClick_Released();
	UFUNCTION(BlueprintCallable, Category="Events")
	void HandleRightClick_Pressed();
	UFUNCTION(BlueprintCallable, Category="Events")
	void HandleRightClick_Released();
	UFUNCTION(BlueprintCallable, Category="ESC Event")
	void HandleEscape_Pressed();
	UFUNCTION(BlueprintCallable, Category="Events")
	void HandleRotation_Pressed();


	void SetMouseDelta();
	void LockMousePosition();
	void ClickOnActor();
	void PerformBuildingPlacement();
	void HandleRightMouseButtonClick();
	void HandleLeftMouseButtonReleased();
	void SetDummyDraggingPosition(FVector StartPosition, int X, int Y);
	void HandleLeftMouseButtonHeld();
	void HandleLeftMouseButtonClick();
	

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=CPP)
	TEnumAsByte<ECollisionChannel> SelectableTraceChannel;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=CPP)
	TEnumAsByte<ECollisionChannel> BuildableTraceChannel;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=CPP)
	TEnumAsByte<ECollisionChannel> VisibleTraceChannel;

	UPROPERTY(BlueprintReadWrite, Category=CPP)
	UUserWidget* PopupWidget;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=CPP)
	TSubclassOf<UUserWidget> EscMenuClass;
	UPROPERTY(BlueprintReadOnly, Category=CPP_ReadOnly)
	UUserWidget* EscMenu;
	
	bool LeftMouseButtonPressed = false;
	bool LeftMouseButtonHeld = false;
	bool LeftMouseButtonReleased = false;
	bool RightMouseButtonPressed = false;
	bool RightMouseButtonReleased = false;

	//Popup
	UFUNCTION(BlueprintCallable)
	void TraceForSelectable();
	UFUNCTION(BlueprintImplementableEvent)
	void TraceForSelectableResult(AActor* Actor);


	UFUNCTION(BlueprintImplementableEvent)
	void AddPopup(AActor* Actor);
	// void AddPopup_Implementation(AActor* Actor);

	UFUNCTION(BlueprintCallable)
	void RemovePopup();
	// void RemovePopup_Implementation();

	UFUNCTION()
	void ToggleEscapeMenu();

	//Buildable
	FVector StartHoldingMousePosition;
	UPROPERTY()
	TArray<ADummyBuilding*> Dummys;

	bool HasDummysToPlace;
	float DummyPlacementTimer;
	TArray<ADummyBuilding*> DummysToPlace;

	TSubclassOf<ADummyBuilding>  DummyClass;
	bool IsDraggingDummyWithMouse;
	bool LockDummyXPosition;
	bool LockDummyYPosition;

	void SetBuildablePositionAtMousePosition();
	FVector ModPosition(FVector PositionToMod);
	void PlaceDraggableInWorld();


	UFUNCTION(BlueprintCallable)
	void SpawnBuildable(TSubclassOf<ADummyBuilding> BuildingClass);

	UFUNCTION(BlueprintCallable)
	void DespawnBuildable();

	//Building Destruction Mode
	UPROPERTY()
	ABuildingBase* HoveredBuildingToDestruct;
	
	UFUNCTION(BlueprintCallable)
	void DestroyBuilding();
	UFUNCTION(BlueprintCallable)
	void TraceForDestruction();
	UFUNCTION(BlueprintImplementableEvent)
	void HoveredBuildingToDestroy(ABuildingBase* Building);


	//Lifeguard
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category=CPP)
	TSubclassOf<AAI_BeachCharacter> LifeguardClass;
	UFUNCTION(BlueprintCallable)
	void SpawnLifeguard();
	UFUNCTION(BlueprintCallable)
	void DespawnLifeguard();
	UFUNCTION(BlueprintCallable)
	AAILifeGuard_Controller* PickupLifeguard();
	UFUNCTION(BlueprintCallable)
	void DropLifeguard();
	UFUNCTION(BlueprintImplementableEvent)
	void DropLifeguardEvent(AAILifeGuard_Controller* Lifeguard);

	//Menus
	UFUNCTION(BlueprintCallable)
	void ToggleBuildMenu(bool IsOpen);
	UFUNCTION(BlueprintImplementableEvent)
	void HideBuildMenu();
	
	UFUNCTION(BlueprintCallable)
	void ToggleDecorMenu(bool IsOpen);
	UFUNCTION(BlueprintImplementableEvent)
	void HideDecorMenu();

	UFUNCTION(BlueprintCallable)
	void ToggleStaffMenu(bool IsOpen);
	UFUNCTION(BlueprintImplementableEvent)
	void HideStaffMenu();
	
	UFUNCTION(BlueprintCallable)
	void ToggleEconomyMenu(bool IsOpen);
	UFUNCTION(BlueprintImplementableEvent)
	void HideEconomyMenu();

	UFUNCTION(BlueprintCallable)
	void ToggleBuildingDestructionMenu(bool IsOpen);
	UFUNCTION(BlueprintImplementableEvent)
	void HideDestructBuildingMenu();

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	void MoveLifeguardToMousePosition() const;
	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	//
	// UFUNCTION(BlueprintNativeEvent)
	// FHitResult TraceAndReturn(ETraceTypeQuery TraceChannel);
	FHitResult TraceAndReturn(ECollisionChannel TraceChannel) const;
	// UFUNCTION(BlueprintCallable)
	// void RemovePopup();


	// void AddPopup_Implementation(AActor* Actor);
	// End PlayerController interface


private:
	FVector2D MousePositionBeforeMiddleMousePress;
	FVector2D MousePosLastFrame;
	FVector2D MouseDelta;
	bool bLockMouse = false;
};
