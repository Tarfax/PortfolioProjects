#include "GP_LevelSpawnerData.h"

#include "GP_LevelBoundComponent.h"
#include "GP_LevelInstanceData.h"
#include "Chaos/ChaosDebugDraw.h"

#include "GP4/FG_SaveGame/GP_SaveGameManager.h"


void ULevelSpawnerData::UpdateLevels(){
	UGP_SaveGameManager* SaveGameManager = UGP_SaveGameManager::Get();
	for ( int i = 0; i < Levels.Num(); i++ ){
		Levels[i].LevelNumber = i;
		AGP_LevelInstanceData* levelInstanceData = Cast<AGP_LevelInstanceData>(
			Levels[i].LevelInstanceData->GetDefaultObject());
		AGP_LevelSpawner* levelSpawner = Cast<AGP_LevelSpawner>(Levels[i].LevelSpawner->GetDefaultObject());

		levelInstanceData->LevelSpawnerData = this;
		levelSpawner->LevelSpawnerData = this;
		levelSpawner->LevelNumber = i;
		levelInstanceData->LevelNumber = i;

		if ( SaveGameManager->CurrentSave->LevelHighScore.Contains(i) ){
			levelInstanceData->MinionsInGoal = SaveGameManager->CurrentSave->LevelHighScore.FindRef(i);
		}
		else{
			SaveGameManager->CurrentSave->LevelHighScore.Add(i, levelInstanceData->MinionsInGoal);
		}
		SetLevelLocation(i, levelInstanceData);
		SetNextAndPreviousLevels(i, levelInstanceData);
	}
}

void ULevelSpawnerData::SetLevelLocation( int Index, AGP_LevelInstanceData* LevelInstanceData ){
	FVector levelPosition;

	float YPosition = 0;
	if ( Index > 0 ){
		AGP_LevelInstanceData* PrevLevel = Cast<AGP_LevelInstanceData>(
			Levels[Index - 1].LevelInstanceData->GetDefaultObject());
		const float OffsetPreLevel = PrevLevel->BottomRightLevelBoundLocation.Y;
		const float OffsetThisLevel = LevelInstanceData->TopLeftLevelBoundLocation.Y;
		YPosition = Levels[Index - 1].LevelLocation.Y + OffsetPreLevel - OffsetThisLevel;
	}

	levelPosition.X = 0;
	levelPosition.Y = YPosition;
	levelPosition.Z = LevelInstanceData->GetAdaptedLevelBounds().BottomRight.Z;

	Levels[Index].LevelLocation = levelPosition;
}

void ULevelSpawnerData::SetNextAndPreviousLevels( int Index, AGP_LevelInstanceData* LevelInstanceData ){
	if ( Index > 0 ){
		LevelInstanceData->PreviousLevel = Levels[Index - 1].MainLevel;
	}

	if ( Index < Levels.Num() - 1 ){
		LevelInstanceData->NextLevel = Levels[Index + 1].MainLevel;
	}
}

int ULevelSpawnerData::GetLevelSpawnerLevelNumber( AGP_LevelSpawner* LevelSpawner ){
	for ( int i = 0; i < Levels.Num(); i++ ){
		if ( LevelSpawner->IsA(Levels[i].LevelSpawner) == true ){
			return i;
		}
	}
	return -1;
}

int ULevelSpawnerData::GetLevelInstanceDataLevelNumber( AGP_LevelInstanceData* LevelInstanceData ){
	for ( int i = 0; i < Levels.Num(); i++ ){
		if ( LevelInstanceData->IsA(Levels[i].LevelInstanceData) == true ){
			UE_LOG(LogTemp, Log, TEXT("Found Level Instance Data Level Number!: %i"), i);
			return i;
		}
	}
	return -1;
}


void UTestTestData::Dothingy(){
	NumberThatWillChange += 1.0f;
	// FVector lastVector = FVector::ZeroVector;
	for ( int i = 0; i < Struct.Num(); i++ ){
		// Struct[i].Location = FVector{0,0,100} + lastVector;
		// lastVector =Struct[i].Location; 
	}
	for ( int i = 0; i < Struct.Num(); i++ ){
		Struct[i].LevelNum = i;

		FVector levelPosition;
		AGP_LevelInstanceData* levelInstanceData = Cast<AGP_LevelInstanceData>(
			Struct[i].LevelInstanceData->GetDefaultObject());
		const float Left = levelInstanceData->GetAdaptedLevelBounds().TopLeft.Y;
		const float Right = levelInstanceData->GetAdaptedLevelBounds().BottomRight.Y;
		const float VectorLength = Left - Right;
		float YPositionPreviousLevel = 0;
		if ( i > 0 ){
			YPositionPreviousLevel = Struct[i - 1].LevelPosition.Y;
		}
		YPositionPreviousLevel += FMath::Abs(VectorLength);

		levelPosition.X = 0;
		levelPosition.Y = YPositionPreviousLevel;
		levelPosition.Z = levelInstanceData->GetAdaptedLevelBounds().BottomRight.Z;

		Struct[i].LevelPosition = levelPosition;
	}


}

void UTestTestData::ChangeLevelNumberInBlueprint(){

	AGP_LevelInstanceData* levelInstanceData = Cast<AGP_LevelInstanceData>(
		Struct[0].LevelInstanceData->GetDefaultObject());
	levelInstanceData->LevelNumber = 0;

}
