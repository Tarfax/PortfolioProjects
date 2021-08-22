// I like sand


#include "AI_WaterArea.h"
#include "Components/BoxComponent.h"


// Sets default values
AAI_WaterArea::AAI_WaterArea() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Area = CreateDefaultSubobject<UBoxComponent>(TEXT("Area"));
	RootComponent = Area;
	Area->SetBoxExtent(FVector(SizeX, SizeY, 100));
}

// Called when the game starts or when spawned
void AAI_WaterArea::BeginPlay() {
	Super::BeginPlay();
	
}

// Called every frame
void AAI_WaterArea::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

#if WITH_EDITORONLY_DATA
void AAI_WaterArea::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
	Super::PostEditChangeProperty(PropertyChangedEvent);
	SizeX = SizeX - SizeX % 100;
	SizeY = SizeY - SizeY % 100;
	Area->SetBoxExtent(FVector(SizeX, SizeY, 100));

	SetActorScale3D(FVector::OneVector);
}
#endif

FVector AAI_WaterArea::FindRandomLocation() {
	FVector Location = GetActorLocation();

	float XMinRange = Location.X - (SizeX);
	float XMaxRange = Location.X + (SizeX);

	float YMinRange = Location.Y + (SizeY);
	float YMaxRange = Location.Y - (SizeY);

	float RandomX = FMath::RandRange(XMinRange, XMaxRange);
	float RandomY = FMath::RandRange(YMinRange, YMaxRange);

	float RandomXMod = (int)RandomX % 100;
	float RandomYMod = (int)RandomY % 100;

	FVector RandomPosition = FVector(RandomX - RandomXMod, RandomY - RandomYMod, Location.Z);
	return RandomPosition;
}

