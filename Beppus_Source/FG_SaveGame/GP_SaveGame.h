// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/SaveGame.h"

#include "GP4/LevelStreaming/GP_LevelInstanceData.h"

#include "GP_SaveGame.generated.h"


USTRUCT(BlueprintType)
struct FGP_SoundSettings {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, SaveGame)
	float MasterVolume = 0.3f;
	UPROPERTY(BlueprintReadWrite, SaveGame)
	float SFXVolume = 0.3f;
	UPROPERTY(BlueprintReadWrite, SaveGame)
	float MusicVolume = 0.3f;
};
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class GP4_API UGP_SaveGame : public USaveGame {
	GENERATED_BODY()
public:
	UPROPERTY(SaveGame, BlueprintReadOnly)
	TMap<int, int> LevelHighScore = {};
	UPROPERTY(SaveGame)
	FGP_SoundSettings SoundSettings;
	UPROPERTY(SaveGame)
	int LastPlayedLevelNumber = 0;
	UPROPERTY(SaveGame)
	UGameUserSettings* UserSettings = nullptr;
	UPROPERTY(SaveGame, BlueprintReadOnly)
	int TotalMinionsDead = 0;
	UPROPERTY(SaveGame)
	bool bIsTutorialComplete = false;
	UPROPERTY(SaveGame)
	bool bAllowInit = true;

	UFUNCTION()
	void InitSave();
	UFUNCTION(BlueprintCallable)
	void UpdateSave( int CurrentLevelNumber );
	UFUNCTION(BlueprintCallable)
	void SaveSoundSettings( float Master, float Sfx, float Music );
	UFUNCTION(BlueprintCallable)
	FGP_SoundSettings LoadSoundSettings();
	UFUNCTION(BlueprintCallable)
	void Load();
	void DEBUG_printSaveGameContent(){
		UE_LOG(LogTemp, Log, TEXT("SoundSettings: %f"), SoundSettings.MasterVolume);
		UE_LOG(LogTemp, Log, TEXT("LastPLayedLevelNumber: %i"), LastPlayedLevelNumber);
		UE_LOG(LogTemp, Log, TEXT("IsTutorialCOmplete: %b"), bIsTutorialComplete);
		UE_LOG(LogTemp, Log, TEXT("AllowInit: %b"), bAllowInit);
		for ( auto& level : LevelHighScore ){
			UE_LOG(LogTemp, Log, TEXT("Level: %i, MinionsInGoal: %i"), level.Key, level.Value)
		}

	}




};
