// Fill out your copyright notice in the Description page of Project Settings.


#include "GP_LevelInstanceData.h"
#include "GP_LevelBoundComponent.h"
#include "GP_RestartComp.h"
#include "Components/LineBatchComponent.h"
#include "GameFramework/PlayerStart.h"

#include "GP4/FG_SaveGame/GP_SaveGameManager.h"
#include "GP4/GP_Character/GP_PlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AGP_LevelInstanceData::AGP_LevelInstanceData(){
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RestartComp = CreateDefaultSubobject<UGP_RestartComp>(TEXT("RestartComp"));
	SceneComponent = CreateDefaultSubobject<UGP_LevelBoundComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	LevelRootComponent = CreateDefaultSubobject<UGP_LevelBoundComponent>(TEXT("RootLevelComponent"));
	LevelRootComponent->SetupAttachment(SceneComponent);

	TopLeftLevelBound = CreateDefaultSubobject<UGP_LevelBoundComponent>(TEXT("TopLeft CameraBound"));
	TopLeftLevelBound->SetupAttachment(LevelRootComponent);
	BottomRightLevelBound = CreateDefaultSubobject<UGP_LevelBoundComponent>(TEXT("BottomRight CameraBound"));
	BottomRightLevelBound->SetupAttachment(LevelRootComponent);

	CameraRootComponent = CreateDefaultSubobject<UGP_LevelBoundComponent>(TEXT("CameraRootComponent"));
	CameraRootComponent->SetupAttachment(SceneComponent);

	TopLeftCameraBound = CreateDefaultSubobject<UGP_LevelBoundComponent>(TEXT("TopLeft LevelBound"));
	TopLeftCameraBound->SetupAttachment(CameraRootComponent);
	BottomRightCameraBound = CreateDefaultSubobject<UGP_LevelBoundComponent>(TEXT("BottomRight LevelBound"));
	BottomRightCameraBound->SetupAttachment(CameraRootComponent);


}

// Called when the game starts or when spawned
void AGP_LevelInstanceData::BeginPlay(){
	Super::BeginPlay();
	SetCameraBounds();

	AGP4GameModeBase* GameMode = Cast<AGP4GameModeBase>(GetWorld()->GetAuthGameMode());
	if ( GameMode != nullptr ){
		LevelSpawnerData = GameMode->LevelSpawnerData;
		LevelNumber = LevelSpawnerData->GetLevelInstanceDataLevelNumber(this);
		LevelHighScore = GameMode->SaveGameManager->GetLevelHighScore(LevelNumber);
	}

	AGP_PlayerController* PlayerController = Cast<AGP_PlayerController>(GetWorld()->GetFirstPlayerController());
	PlayerController->ReceiveLevelInstanceData(this);

#if WITH_EDITOR
	GetWorld()->PersistentLineBatcher->Flush();
#endif
}

// Called every frame
void AGP_LevelInstanceData::Tick( float DeltaTime ){
	Super::Tick(DeltaTime);
}

F_LevelStreamBounds AGP_LevelInstanceData::GetAdaptedCameraBounds() const{

	const FVector Location = GetActorLocation();
	F_LevelStreamBounds NewCameraBounds;
	NewCameraBounds.TopLeft.Y = Location.Y + LevelCameraBounds.TopLeft.Y;
	NewCameraBounds.TopLeft.Z = Location.Y + LevelCameraBounds.TopLeft.Z;
	NewCameraBounds.BottomRight.Y = Location.Z + LevelCameraBounds.BottomRight.Y;
	NewCameraBounds.BottomRight.Z = Location.Z + LevelCameraBounds.BottomRight.Z;
	return NewCameraBounds;
}

F_LevelStreamBounds AGP_LevelInstanceData::GetAdaptedLevelBounds() const{
	F_LevelStreamBounds NewLevelBounds;
	NewLevelBounds.TopLeft.Y = LevelBounds.TopLeft.Y;
	NewLevelBounds.TopLeft.Z = RootComponentLocation.Z - LevelRootComponentLocation.Z;
	NewLevelBounds.BottomRight.Y = LevelBounds.BottomRight.Y;
	NewLevelBounds.BottomRight.Z = RootComponentLocation.Z - LevelRootComponentLocation.Z;
	return NewLevelBounds;
}

void AGP_LevelInstanceData::Reset(){
	MinionsInGoal = 0;
}



void AGP_LevelInstanceData::SetCameraBounds(){
	TopLeftCameraBoundLocation = TopLeftCameraBound->GetComponentLocation();
	BottomRightCameraBoundLocation = BottomRightCameraBound->GetComponentLocation();

	LevelCameraBounds.TopLeft.Y = TopLeftCameraBoundLocation.Y;
	LevelCameraBounds.TopLeft.Z = TopLeftCameraBoundLocation.Z;
	LevelCameraBounds.BottomRight.Y = BottomRightCameraBoundLocation.Y;
	LevelCameraBounds.BottomRight.Z = BottomRightCameraBoundLocation.Z;

	const FVector TopLeft = FVector{
		0.0f, TopLeftCameraBoundLocation.Y, TopLeftCameraBoundLocation.Z
	};
	const FVector BottomLeft = FVector{
		0.0f, TopLeftCameraBoundLocation.Y, BottomRightCameraBoundLocation.Z
	};
	const FVector BottomRight = FVector{
		0.0f, BottomRightCameraBoundLocation.Y, BottomRightCameraBoundLocation.Z
	};
	const FVector TopRight = FVector{
		0.0f, BottomRightCameraBoundLocation.Y, TopLeftCameraBoundLocation.Z
	};
#if WITH_EDITOR
	GetWorld()->PersistentLineBatcher->DrawLine(TopLeft, BottomLeft, FLinearColor::Blue, 1.0f, 5.0f);
	GetWorld()->PersistentLineBatcher->DrawLine(BottomLeft, BottomRight, FLinearColor::Blue, 1.0f, 5.0f);
	GetWorld()->PersistentLineBatcher->DrawLine(BottomRight, TopRight, FLinearColor::Blue, 1.0f, 5.0f);
	GetWorld()->PersistentLineBatcher->DrawLine(TopRight, TopLeft, FLinearColor::Blue, 1.0f, 5.0f);
#endif
	if ( UpdateBounds == false ){
		UpdateBounds = true;
	}
}

void AGP_LevelInstanceData::SetLevelBounds(){
	TopLeftLevelBoundLocation = TopLeftLevelBound->GetComponentLocation();
	BottomRightLevelBoundLocation = BottomRightLevelBound->GetComponentLocation();

	LevelRootComponentLocation = LevelRootComponent->GetComponentLocation();
	LevelRootComponentLocation.Y = TopLeftLevelBound->GetComponentLocation().Y;

	LevelBounds.TopLeft.Y = TopLeftLevelBoundLocation.Y;
	LevelBounds.TopLeft.Z = TopLeftLevelBoundLocation.Z;
	LevelBounds.BottomRight.Y = BottomRightLevelBoundLocation.Y;
	LevelBounds.BottomRight.Z = BottomRightLevelBoundLocation.Z;

	const FVector TopLeft = FVector{
		0.0f, TopLeftLevelBoundLocation.Y, TopLeftLevelBoundLocation.Z
	};
	const FVector BottomLeft = FVector{
		0.0f, TopLeftLevelBoundLocation.Y, BottomRightLevelBoundLocation.Z
	};
	const FVector BottomRight = FVector{
		0.0f, BottomRightLevelBoundLocation.Y, BottomRightLevelBoundLocation.Z
	};
	const FVector TopRight = FVector{
		0.0f, BottomRightLevelBoundLocation.Y, TopLeftLevelBoundLocation.Z
	};

#if WITH_EDITOR
	GetWorld()->PersistentLineBatcher->DrawLine(TopLeft, BottomLeft, FLinearColor::Yellow, 1.0f, 8.0f);
	GetWorld()->PersistentLineBatcher->DrawLine(BottomLeft, BottomRight, FLinearColor::Yellow, 1.0f, 8.0f);
	GetWorld()->PersistentLineBatcher->DrawLine(BottomRight, TopRight, FLinearColor::Yellow, 1.0f, 8.0f);
	GetWorld()->PersistentLineBatcher->DrawLine(TopRight, TopLeft, FLinearColor::Yellow, 1.0f, 8.0f);
#endif

	if ( UpdateBounds == false ){
		UpdateBounds = true;
	}
}

void AGP_LevelInstanceData::SetBounds(){
	UpdateBounds = true;
	RootComponentLocation = GetActorLocation();
	GetWorld()->PersistentLineBatcher->Flush();
	if ( ShowLevelBounds == true ){
		SetCameraBounds();
		SetLevelBounds();
	}
	UpdateBounds = false;
}
// ---- Begin Editor only ---- // 
#if WITH_EDITOR
void AGP_LevelInstanceData::OnConstruction( const FTransform& Transform ){
	Super::OnConstruction(Transform);
	UpdateBounds = true;
	GetWorld()->PersistentLineBatcher->Flush();
	SetCameraBounds();
	SetLevelBounds();
	UpdateBounds = false;
}
#endif

// ---- End Editor only ---- //

FTransform AGP_LevelInstanceData::GetPlayerStartTransform(){
	if ( PlayerStartPosition == nullptr ){

		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);
		float CurrentDistance = 999999999.0f;

		for ( int i = 0; i < PlayerStarts.Num(); i++ ){
			if ( PlayerStartPosition == nullptr ){
				PlayerStartPosition = PlayerStarts[i];
				CurrentDistance = FVector::Distance(PlayerStartPosition->GetActorLocation(), GetActorLocation());
				continue;
			}
			const float PlayerStartDistance = FVector::Distance(PlayerStarts[i]->GetActorLocation(), GetActorLocation());
			if ( PlayerStartDistance < CurrentDistance ){
				CurrentDistance = PlayerStartDistance;
				PlayerStartPosition = PlayerStarts[i];
			}
		}
	}
	return PlayerStartPosition->GetActorTransform();
}

void AGP_LevelInstanceData::MinionToGoal(){
	MinionsInGoal++;
	UGP_SaveGameManager::Get()->SetLevelHighScore(LevelNumber, StarLevelCompleted());
	LevelHighScore = UGP_SaveGameManager::Get()->GetLevelHighScore(LevelNumber);
	OnMinionEnteredGoal.Broadcast();
}

int AGP_LevelInstanceData::StarLevelCompleted(){
	if ( MinionsInGoal >= ThreeStar_MinionsToSave ){
		return 3;
	}

	if ( MinionsInGoal >= TwoStar_MinionsToSave ){
		return 2;
	}

	if ( MinionsInGoal >= OneStar_MinionsToSave ){
		return 1;
	}

	return 0;
}
int AGP_LevelInstanceData::GetBestStarLevelCompleted( int HighScore ){
	if ( HighScore >= ThreeStar_MinionsToSave ){
		return 3;
	}

	if ( HighScore >= TwoStar_MinionsToSave ){
		return 2;
	}

	if ( HighScore >= OneStar_MinionsToSave ){
		return 1;
	}

	return 0;
}
