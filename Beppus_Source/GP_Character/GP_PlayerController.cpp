// Fill out your copyright notice in the Description page of Project Settings.

#include "GP_PlayerController.h"

#include "GP_Pawn_Base.h"
#include "Blueprint/UserWidget.h"
#include "GP4/GP4.h"
#include "GP_CameraPawn.h"

#include "GP4/FG_SaveGame/GP_SaveGame.h"
#include "GP4/FG_SaveGame/GP_SaveGameManager.h"
#include "GP4/LevelStreaming/GP_LevelInstanceData.h"
#include "GP4/GP_Minions/GP_PatrollingAIController.h"
#include "GP4/GP_Minions/GP_MinionCommands/GP_MinionCommand_Base.h"
#include "GP4/GP_Minions/GP_Minion.h"
#include "GP4/GP_Minions/GP_MinionCommands/GP_MinionCommand_CreateBlob.h"
#include "GP4/GP_Minions/GP_MinionCommands/GP_MinionCommand_CreatePortal.h"
#include "GP4/GP_Minions/GP_MinionCommands/GP_MinionCommand_Explode.h"
#include "GP4/GP_Minions/GP_MinionCommands/GP_MinionCommand_Possess.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/TypeContainer.h"

AGP_PlayerController::AGP_PlayerController() {
	SetShowMouseCursor(true);
	DefaultMouseCursor = EMouseCursor::Default;
	// bEnableClickEvents = true;
	bAutoManageActiveCameraTarget = false;
	bEnableMouseOverEvents = false;
}

void AGP_PlayerController::ReceiveLevelInstanceData(AGP_LevelInstanceData* data) {
	LevelData = data;
	CameraPawn->SetLevelInstanceData(data);
	OnReceivedLevelInstanceData(data);
	CameraPawn->MoveToPlayerStart();

}

void AGP_PlayerController::BeginPlay() {
	Super::BeginPlay();
	if (CameraPawn != nullptr) {
		SetViewTarget(CameraPawn);
	}
	MouseCursor_Default = CreateWidget<UUserWidget>(GetWorld(), MouseCursor_Default_Class);
	MouseCursor_Selected = CreateWidget<UUserWidget>(GetWorld(), MouseCursor_Selected_Class);
	// SetMouseCursorWidget(EMouseCursor::Default, MouseCursor_Default);
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);
	// PopulateActionBar(LevelData);
	GameMode = GetWorld()->GetAuthGameMode<AGP4GameModeBase>();
	//ensure(GameMode);
	//GameMode->SaveGameManager->LoadGame();

}

void AGP_PlayerController::Reset() {
	Super::Reset();
	PossessedPawn = CameraPawn;
	if (MinionPawn != nullptr) {
		MinionPawn->MinionAiController->Possess(MinionPawn);
		MinionPawn = nullptr;
	}
	Possess(CameraPawn);
	SetViewTarget(CameraPawn);

	//	GameMode->SaveGameManager->UpdateSave(GameMode, LevelData->LevelNumber);

}

void AGP_PlayerController::SetupInputComponent() {
	Super::SetupInputComponent();
	this->InputComponent->BindAction(TEXT("Escape"), IE_Pressed, this, &AGP_PlayerController::HandleESCInput);
	this->InputComponent->BindAxis(TEXT("Up"), this, &AGP_PlayerController::HandleUpInput);
	this->InputComponent->BindAxis(TEXT("Right"), this, &AGP_PlayerController::HandleRightInput);
	this->InputComponent->BindAction(TEXT("SpaceBar"), IE_Pressed, this, &AGP_PlayerController::HandleSpaceInput);
	this->InputComponent->BindAxis(TEXT("Zoom"), this, &AGP_PlayerController::HandleZoomInput);
	this->InputComponent->BindAction(TEXT("LeftMouseButton"),
	                                 IE_Pressed,
	                                 this,
	                                 &AGP_PlayerController::HandleLeftMouseButtonPressed);
	this->InputComponent->BindAction(TEXT("RightMouseButton"),
	                                 IE_Pressed,
	                                 this,
	                                 &AGP_PlayerController::HandleRightMouseButtonPressed);
	this->InputComponent->BindAction(TEXT("DoubleClick"), IE_DoubleClick, this,
	                                 &AGP_PlayerController::HandleDoubleClick);
	ActionBinding_CommandPossessPressed = this->InputComponent->BindAction(
		TEXT("Possess"),
		IE_Pressed,
		this,
		&AGP_PlayerController::HandlePossessInputPressed);
	ActionBinding_BombPressed = this->InputComponent->BindAction(
		TEXT("Bomb"),
		IE_Pressed,
		this,
		&AGP_PlayerController::HandleBombInputPressed);
	ActionBinding_BlobPressed = this->InputComponent->BindAction(
		TEXT("Blob"),
		IE_Pressed,
		this,
		&AGP_PlayerController::HandleBlobInputPressed);
	ActionBinding_PortalPressed = this->InputComponent->BindAction(
		TEXT("Portal"),
		IE_Pressed,
		this,
		&AGP_PlayerController::HandlePortalInputPressed);
	ActionBinding_BombReleased = this->InputComponent->BindAction(
		TEXT("Bomb"),
		IE_Released,
		this,
		&AGP_PlayerController::HandleBombInputReleased);
	ActionBinding_BlobReleased = this->InputComponent->BindAction(
		TEXT("Blob"),
		IE_Released,
		this,
		&AGP_PlayerController::HandleBlobInputReleased);
	ActionBinding_PortalReleased = this->InputComponent->BindAction(
		TEXT("Portal"),
		IE_Released,
		this,
		&AGP_PlayerController::HandlePortalInputReleased);
}

void AGP_PlayerController::HandleShortcutKeyCooldown(float DeltaSeconds) {
	if (IsBombButtonPressed == true) {
		BombButtonTimer += DeltaSeconds;
		OnCommandHeldEvent.Broadcast(ActionBinding_BombPressed.GetActionName(),
		                             BombButtonTimer / CommandPressedThresholdTime);
		if (BombButtonTimer > CommandPressedThresholdTime) {
			HandleBombInputReleased();
		}
	}
	else if (IsBlobButtonPressed == true) {
		BlobButtonTimer += DeltaSeconds;
		OnCommandHeldEvent.Broadcast(ActionBinding_BlobPressed.GetActionName(),
		                             BlobButtonTimer / CommandPressedThresholdTime);
		if (BlobButtonTimer > CommandPressedThresholdTime) {
			HandleBlobInputReleased();
		}
	}
	else if (IsPortalButtonPressed == true) {
		PortalButtonTimer += DeltaSeconds;
		OnCommandHeldEvent.Broadcast(ActionBinding_PortalPressed.GetActionName(),
		                             PortalButtonTimer / CommandPressedThresholdTime);
		if (PortalButtonTimer > CommandPressedThresholdTime) {
			HandlePortalInputReleased();
		}
	}
}

void AGP_PlayerController::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
	CheckForMouseOver();
	if (IsPressingCommandKey == true) {
		HandleShortcutKeyCooldown(DeltaSeconds);
	}
	if (DoBombFollowMousePosition == true) {
		BombFollowMousePosition();
	}
}

void AGP_PlayerController::OnPossess(APawn* InPawn) {
	Super::OnPossess(InPawn);
	if (CameraPawn == nullptr) {
		AGP_CameraPawn* Camera = Cast<AGP_CameraPawn>(InPawn);
		CameraPawn = Camera;
	}
	if (InPawn == CameraPawn) {
		PossessedPawn = CameraPawn;
		if (MinionPawn != nullptr) {
			MinionPawn->MinionAiController->Possess(MinionPawn);
			MinionPawn = nullptr;
		}
	}
	else if (MinionPawn == nullptr) {
		MinionPawn = Cast<AGP_Minion>(InPawn);
		if (MinionPawn != nullptr) {
			PossessedPawn = MinionPawn;
			CameraPawn->FollowMinion(MinionPawn);
			OnPossessingMinion();
		}
	}
	SetViewTarget(CameraPawn);
}

void AGP_PlayerController::OnUnPossess() {
	Super::OnUnPossess();
	if (PossessedPawn != CameraPawn) {
		if (MinionPawn != nullptr) {
			MinionPawn->MinionAiController->Possess(MinionPawn);
			MinionPawn = nullptr;
		}
		Possess(CameraPawn);
		OnUnPossessingMinion();
	}
	SetViewTarget(CameraPawn);
}

void AGP_PlayerController::HandleDoubleClick() {
	//	LOG("DONT FUCKING DARE DOUBLE CLICK MUTHA FUCKA!")

}

void AGP_PlayerController::HandleBombInputPressed() {
	if (IsPressingCommandKey == false) {
		BombButtonTimer = 0.0f;
		IsBombButtonPressed = true;
		IsPressingCommandKey = true;
	}
}

void AGP_PlayerController::HandleBombInputReleased() {
	if (IsBombButtonPressed == true) {
		if (BombButtonTimer > CommandPressedThresholdTime && MinionPawn != nullptr) {
			IsCommandToPossessedMinion = true;
			ResetButton = true;
		}
		OnCommandReleasedEvent.Broadcast(ActionBinding_BombReleased.GetActionName());
		IsBombButtonPressed = false;
		BombButtonTimer = 0.0f;
		IsPressingCommandKey = false;
		if (ResetButton == true) {
			CancelActionButtonPressed();
			ResetButton = false;
		}
	}
}

void AGP_PlayerController::HandleBlobInputPressed() {
	if (IsPressingCommandKey == false) {
		BlobButtonTimer = 0.0f;
		IsBlobButtonPressed = true;
		IsPressingCommandKey = true;
	}
}

void AGP_PlayerController::HandleBlobInputReleased() {
	if (IsBlobButtonPressed == true) {
		if (BlobButtonTimer > CommandPressedThresholdTime && MinionPawn != nullptr) {
			IsCommandToPossessedMinion = true;
			ResetButton = true;
		}
		OnCommandReleasedEvent.Broadcast(ActionBinding_BlobReleased.GetActionName());
		IsBlobButtonPressed = false;
		BlobButtonTimer = 0.0f;
		IsPressingCommandKey = false;
		if (ResetButton == true) {
			CancelActionButtonPressed();
			ResetButton = false;
		}
	}
}

void AGP_PlayerController::HandlePortalInputPressed() {
	if (IsPressingCommandKey == false) {
		PortalButtonTimer = 0.0f;
		IsPortalButtonPressed = true;
		IsPressingCommandKey = true;
	}
}

void AGP_PlayerController::HandlePortalInputReleased() {
	if (IsPortalButtonPressed == true) {
		if (PortalButtonTimer > CommandPressedThresholdTime && MinionPawn != nullptr) {
			IsCommandToPossessedMinion = true;
			ResetButton = true;
		}
		OnCommandReleasedEvent.Broadcast(ActionBinding_PortalReleased.GetActionName());
		IsPortalButtonPressed = false;
		PortalButtonTimer = 0.0f;
		IsPressingCommandKey = false;
		if (ResetButton == true) {
			CancelActionButtonPressed();
			ResetButton = false;
		}
	}
}

void AGP_PlayerController::HandlePossessInputPressed() {
	if (MinionPawn != nullptr) {
		MinionPawn->ReceivedCommand(UnPossessCommandClass);
		return;
	}
	OnCommandReleasedEvent.Broadcast(ActionBinding_CommandPossessPressed.GetActionName());
}

void AGP_PlayerController::HandleLeftMouseButtonPressed() {
	if (SelectedCommand != nullptr) {
		if (HoveredMinion != nullptr) {
			if (HoveredMinion->ReceivedCommand(SelectedCommand) == true) {
				OnMinionReceivedCommand(SelectedCommand);
				if (SelectedCommand->IsChildOf(UGP_MinionCommand_Explode::StaticClass())) {
					if (InstancedBomb != nullptr) {
						InstancedBomb->ThrowToMinion(HoveredMinion);
						DoBombFollowMousePosition = false;
						InstancedBomb = nullptr;
					}
				}
				CancelActionButtonPressed();
				return;
			}
		}
		return;
	}
	if (SelectedCommand == nullptr && HoveredMinion != nullptr) {
		if (MinionPawn == nullptr) {
			HoveredMinion->ReceivedCommand(PossessCommandClass);
			return;
		}
		if (MinionPawn == HoveredMinion) {
			return;
		}
		MinionPawn->ReceivedCommand(UnPossessCommandClass);
		HoveredMinion->ReceivedCommand(PossessCommandClass);
	}
}

void AGP_PlayerController::HandleRightMouseButtonPressed() {
	if (SelectedCommand != nullptr) {
		CancelActionButtonPressed();
		return;
	}
	if (MinionPawn != nullptr && SelectedCommand == nullptr) {
		HandlePossessInputPressed();
		return;
	}
}

void AGP_PlayerController::HandleUpInput(const float Value) {
	if (PossessedPawn != nullptr) {
		PossessedPawn->HandleUpInput(Value);
		if (MinionPawn != nullptr) {
			CameraPawn->HandleUpInput(Value);
		}
	}
}

void AGP_PlayerController::HandleRightInput(const float Value) {
	if (PossessedPawn != nullptr) {
		PossessedPawn->HandleRightInput(Value);
	}
}

void AGP_PlayerController::HandleSpaceInput() {
	if (PossessedPawn != nullptr) {
		PossessedPawn->HandleSpaceInput();
	}
}

void AGP_PlayerController::HandleESCInput() {
	if (SelectedCommand != nullptr) {
		CancelActionButtonPressed();
		return;
	}
	if (MinionPawn != nullptr && SelectedCommand == nullptr) {
		HandlePossessInputPressed();
		return;
	}
	OnOpenEscMenu();
}

void AGP_PlayerController::HandleZoomInput(float Value) {
	if (PossessedPawn != nullptr)
		PossessedPawn->HandleZoomInput(Value);
}

void AGP_PlayerController::ActionButtonPressed(TSubclassOf<UGP_MinionCommand_Base> CommandBase) {
	if (SelectedCommand != nullptr) {
		CancelActionButtonPressed();
	}
	if (CommandBase == nullptr) {
		return;
	}
	SelectedCommand = CommandBase;
	if (CommandBase == UnPossessCommandClass && MinionPawn != nullptr) {
		MinionPawn->ReceivedCommand(UnPossessCommandClass);
		CancelActionButtonPressed();
		SetMouseCursor(EGP_MouseCursor::Default);
		return;
	}
	if (MinionPawn != nullptr && IsCommandToPossessedMinion == true) {
		if (MinionPawn->ReceivedCommand(CommandBase) == true) {
			if (CommandBase->IsChildOf(UGP_MinionCommand_Explode::StaticClass())) {
				AActor* SpawnedActor = GetWorld()->SpawnActor<AGP_PuzzlePiece_Bomb>(
					BombClass, MinionPawn->GetActorTransform());
				AGP_PuzzlePiece_Bomb* Bomb = Cast<AGP_PuzzlePiece_Bomb>(SpawnedActor);
				Bomb->ThrowToMinion(MinionPawn);
			}
		}
		CancelActionButtonPressed();
		IsCommandToPossessedMinion = false;
		OnActionButtonPressed(CommandBase);
		SetMouseCursor(EGP_MouseCursor::Default);
		return;
	}
	if (CommandBase->IsChildOf(UGP_MinionCommand_Explode::StaticClass())) {
		if (InstancedBomb == nullptr) {
			InstancedBomb = Cast<AGP_PuzzlePiece_Bomb>(GetWorld()->SpawnActor(BombClass));
			DoBombFollowMousePosition = true;
		}
		SetMouseCursor(EGP_MouseCursor::Bomb);
	}
	else if (InstancedBomb != nullptr) {
		InstancedBomb->Destroy();
		InstancedBomb = nullptr;
		DoBombFollowMousePosition = false;
	}
	OnActionButtonPressed(CommandBase);

	if (CommandBase->IsChildOf(UGP_MinionCommand_CreateBlob::StaticClass())) {
		SetMouseCursor(EGP_MouseCursor::Blob);
		return;
	}
	if (CommandBase->IsChildOf(UGP_MinionCommand_CreatePortal::StaticClass())) {
		SetMouseCursor(EGP_MouseCursor::Portal);
		return;
	}
	if (CommandBase->IsChildOf(UGP_MinionCommand_Possess::StaticClass())) {
		SetMouseCursor(EGP_MouseCursor::Possess);
	}
	// SetMouseCursorWidget(EMouseCursor::Default, MouseCursor_Selected);
}

void AGP_PlayerController::CancelActionButtonPressed() {
	SetMouseCursor(EGP_MouseCursor::Default);
	if (InstancedBomb != nullptr) {
		InstancedBomb->Destroy();
		InstancedBomb = nullptr;
		DoBombFollowMousePosition = false;
	}
	OnCancelActionButtonPressed();
	SelectedCommand = nullptr;
}

void AGP_PlayerController::HandleHoverEffect(AGP_Minion* Minion) {
	if (Minion != nullptr) {
		if (HoveredMinion != nullptr && HoveredMinion != Minion) {
			HoveredMinion->MouseOverEnd();
			OnUnhoveringMinion();
			if (SelectedCommand == nullptr) {
				SetMouseCursor(EGP_MouseCursor::Default);
			}
		}
		else if (HoveredMinion != nullptr && HoveredMinion == Minion) {
			return;
		}
		Minion->MouseOver(1, SelectedCommand);
		if (SelectedCommand == nullptr) {
			SetMouseCursor(EGP_MouseCursor::Possess);
		}
		OnHoveringMinion(Minion);
		HoveredMinion = Minion;
	}
	else if (HoveredMinion != nullptr) {
		HoveredMinion->MouseOverEnd();
		OnUnhoveringMinion();
		if (SelectedCommand == nullptr) {
			SetMouseCursor(EGP_MouseCursor::Default);
		}
		HoveredMinion = nullptr;
	}
	if (Minion == nullptr && HoveredMinion != nullptr) {
		HoveredMinion->MouseOverEnd();
		OnUnhoveringMinion();
		if (SelectedCommand == nullptr) {
			SetMouseCursor(EGP_MouseCursor::Default);
		}
		HoveredMinion = nullptr;
	}
}

void AGP_PlayerController::BombFollowMousePosition() const {
	if (InstancedBomb != nullptr) {
		if (HoveredMinion != nullptr) {
			const FVector BombPosition = InstancedBomb->GetActorLocation();
			FVector MinionPosition = HoveredMinion->GetActorLocation();
			MinionPosition.Z += 100;
			MinionPosition.X = -400;
			const FVector LerpedPosition = FMath::Lerp(BombPosition, MinionPosition, 0.16f);
			InstancedBomb->SetActorLocation(LerpedPosition, false, nullptr, ETeleportType::TeleportPhysics);
		}
		else {
			const FHitResult Result = TraceForBomb();
			if (Result.bBlockingHit == true) {
				const FVector BombPosition = InstancedBomb->GetActorLocation();
				FVector HitPosition = Result.ImpactPoint;
				HitPosition.Z += 100;
				HitPosition.X = -300;
				const FVector LerpedPosition = FMath::Lerp(BombPosition, HitPosition, 0.16f);
				InstancedBomb->SetActorLocation(LerpedPosition, false, nullptr, ETeleportType::TeleportPhysics);
			}
		}
	}
}

void AGP_PlayerController::CheckForMouseOver() {
	const FHitResult Result = TraceForMinions();
	AGP_Minion* Minion = Cast<AGP_Minion>(Result.Actor.Get());
	HandleHoverEffect(Minion);
}

FHitResult AGP_PlayerController::TraceForMinions() const {
	FVector WorldLocation;
	FVector WorldDirection;
	DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
	FHitResult Result;
	if (GetWorld()->LineTraceSingleByChannel(Result,
	                                         WorldLocation,
	                                         (WorldDirection * 40000) + WorldLocation,
	                                         MinionTraceChannel)) { }
	return Result;
}

FHitResult AGP_PlayerController::TraceForBomb() const {
	FVector WorldLocation;
	FVector WorldDirection;
	DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
	FHitResult Result;
	if (GetWorld()->LineTraceSingleByChannel(Result,
	                                         WorldLocation,
	                                         (WorldDirection * 40000) + WorldLocation,
	                                         BombTraceChannel)) { }
	return Result;
}
