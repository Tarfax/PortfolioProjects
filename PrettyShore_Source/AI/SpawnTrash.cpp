// I like sand

#include "SpawnTrash.h"
#include "Team9Assemble/AI/Models/AI_Trash.h"
#include "Team9Assemble/World/BeachGameState.h"

// Sets default values for this component's properties
USpawnTrash::USpawnTrash()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void USpawnTrash::BeginPlay() {
	// TrashObserver = Cast<ABeachGameState> (GetWorld()->GetGameState())->TrashObserver;
	Super::BeginPlay();
}

void USpawnTrash::SpawnTrash(TSubclassOf<AAI_Trash> TrashObject)
{
	FVector AIActorPos = GetOwner()->GetActorLocation();
	FRotator AIActorRot = GetOwner()->GetActorRotation();
	FActorSpawnParameters SpawnParams;
	
	AAI_Trash* TrashInstance = GetWorld()->SpawnActor<AAI_Trash>(TrashObject, AIActorPos + FVector(0, 0, -100), AIActorRot, SpawnParams);
	// VisitorTrash.Add(TrashInstance);
}


