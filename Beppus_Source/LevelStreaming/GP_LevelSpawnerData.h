#pragma once

#include "GP_LevelSpawner.h"
#include "GP4/GP4.h"
#include "GP_LevelSpawnerData.generated.h"


class AGP_LevelInstanceData;

USTRUCT(BlueprintType, Blueprintable)
struct F_LevelStreamingData {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level Refs| The Levels")
	TSoftObjectPtr<UWorld> MainLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level Refs| The Levels")
	TSoftObjectPtr<UWorld> StaticDecorations;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level Refs| The Levels")
	TArray<TSoftObjectPtr<UWorld>> LevelsToLoad;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
	TSubclassOf<AGP_LevelInstanceData> LevelInstanceData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
	TSubclassOf<AGP_LevelSpawner> LevelSpawner;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
	int LevelNumber;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
	FVector LevelLocation;
};

UCLASS(BlueprintType, Blueprintable)
class ULevelSpawnerData : public UDataAsset {
	GENERATED_BODY()
public:
	UFUNCTION(CallInEditor)
	void UpdateLevels();
	void SetLevelLocation(int Index, AGP_LevelInstanceData* LevelInstanceData);
	void SetNextAndPreviousLevels(int Index, AGP_LevelInstanceData* LevelInstanceData);
	int GetLevelSpawnerLevelNumber(AGP_LevelSpawner* LevelSpawner);
	int GetLevelInstanceDataLevelNumber(AGP_LevelInstanceData* LevelInstanceData);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Levels")
	TSoftObjectPtr<UWorld> MainStreamingLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Levels")
	TSoftObjectPtr<UWorld> MainLightingLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Levels")
	TArray<F_LevelStreamingData> Levels;

};



USTRUCT(BlueprintType, Blueprintable)
struct FNumbersInStruct {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="References")
	TSubclassOf<AGP_LevelInstanceData> LevelInstanceData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="References")
	TSubclassOf<AGP_LevelSpawner> LevelSpawner;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="References")
	int LevelNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	F_LevelStreamBounds CameraBounds;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector LevelPosition;
};



UCLASS(BlueprintType, Blueprintable)
class UTestTestData : public UDataAsset {
	GENERATED_BODY()
public:
	UFUNCTION(CallInEditor)
	void Dothingy();
	UFUNCTION(CallInEditor)
	void ChangeLevelNumberInBlueprint();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Levels")
	float NumberThatWillChange = 3.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Levels")
	TArray<FNumbersInStruct> Struct;

};