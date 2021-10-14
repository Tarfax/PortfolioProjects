#include "GP_PuzzlePiece_Bomb.h"

#include "GP4/GP4.h"
#include "GP4/GP_Minions/GP_Minion.h"
#include "GP4/LevelStreaming/GP_RestartComp.h"
#include "Kismet/KismetMathLibrary.h"

AGP_PuzzlePiece_Bomb::AGP_PuzzlePiece_Bomb() {
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(SceneComponent);
	RestartComp = CreateDefaultSubobject<UGP_RestartComp>(TEXT("RestartComp"));

}

void AGP_PuzzlePiece_Bomb::BeginPlay() {
	Super::BeginPlay();
	LerpToMinionTimer = LerpToMinionTime;
	BombExplosionTimer = BombExplosionTime;
}

void AGP_PuzzlePiece_Bomb::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
}

void AGP_PuzzlePiece_Bomb::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	if (StartBombCountdown == true) {
		if (StartThrow == true) {

			LerpToMinionTimer -= DeltaTime;
			AlphaTime = LerpToMinionTimer / LerpToMinionTime;

			FVector MinionUpOffset = MinionToExplode->GetActorLocation() + FVector(00, 0, -50);
			FVector Direction = MinionToExplode->GetActorRightVector() * (FVector::OneVector * 50.0f);
			FVector Position = MinionUpOffset + Direction;
			FVector LerpPosition = UKismetMathLibrary::VLerp(GetActorLocation(), Position, DeltaTime * 10.0f);

			SetActorLocation(LerpPosition, false, nullptr, ETeleportType::TeleportPhysics);

			if (AlphaTime < 0) {
				StartThrow = false;
				MinionToExplode->OnRecieveBomb(this);
			}
		}

		BombExplosionTimer -= DeltaTime;
		if (BombExplosionTimer <= 0.0f) {
			OnExplodeBegin();
		}
	}
}

void AGP_PuzzlePiece_Bomb::ThrowToMinion(AGP_Minion* Minion) {
	MinionToExplode = Minion;
	StartThrow = true;
	OnThrowToMinion(Minion);
}
