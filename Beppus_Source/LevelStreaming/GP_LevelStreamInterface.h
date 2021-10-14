// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GP_LevelStreamInterface.generated.h"


// This class does not need to be modified.
UINTERFACE()
class UGP_LevelStreamInterface : public UInterface {
	GENERATED_BODY()
};

/**
 *
 */
class GP4_API IGP_LevelStreamInterface {
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void RestartLevel();
	void RestartLevel_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void GoToPreviousLevel();
	void GoToPreviousLevel_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void GoToNextLevel();
	void GoToNextLevel_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void LoadLevel(FName LevelToLoad);
	void LoadLevel_Implementation(FName LevelToLoad);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UnLoadLevel(FName LevelToUnload);
	void UnLoadLevel_Implementation(FName LevelToUnload);


	UFUNCTION(BlueprintImplementableEvent)
	void OnLevelRestartBegin();
	UFUNCTION(BlueprintImplementableEvent)
	void OnLevelRestartEnd();
	UFUNCTION(BlueprintImplementableEvent)
	void OnTearDownBegin();
	UFUNCTION(BlueprintImplementableEvent)
	void OnTearDownEnd();
	UFUNCTION(BlueprintImplementableEvent)
	void OnLoadBegin();
	UFUNCTION(BlueprintImplementableEvent)
	void OnLoadEnd();



};
