// Fill out your copyright notice in the Description page of Project Settings.


#include "GP_SaveGameManager.h"

#include "Kismet/GameplayStatics.h"
UGP_SaveGameManager::UGP_SaveGameManager(){

}
UGP_SaveGameManager* UGP_SaveGameManager::Get(){
	static TWeakObjectPtr<UGP_SaveGameManager> Instance;
	if ( Instance.IsValid() ){
		if ( Instance->CurrentSave == nullptr ){
			Instance->CurrentSave = Instance->LoadGame();
		}
		return Instance.Get();
	}
	Instance = NewObject<UGP_SaveGameManager>();
	if ( Instance->CurrentSave == nullptr ){
		Instance->CurrentSave = Instance->LoadGame();
	}
	return Instance.Get();
}
void UGP_SaveGameManager::AddToTotalMinionsDead(){
	CurrentSave->TotalMinionsDead++;
	SaveGame();
}
int UGP_SaveGameManager::GetTotalMinionsDead(){
	return CurrentSave->TotalMinionsDead;
}

void UGP_SaveGameManager::SetLevelHighScore( int LevelNumber, int Stars ){

	if ( Stars > CurrentSave->LevelHighScore.FindRef(LevelNumber) ){
		CurrentSave->LevelHighScore.Add(LevelNumber, Stars);
		SaveGame();
		return;
	}


}

int UGP_SaveGameManager::GetLevelHighScore( int LevelNumber ){
	if ( CurrentSave->LevelHighScore.Num() > 0 )
		return CurrentSave->LevelHighScore.FindRef(LevelNumber);
	return 0;
}
void UGP_SaveGameManager::UpdateSave( int CurrentLevel ){
	CurrentSave->UpdateSave(CurrentLevel);

}
bool UGP_SaveGameManager::DoesSaveGameExist(){
	return UGameplayStatics::DoesSaveGameExist(SaveName, SaveIndex);
}
bool UGP_SaveGameManager::SaveGame(){
	if ( CurrentSave == nullptr )
		CurrentSave = NewObject<UGP_SaveGame>();

	return UGameplayStatics::SaveGameToSlot(CurrentSave, SaveName, SaveIndex);
}
UGP_SaveGame* UGP_SaveGameManager::LoadGame(){
	if ( DoesSaveGameExist() ){
		TArray<uint8> SaveData;
		UGameplayStatics::LoadDataFromSlot(SaveData, SaveName, SaveIndex);
		USaveGame* tempSave = UGameplayStatics::LoadGameFromMemory(SaveData);
		CurrentSave = Cast<UGP_SaveGame>(tempSave);
		CurrentSave->Load();
	}
	else{
		SaveGame();
	}
	return CurrentSave;
}
bool UGP_SaveGameManager::DeleteGame(){
	CurrentSave = nullptr;
	bool success = UGameplayStatics::DeleteGameInSlot(SaveName, SaveIndex);
	CurrentSave = LoadGame();
	//TODO this is a hack to reset the UI TOTAL Deaths in runtime. Might need its own thing if we use this for more stuff in the future.
	AGP_MinionMasterMindBigBrainController::Get()->OnMinionDied.Broadcast();
	return success;
}
