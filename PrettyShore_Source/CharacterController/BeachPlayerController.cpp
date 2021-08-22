// Copyright Epic Games, Inc. All Rights Reserved.

#include "BeachPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Team9Assemble/AI/AILifeguard/AILifeGuard_Controller.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Team9Assemble/AI/AI_BeachCharacter.h"
#include "Team9Assemble/Buildings/BuildingBase.h"
#include "Team9Assemble/Buildings/DummyBuilding.h"
#include "Team9Assemble/AI/Models/AI_BeachArea.h"

ABeachPlayerController::ABeachPlayerController() {
	SetShowMouseCursor(true);
	DefaultMouseCursor = EMouseCursor::Default;
}


void ABeachPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	InputComponent->BindAction(TEXT("LeftMouseButton"), IE_Pressed, this,
	                           &ABeachPlayerController::HandleLeftClick_Pressed);

	InputComponent->BindAction(TEXT("LeftMouseButton"), IE_Released, this,
	                           &ABeachPlayerController::HandleLeftClick_Released);

	InputComponent->BindAction(TEXT("RightMouseButton"), IE_Pressed, this,
	                           &ABeachPlayerController::HandleRightClick_Pressed);

	InputComponent->BindAction(TEXT("RightMouseButton"), IE_Released, this,
	                           &ABeachPlayerController::HandleRightClick_Released);

	InputComponent->BindAction(TEXT("Escape"), IE_Pressed, this, &ABeachPlayerController::HandleEscape_Pressed);
	InputComponent->BindAction(TEXT("Rotate"), IE_Pressed, this, &ABeachPlayerController::HandleRotation_Pressed);

}

// FHitResult ABeachPlayerController::TraceAndReturn_Implementation(ETraceTypeQuery TraceChannel) {
// 	FHitResult Result;
// 	return Result;
// }

FHitResult ABeachPlayerController::TraceAndReturn(ECollisionChannel TraceChannel) const {
	FVector WorldLocation;
	FVector WorldDirection;
	DeprojectMousePositionToWorld(WorldLocation, WorldDirection);

	FHitResult Result;
	if (GetWorld()->LineTraceSingleByChannel(Result, WorldLocation, (WorldDirection * 40000) + WorldLocation,
	                                         TraceChannel)) {
		return Result;
	}

	FHitResult Result2;
	return Result2;
}

void ABeachPlayerController::RemovePopup() {
	if (HasPopupOpen == true) {
		PopupWidget->RemoveFromParent();
		PopupWidget = nullptr;
		HasPopupOpen = false;
		ABuildingBase* SelectedBuilding = Cast<ABuildingBase>(SelectedObject);
		if (SelectedBuilding != nullptr) {
			SelectedBuilding->IsSelected = false;
		}
		SelectedObject = nullptr;
	}
}


// void ABeachPlayerController::AddPopup_Implementation(AActor* Actor) { }

void ABeachPlayerController::TraceForSelectable() {
	FHitResult Result = TraceAndReturn(SelectableTraceChannel);
	TraceForSelectableResult(Result.GetActor());
}

void ABeachPlayerController::ClickOnActor() {
	FHitResult Result = TraceAndReturn(SelectableTraceChannel);
	AActor* Actor = nullptr;

	if (Result.bBlockingHit == true/* && IsValid(Result.GetActor()) == true*/) {
		Actor = Result.Actor.Get();
	}

	if (Actor == nullptr) {
		RemovePopup();
		return;
	}

	if (Actor == SelectedObject) {
		return;
	}

	if (Actor != SelectedObject) {
		RemovePopup();
	}

	if (Actor != nullptr) {
		SelectedObject = Actor;
		AddPopup(SelectedObject);
	}
}

void ABeachPlayerController::PerformBuildingPlacement() {
	if (DummyReference != nullptr) {
		if (DummyReference->BuildingData->IsDraggable) {
			return;
		}
		ABuildingBase* Building = DummyReference->TryPlaceDummyBuilding();
		if (DummyReference->BuildingData->BuildingType == E_BuildingType::Cosmetica ||
			DummyReference->BuildingData->BuildingType == E_BuildingType::VisitorHome ||
			DummyReference->BuildingData->BuildingType == E_BuildingType::Toilet) { }
		else if (Building != nullptr) {
			DespawnBuildable();
		}
	}
}


void ABeachPlayerController::PlayerTick(float DeltaTime) {
	Super::PlayerTick(DeltaTime);
	SetMouseDelta();
	LockMousePosition();

	if (bLockMouse == false) {

		if (IsInBuildMode == true) {
			SetBuildablePositionAtMousePosition();
		}
		else if (IsHoldingLifeguard == true) {
			MoveLifeguardToMousePosition();
		}
		else if (IsInDestructionMode) {
			TraceForDestruction();
		}
		else {
			TraceForSelectable();
		}

		HandleLeftMouseButtonClick();
		HandleRightMouseButtonClick();
		HandleLeftMouseButtonReleased();

		HandleLeftMouseButtonHeld();

		if (HasDummysToPlace == true) {
			DummyPlacementTimer -= DeltaTime;
			if (DummyPlacementTimer <= 0.0f) {
				PlaceDraggableInWorld();
				DummyPlacementTimer = 0.06f;
			}
		}
	}

	LeftMouseButtonPressed = false;
	LeftMouseButtonReleased = false;
	RightMouseButtonPressed = false;
	RightMouseButtonReleased = false;
}

void ABeachPlayerController::HandleLeftMouseButtonClick() {
	if (LeftMouseButtonPressed == true) {
		if (IsInBuildMode == true) {
			if (DummyReference != nullptr) {
				LeftMouseButtonHeld = true;
				StartHoldingMousePosition = DummyReference->GetActorLocation();
			}
			PerformBuildingPlacement();
			return;
		}

		if (IsInDestructionMode == true) {
			DestroyBuilding();
			return;
		}

		if (IsHoldingLifeguard == true) {
			DropLifeguard();
			return;
		}

		if (IsHoldingLeftMouseForPlacement == true) {

			return;
		}

		ClickOnActor();
	}
}

void ABeachPlayerController::HandleRightMouseButtonClick() {
	if (RightMouseButtonPressed == true) {
		if (HasPopupOpen == true) {
			RemovePopup();
		}

		if (IsInBuildMode == true) {
			if (DummyReference != nullptr && IsDraggingDummyWithMouse == true && IsInBuildMode == true) {
				for (int i = 0; i < Dummys.Num(); i++) {
					Dummys[i]->Delete();
				}
				Dummys.Reset();
			}
			DespawnBuildable();
		}

		if (IsHoldingLifeguard == true) {
			if (IsLifeguardPickedUp == false) {
				DespawnLifeguard();
			}
			else {
				DropLifeguard();
			}
		}

		if (IsInDestructionMode == true) {
			ToggleBuildingDestructionMenu(false);
		}
	}
}

void ABeachPlayerController::HandleLeftMouseButtonReleased() {
	if (LeftMouseButtonReleased == true) {
		LeftMouseButtonHeld = false;
		if (DummyReference != nullptr && IsDraggingDummyWithMouse == true && IsInBuildMode == true) {
			if (Dummys.Num() > 0) {
				for (int i = 0; i < Dummys.Num(); i++) {
					DummysToPlace.Add(Dummys[i]);
				}
				HasDummysToPlace = true;
			}
			else {
				DummyReference->TryPlaceDummyBuilding();
			}
			Dummys.Reset();
		}
		IsDraggingDummyWithMouse = false;
		LockDummyXPosition = false;
		LockDummyYPosition = false;
	}
}

void ABeachPlayerController::SetDummyDraggingPosition(FVector StartPosition, int X, int Y) {
	FVector XPosition = FVector(StartPosition.X + (DummyReference->BuildingData->Extent_X * X),
	                            StartPosition.Y + (DummyReference->BuildingData->Extent_X * Y), 0);

	ADummyBuilding* Dummy = Cast<ADummyBuilding>(GetWorld()->SpawnActor(DummyClass));
	Dummy->SetActorLocation(XPosition);
	Dummy->SetActorRotation(DummyReference->GetActorRotation());
	Dummy->CanPlaceFromRaytrace = DummyReference->CanPlaceFromRaytrace;
	Dummys.AddUnique(Dummy);
}

void ABeachPlayerController::HandleLeftMouseButtonHeld() {

	if (LeftMouseButtonHeld == true) {
		if (DummyReference != nullptr && IsInBuildMode == true) {
			if (DummyReference->BuildingData->IsDraggable) {
				IsDraggingDummyWithMouse = true;
				for (int i = 0; i < Dummys.Num(); i++) {
					Dummys[i]->Delete();
				}
				Dummys.Reset();

				FVector NowPosition;
				FHitResult Result = TraceAndReturn(VisibleTraceChannel);
				if (Result.bBlockingHit == true) {
					FVector HitLocation = Result.Location;
					HitLocation.Z = 0;
					NowPosition = ModPosition(HitLocation);
				}
				else {
					NowPosition = ModPosition(DummyReference->GetActorLocation());
				}
				FVector StartPosition = ModPosition(StartHoldingMousePosition);
				FVector Largest = (StartPosition - NowPosition).GetAbs();

				float X = NowPosition.X;
				float Y = NowPosition.Y;
				LockDummyXPosition = false;
				LockDummyYPosition = false;

				if (Largest.X > Largest.Y) {
					LockDummyXPosition = true;
					int steps = FMath::CeilToInt((X - StartPosition.X) / DummyReference->BuildingData->Extent_X);
					if (X > StartHoldingMousePosition.X) {
						for (int i = 0; i < steps; i++) {
							SetDummyDraggingPosition(StartPosition, i, 0);
						}
					}
					else {
						for (int i = 0; i >= steps; i--) {
							SetDummyDraggingPosition(StartPosition, i, 0);
						}
					}
				}
				else {
					LockDummyYPosition = true;

					int steps = FMath::CeilToInt((Y - StartPosition.Y) / DummyReference->BuildingData->Extent_Y);
					if (Y > StartHoldingMousePosition.Y) {
						for (int i = 0; i < steps; i++) {
							SetDummyDraggingPosition(StartPosition, 0, i);
						}
					}
					else {
						for (int i = 0; i >= steps; i--) {
							SetDummyDraggingPosition(StartPosition, 0, i);
						}
					}
				}
			}
		}
	}
}

void ABeachPlayerController::SetLockHideMouseCursor(const bool bLockHideMouse) {
	bLockMouse = bLockHideMouse;
	SetShowMouseCursor(!bLockMouse);
}

FVector2D ABeachPlayerController::RetrieveMouseDelta() const {
	return MouseDelta;
}

void ABeachPlayerController::SetMouseDelta() {
	FVector2D MousePos;
	if (GetMousePosition(MousePos.X, MousePos.Y)) {
		MouseDelta.X = -MousePosLastFrame.X - -MousePos.X;
		MouseDelta.Y = MousePosLastFrame.Y - MousePos.Y;

		MousePosLastFrame = MousePos;
	}
}

void ABeachPlayerController::LockMousePosition() {
	if (bLockMouse) {
		int32 X;
		int32 Y;
		GetViewportSize(X, Y);
		const FVector2D ViewportCenterPos(X * 0.5f, Y * 0.5f);
		SetMouseLocation(ViewportCenterPos.X, ViewportCenterPos.Y);

		MousePosLastFrame = FVector2D(ViewportCenterPos.X, ViewportCenterPos.Y);
	}
}

void ABeachPlayerController::HandleLeftClick_Pressed() {
	LeftMouseButtonPressed = true;
}

void ABeachPlayerController::HandleLeftClick_Released() {
	LeftMouseButtonReleased = true;
}

void ABeachPlayerController::HandleRightClick_Pressed() {
	RightMouseButtonPressed = true;
}

void ABeachPlayerController::HandleRightClick_Released() {
	RightMouseButtonReleased = true;
}

void ABeachPlayerController::HandleEscape_Pressed() {
	if (IsHoldingLifeguard == true) {
		if (IsLifeguardPickedUp == true) {
			DropLifeguard();
			return;
		}
		DespawnLifeguard();
		return;
	}

	if (HasPopupOpen == true) {
		RemovePopup();
		return;
	}

	if (IsInBuildMode == true) {
		DespawnBuildable();
		return;
	}

	if (IsInDestructionMode == true) {
		ToggleBuildingDestructionMenu(false);
		HideDestructBuildingMenu();
		return;
	}

	if (IsBuildMenuOpen == true) {
		ToggleBuildMenu(false);
		HideBuildMenu();
		return;
	}

	if (IsEconomyMenuOpen == true) {
		ToggleEconomyMenu(false);
		HideEconomyMenu();
		return;
	}

	if (IsStaffMenuOpen == true) {
		ToggleStaffMenu(false);
		HideStaffMenu();
		return;
	}

	if (IsDecorMenuOpen == true) {
		ToggleDecorMenu(false);
		HideDecorMenu();
		return;
	}

	ToggleEscapeMenu();
}

void ABeachPlayerController::HandleRotation_Pressed() { }

void ABeachPlayerController::ToggleEscapeMenu() {
	HasEscapeMenuOpen = !HasEscapeMenuOpen;
	if (HasEscapeMenuOpen == true) {
		if (EscMenu == nullptr) {
			EscMenu = CreateWidget<UUserWidget>(GetWorld(), EscMenuClass);
		}
		EscMenu->AddToViewport();
		return;
	}
	EscMenu->RemoveFromParent();
}

void ABeachPlayerController::SpawnBuildable(TSubclassOf<ADummyBuilding> BuildingClass) {
	if (IsInBuildMode == false) {
		if (HasEscapeMenuOpen == true) {
			ToggleEscapeMenu();
		}
		if (HasPopupOpen == true) {
			RemovePopup();
		}
	}
	else if (DummyReference != nullptr && DummyReference->StaticClass() != BuildingClass) {
		DespawnBuildable();
	}

	IsInBuildMode = true;
	DummyReference = Cast<ADummyBuilding>(GetWorld()->SpawnActor(BuildingClass));
	DummyClass = BuildingClass;
}

void ABeachPlayerController::DespawnBuildable() {
	if (DummyReference != nullptr) {
		DummyReference->Delete();
	}
	DummyReference = nullptr;
	IsInBuildMode = false;
}

void ABeachPlayerController::SetBuildablePositionAtMousePosition() {
	FHitResult Result = TraceAndReturn(BuildableTraceChannel);
	if (Result.bBlockingHit == true) {
		AAI_BeachArea* BeachArea = nullptr;
		BeachArea = Cast<AAI_BeachArea>(Result.Actor.Get());

		if (BeachArea != nullptr) {
			DummyReference->CanPlaceFromRaytrace = true;

			FVector NewPosition = ModPosition(Result.Location);
			if (LockDummyXPosition == true) {
				NewPosition.Y = ModPosition(StartHoldingMousePosition).Y;
			}
			if (LockDummyYPosition == true) {
				NewPosition.X = ModPosition(StartHoldingMousePosition).X;
			}
			DummyReference->SetActorLocation(NewPosition);
		}
		else if (DummyReference != nullptr) {
			DummyReference->CanPlaceFromRaytrace = false;
			FVector NewPosition;
			const float X = Result.Location.X;
			const float Y = Result.Location.Y;
			NewPosition.Z = 0;
			NewPosition.X = X;
			NewPosition.Y = Y;
			DummyReference->SetActorLocation(NewPosition);
		}
	}
}

FVector ABeachPlayerController::ModPosition(FVector PositionToMod) {
	const float X = PositionToMod.X;
	const float Y = PositionToMod.Y;

	const int XModInt = FMath::TruncToInt(X);
	const int YModInt = FMath::TruncToInt(Y);

	const int XMod = XModInt % 50;
	const int YMod = YModInt % 50;

	FVector NewPosition;
	NewPosition.X = XModInt - XMod;
	NewPosition.Y = YModInt - YMod;
	NewPosition.Z = 0;

	return NewPosition;
}

void ABeachPlayerController::PlaceDraggableInWorld() {
	if (HasDummysToPlace == true && DummysToPlace.Num() > 0) {
		FRotator Rotation = DummysToPlace[0]->GetActorRotation();
		if (DummysToPlace.Num() % 2 == 1) {
			Rotation.Yaw += 180.0f;
		}
		DummysToPlace[0]->SetActorRotation(Rotation);
		DummysToPlace[0]->TryPlaceDummyBuilding();
		DummysToPlace[0]->Delete();
		DummysToPlace.RemoveAt(0, 1, true);
	}
	else {
		HasDummysToPlace = false;
	}
}

void ABeachPlayerController::ToggleBuildMenu(bool IsOpen) {
	IsBuildMenuOpen = IsOpen;
}

void ABeachPlayerController::ToggleStaffMenu(bool IsOpen) {
	IsStaffMenuOpen = IsOpen;
}

void ABeachPlayerController::ToggleDecorMenu(bool IsOpen) {
	IsDecorMenuOpen = IsOpen;
}

void ABeachPlayerController::ToggleEconomyMenu(bool IsOpen) {
	IsEconomyMenuOpen = IsOpen;
}

void ABeachPlayerController::ToggleBuildingDestructionMenu(bool IsOpen) {
	IsInDestructionMode = IsOpen;

	if (IsOpen == true) {
		if (IsInBuildMode == true) {
			DespawnBuildable();
		}

		if (IsHoldingLifeguard == true) {
			if (IsLifeguardPickedUp == true) {
				DropLifeguard();
			}
			else {
				DespawnLifeguard();
			}
		}

		if (HasPopupOpen == true) {
			RemovePopup();
		}
	}
}

void ABeachPlayerController::DestroyBuilding() {
	if (HoveredBuildingToDestruct != nullptr) {
		HoveredBuildingToDestruct->DeconstructBuilding();
		HoveredBuildingToDestruct = nullptr;
	}
}

void ABeachPlayerController::TraceForDestruction() {
	FHitResult Result = TraceAndReturn(SelectableTraceChannel);

	ABuildingBase* Building = Cast<ABuildingBase>(Result.GetActor());
	if (Building != nullptr) {
		if (Building->BuildingData->BuildingName != "Towel") {
			HoveredBuildingToDestruct = Building;
		}
		else {
			HoveredBuildingToDestruct = nullptr;
		}
	}
	else {
		HoveredBuildingToDestruct = nullptr;
	}
	HoveredBuildingToDestroy(HoveredBuildingToDestruct);
}

void ABeachPlayerController::SpawnLifeguard() {
	if (IsHoldingLifeguard == true) {
		return;
	}
	if (IsInBuildMode == true) {
		DespawnBuildable();
	}
	if (HasPopupOpen == true) {
		RemovePopup();
	}
	if (HasEscapeMenuOpen == true) {
		ToggleEscapeMenu();
	}
	if (IsInDestructionMode == true) {
		ToggleBuildingDestructionMenu(false);
	}

	FHitResult Result = TraceAndReturn(VisibleTraceChannel);

	FVector spawnLocation = Result.Location;
	spawnLocation.Z = 200;
	AAI_BeachCharacter* AICharacter = Cast<AAI_BeachCharacter>(GetWorld()->SpawnActor(LifeguardClass));
	AICharacter->SetActorLocation(spawnLocation);
	HoldingLifeguard = Cast<AAILifeGuard_Controller>(AICharacter->GetController());
	HoldingLifeguard->IsPlayerControlled = true;
	IsHoldingLifeguard = true;
	IsLifeguardPickedUp = false;
}

void ABeachPlayerController::DespawnLifeguard() {
	if (HoldingLifeguard != nullptr) {
		HoldingLifeguard->RegretHire();
		HoldingLifeguard = nullptr;
	}
	IsHoldingLifeguard = false;
}

AAILifeGuard_Controller* ABeachPlayerController::PickupLifeguard() {
	if (SelectedObject != nullptr) {
		AAI_BeachCharacter* AICharacter = Cast<AAI_BeachCharacter>(SelectedObject);
		HoldingLifeguard = Cast<AAILifeGuard_Controller>(AICharacter->GetController());
		if (HoldingLifeguard->SetPlayerControlled() == true) {
			IsHoldingLifeguard = true;
			IsLifeguardPickedUp = true;
		}
	}
	RemovePopup();
	return HoldingLifeguard;
}

void ABeachPlayerController::DropLifeguard() {
	HoldingLifeguard->IsPlayerControlled = false;
	if (IsLifeguardPickedUp == false) {
		HoldingLifeguard->EnterWorld();
	}
	DropLifeguardEvent(HoldingLifeguard);
	IsHoldingLifeguard = false;
	IsLifeguardPickedUp = false;
	HoldingLifeguard = nullptr;
}

void ABeachPlayerController::MoveLifeguardToMousePosition() const {
	FVector Position;
	FVector Direction;
	GetWorld()->GetFirstPlayerController()->DeprojectMousePositionToWorld(Position, Direction);
	FHitResult HitResult;
	ECollisionChannel CollisionChannel = ECC_GameTraceChannel3;
	GetWorld()->LineTraceSingleByChannel(HitResult, Position, Position + (Direction * 40000),
	                                     CollisionChannel);

	const float LockedHeight = 400.f;
	const float CamToCloseThreshold = 3000.f;

	const FVector HitLocation = HitResult.ImpactPoint;
	const FVector StartLocation = HitResult.TraceStart;
	const float DistToCam = (StartLocation - HitLocation).Size();

	// Calculate the angle of the -Direction vector, which points from the Hit.ImpactPoint back towards the camera.
	FVector VecA = FVector::UpVector;
	FVector VecB = -Direction.GetSafeNormal();
	const float AngA = acosf((VecA.X * VecB.X + VecA.Y * VecB.Y + VecA.Z * VecB.Z) /
		(sqrtf(powf(VecA.X, 2) + powf(VecA.Y, 2) + powf(VecA.Z, 2)) *
			sqrtf(powf(VecB.X, 2) + powf(VecB.Y, 2) + powf(VecB.Z, 2))));

	const float AngB = 1.570796f; // 90 degrees in radians.
	const float AngC = UKismetMathLibrary::GetPI() - AngA - AngB; // pi = 180 degrees in radians.

	const float LengthAlongGround = LockedHeight * (sinf(AngA) / sinf(AngC));
	const float LAG = DistToCam < CamToCloseThreshold ? 0.f : LengthAlongGround;

	const FVector DirToCamXY = ((StartLocation * FVector(1.f, 1.f, 0.f)) -
		(HitLocation * FVector(1.f, 1.f, 0.f))).GetSafeNormal();

	const FVector HoldActorOffset = FVector(0.f, 0.f, -200.f);

	const FVector HoverLocation = HitLocation + FVector(LAG * DirToCamXY.X,
	                                                    LAG * DirToCamXY.Y, LockedHeight) + HoldActorOffset;

	const FVector LerpLocation = UKismetMathLibrary::VLerp(
		HoldingLifeguard->AI_BeachCharacter->GetActorLocation(), HoverLocation, .4f);

	// Only move the position of the Lifeguard if inside beach area.
	if (HitResult.bBlockingHit) {
		HoldingLifeguard->AI_BeachCharacter->SetActorLocation(LerpLocation);
	}
}
