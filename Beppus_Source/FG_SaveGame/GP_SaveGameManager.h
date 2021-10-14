// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GP_SaveGame.h"
#include "SaveGameSystem.h"

#include "UObject/Object.h"
#include "GP_SaveGameManager.generated.h"

/**
 * 
 */
UCLASS()
class GP4_API UGP_SaveGameManager : public UObject {
	GENERATED_BODY()

	UGP_SaveGameManager();
public:
	UFUNCTION(BlueprintCallable)
	static UGP_SaveGameManager* Get();
	UFUNCTION(BlueprintCallable)
	void AddToTotalMinionsDead();
	UFUNCTION(BlueprintCallable)
	int GetTotalMinionsDead();
	UFUNCTION(BlueprintCallable)
	void SetLevelHighScore( int LevelNumber, int Stars );
	UFUNCTION(BlueprintCallable)
	int GetLevelHighScore( int LevelNumber );
	UFUNCTION(BlueprintCallable)
	void UpdateSave( int CurrentLevel );
	UFUNCTION(BlueprintCallable)
	bool DoesSaveGameExist();
	UFUNCTION(BlueprintCallable)
	bool SaveGame();
	UFUNCTION(BlueprintCallable)
	UGP_SaveGame* LoadGame();
	UFUNCTION(BlueprintCallable)
	bool DeleteGame();

	UPROPERTY(BlueprintReadOnly)
	UGP_SaveGame* CurrentSave;
	UPROPERTY(BlueprintReadWrite)
	FString SaveName = FString("Save");
	UPROPERTY(BlueprintReadWrite)
	int32 SaveIndex = 0;


};
