// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GP_LevelSpawnerData.h"
#include "GameFramework/Actor.h"
#include "GP4/GP4.h"
// #include "GP4/GP_Minions/GP_MinionCommands/GP_MinionCommand_Base.h"
#include "GP_LevelInstanceData.generated.h"

class UGP_RestartComp;
class AGP_PlayerController;
class UGP_MinionCommand_Base;
class UGP_LevelBoundComponent;

USTRUCT(BlueprintType, Blueprintable)
struct F_ActionButtonData {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture* IconImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ToolTipText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FKey KeyBinding;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGP_MinionCommand_Base> CommandToPerform;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ActionBindingName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ULevel* NextLevel;
};


UCLASS(BlueprintType, Blueprintable)
class ULevelActionButtons : public UDataAsset {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<F_ActionButtonData> ActionButtons;
};

UCLASS(BlueprintType, Blueprintable)
class USaveLevelData : public UDataAsset {
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Data;

};

UCLASS(BlueprintType, Blueprintable)
class ULevelData : public UDataAsset {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Levels")
	TArray<TSoftObjectPtr<UWorld>> Levels;
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMinionInGoal);

UCLASS(Blueprintable, BlueprintType)
class GP4_API AGP_LevelInstanceData : public AActor {
	GENERATED_BODY()

public:
	AGP_LevelInstanceData();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	F_LevelStreamBounds GetAdaptedCameraBounds() const;
	F_LevelStreamBounds GetAdaptedLevelBounds() const;
	virtual void Reset() override;
	UFUNCTION(CallInEditor)
	void SetCameraBounds();
	UFUNCTION(CallInEditor)
	void SetLevelBounds();
	// virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
	// virtual void PostEditMove(bool bFinished) override;
	void SetBounds();
#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
#endif

	UPROPERTY(EditInstanceOnly)
	bool ShowLevelBounds = true;

	
	FTransform GetPlayerStartTransform();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UGP_LevelBoundComponent* SceneComponent;
	UPROPERTY(VisibleAnywhere)
	FVector RootComponentLocation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UGP_LevelBoundComponent* LevelRootComponent;
	UPROPERTY(VisibleAnywhere)
	FVector LevelRootComponentLocation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UGP_LevelBoundComponent* TopLeftLevelBound;
	UPROPERTY(VisibleAnywhere)
	FVector TopLeftLevelBoundLocation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UGP_LevelBoundComponent* BottomRightLevelBound;
	UPROPERTY(VisibleAnywhere)
	FVector BottomRightLevelBoundLocation;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UGP_LevelBoundComponent* CameraRootComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UGP_LevelBoundComponent* TopLeftCameraBound;
	UPROPERTY(VisibleAnywhere)
	FVector TopLeftCameraBoundLocation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UGP_LevelBoundComponent* BottomRightCameraBound;
	UPROPERTY(VisibleAnywhere)
	FVector BottomRightCameraBoundLocation;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UGP_RestartComp* RestartComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ULevelSpawnerData* LevelSpawnerData;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Start Position In Level")
	AActor* PlayerStartPosition;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Buttons")
	ULevelActionButtons* ActionButtonsForLevel;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Levels")
	TSoftObjectPtr<UWorld> PreviousLevel;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Levels")
	TSoftObjectPtr<UWorld> NextLevel;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Settings")
	// FVector4 CameraBounds;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Camera Settings")
	F_LevelStreamBounds LevelCameraBounds;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Level Settings")
	F_LevelStreamBounds LevelBounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Settings")
	float ZoomOutAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Win Conditions")
	int OneStar_MinionsToSave;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Win Conditions")
	int TwoStar_MinionsToSave;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Win Conditions")
	int ThreeStar_MinionsToSave;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Win Conditions")
	int MinionsInGoal;

	UPROPERTY()
	int LevelHighScore = 0;
	UFUNCTION(BlueprintCallable)
	void MinionToGoal();

	UPROPERTY(BlueprintAssignable)
	FMinionInGoal OnMinionEnteredGoal;

	UFUNCTION(BlueprintCallable)
	int StarLevelCompleted();
	UFUNCTION(BlueprintCallable)
	int GetBestStarLevelCompleted( int HighScore );

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level Settings")
	FString LevelName;
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Level Settings")
	int LevelNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool UpdateBounds;

};
