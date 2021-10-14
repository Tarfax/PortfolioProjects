// Fill out your copyright notice in the Description page of Project Settings.

#include "GP_SaveGame.h"

#include "GP4/GP4GameModeBase.h"
#include "GameFramework/GameUserSettings.h"

#include "GP4/GP4.h"


void UGP_SaveGame::InitSave(){
	if ( bAllowInit ){
		LOG("INIT SAVE")
		LevelHighScore.Reserve(14);
		bAllowInit = false;
		bIsTutorialComplete = false;
		UserSettings = UGameUserSettings::GetGameUserSettings();
		UserSettings->LoadSettings();
		UserSettings->ValidateSettings();
		UserSettings->ApplySettings(false);
	}
	else{
		LOG("NOT ALLOWED TO INIT SAVE")
		LOG("LOADING GAME INSTEAD")
		Load();

	}


}

void UGP_SaveGame::UpdateSave( int CurrentLevelNumber ){
	LastPlayedLevelNumber = CurrentLevelNumber;
	UserSettings = UGameUserSettings::GetGameUserSettings();
	UserSettings->ValidateSettings();
	UserSettings->SaveSettings();
	DEBUG_printSaveGameContent();

}

void UGP_SaveGame::SaveSoundSettings( float Master, float Sfx, float Music ){

	SoundSettings.MasterVolume = Master;
	SoundSettings.SFXVolume = Sfx;
	SoundSettings.MusicVolume = Music;

}
FGP_SoundSettings UGP_SaveGame::LoadSoundSettings(){
	return SoundSettings;
}



void UGP_SaveGame::Load(){
	
	UserSettings = UGameUserSettings::GetGameUserSettings();
	UserSettings->LoadSettings();
	UserSettings->ValidateSettings();
	UserSettings->ApplySettings(false);
	//TODO Load Sound Settings;
	//TODO UpdateLevelInstanceData
	//GameMode->LevelSpawnerData->UpdateLevels();


}
