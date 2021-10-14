// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GP4/GP_Puzzles/GP_PuzzlePiece_Bomb.h"
#include "GP_PlayerController.generated.h"

class AGP4GameModeBase;
class UGP_SaveGame;
class UGP_MinionCommand_UnPossess;
class AGP_Pawn_Base;
class AGP_Minion;
class UGP_MinionCommand_Base;
class AGP_CameraPawn;
class AGP_LevelInstanceData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCommandPressedEvent, FName, KeyBindingName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCommandHeldEvent, FName, KeyBindingName, float, Alpha);

UENUM(BlueprintType)
enum class EGP_MouseCursor : uint8 {
	Default = 0,
	Possess = 1,
	Bomb = 2,
	Blob = 3,
	Portal = 4,
};

UCLASS()
class GP4_API AGP_PlayerController : public APlayerController {
	GENERATED_BODY()

public:
	AGP_PlayerController();

	virtual void BeginPlay() override;
	void HandleShortcutKeyCooldown( float DeltaSeconds );
	virtual void Tick( float DeltaSeconds ) override;
	virtual void OnPossess( APawn* InPawn ) override;
	virtual void OnUnPossess() override;
	virtual void Reset() override;

	virtual void SetupInputComponent() override;
	void HandleLeftMouseButtonPressed();
	void HandleRightMouseButtonPressed();
	void HandleUpInput( float Value );
	void HandleRightInput( float Value );
	void HandleSpaceInput();
	void HandleESCInput();
	UFUNCTION(BlueprintImplementableEvent)
	void OnOpenEscMenu();

	void HandleZoomInput( float Value );

	UFUNCTION(BlueprintImplementableEvent)
	void SetMouseCursor( EGP_MouseCursor NewMouseCursor );

	/* Handle Buttons */
	UPROPERTY(BlueprintAssignable)
	FCommandHeldEvent OnCommandHeldEvent;

	UPROPERTY(BlueprintAssignable)
	FCommandPressedEvent OnCommandReleasedEvent;
	UFUNCTION()
	void HandleDoubleClick();

	void HandleBombInputPressed();
	FInputActionBinding ActionBinding_BombPressed;
	void HandleBombInputReleased();
	FInputActionBinding ActionBinding_BombReleased;

	void HandleBlobInputPressed();
	FInputActionBinding ActionBinding_BlobPressed;
	void HandleBlobInputReleased();
	FInputActionBinding ActionBinding_BlobReleased;

	void HandlePortalInputPressed();
	FInputActionBinding ActionBinding_PortalPressed;
	void HandlePortalInputReleased();
	FInputActionBinding ActionBinding_PortalReleased;

	void HandlePossessInputPressed();
	FInputActionBinding ActionBinding_CommandPossessPressed;

	UFUNCTION(BlueprintCallable)
	void ActionButtonPressed( TSubclassOf<UGP_MinionCommand_Base> CommandBase );
	UFUNCTION(BlueprintImplementableEvent)
	void OnActionButtonPressed( TSubclassOf<UGP_MinionCommand_Base> CommandBase );
	UFUNCTION(BlueprintImplementableEvent)
	void OnCancelActionButtonPressed();
	UFUNCTION(BlueprintCallable)
	void CancelActionButtonPressed();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnReceivedLevelInstanceData( AGP_LevelInstanceData* ActionButtonsInLevelData );

	/* Handle Minion Interaction */

	void HandleHoverEffect( AGP_Minion* Minion );
	UFUNCTION(BlueprintImplementableEvent)
	void OnMinionReceivedCommand( TSubclassOf<UGP_MinionCommand_Base> CommandBase );

	FHitResult TraceForMinions() const;
	FHitResult TraceForBomb() const;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TEnumAsByte<ECollisionChannel> MinionTraceChannel;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TEnumAsByte<ECollisionChannel> BombTraceChannel;
	void BombFollowMousePosition() const;

	void CheckForMouseOver();
	UFUNCTION(BlueprintImplementableEvent)
	void OnHoveringMinion( AGP_Minion* NewHoveredMinion );
	UFUNCTION(BlueprintImplementableEvent)
	void OnUnhoveringMinion();

	UFUNCTION(BlueprintImplementableEvent)
	void OnPossessingMinion();
	UFUNCTION(BlueprintImplementableEvent)
	void OnUnPossessingMinion();

	/* Button Variables */

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float BombButtonTimer;
	bool IsBombButtonPressed;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float BlobButtonTimer;
	bool IsBlobButtonPressed;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float PortalButtonTimer;
	bool IsPortalButtonPressed;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float CommandPossessedTimer;
	bool IsCommandPossessedPressed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CommandPressedThresholdTime;
	bool IsPressingCommandKey;
	UPROPERTY(BlueprintReadWrite)
	bool IsCommandToPossessedMinion;
	bool ResetButton;




	UPROPERTY()
	AGP_CameraPawn* CameraPawn;
	UPROPERTY(BlueprintReadOnly)
	AGP_Minion* MinionPawn;
	UPROPERTY(VisibleAnywhere)
	AGP_Pawn_Base* PossessedPawn;

	UPROPERTY(BlueprintReadWrite)
	AGP_Minion* HoveredMinion;
	UPROPERTY(BlueprintReadWrite)
	AGP_Minion* SelectedMinion;

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UGP_MinionCommand_Base> SelectedCommand;
	UPROPERTY(BlueprintReadWrite)
	bool HasCommand;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSubclassOf<UGP_MinionCommand_Base> UnPossessCommandClass;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSubclassOf<UGP_MinionCommand_Base> PossessCommandClass;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSubclassOf<AGP_PuzzlePiece_Bomb> BombClass;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	AGP_PuzzlePiece_Bomb* InstancedBomb;
	bool DoBombFollowMousePosition;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=MouseCursors)
	UUserWidget* MouseCursor_Default;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=MouseCursors)
	TSubclassOf<UUserWidget> MouseCursor_Default_Class;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=MouseCursors)
	UUserWidget* MouseCursor_Selected;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=MouseCursors)
	TSubclassOf<UUserWidget> MouseCursor_Selected_Class;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=MouseCursors)
	UUserWidget* UI_ActionButtonPossession;
	UPROPERTY(Transient)
	AGP4GameModeBase* GameMode;


	UPROPERTY(BlueprintReadWrite)
	AGP_LevelInstanceData* LevelData;

	UFUNCTION(BlueprintCallable)
	void ReceiveLevelInstanceData( AGP_LevelInstanceData* data );

};
