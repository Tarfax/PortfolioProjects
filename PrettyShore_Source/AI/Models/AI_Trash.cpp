// I like sand


#include "AI_Trash.h"
#include "Components/BoxComponent.h"
#include "Team9Assemble/World/BeachGameState.h"


// Sets default values
AAI_Trash::AAI_Trash() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = SceneComponent;

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	BoxCollider->SetCollisionProfileName(TEXT("BuildingPlaced"));
	BoxCollider->SetupAttachment(SceneComponent);

	Trash = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Building"));
	Trash->SetCollisionProfileName(TEXT("NoCollision"));
	Trash->SetupAttachment(SceneComponent);
}

// Called when the game starts or when spawned
void AAI_Trash::BeginPlay() {
	Super::BeginPlay();
	
	BeachWork = NewObject<UBeachWork>(this);
	BeachWork->Timer = TimeToClean;
	BeachWork->WorkLocation = GetTransform();
	BeachWork->BeachWorkObject = this;
	BeachWork->WorkType = EBeachWorkType::PickupGarbage;
	
	ABeachGameState* GameInstance = Cast<ABeachGameState>(GetWorld()->GetGameState());
	GameInstance->TrashObserver->AddTrash(this);
}

// Called every frame
void AAI_Trash::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

void AAI_Trash::FinishWork(UBeachWork* Work) {
	ABeachGameState* GameInstance = Cast<ABeachGameState>(GetWorld()->GetGameState());
	GameInstance->TrashObserver->RemoveTrash(this);
	Destroy();
	BeachWork->ConditionalBeginDestroy();
}

