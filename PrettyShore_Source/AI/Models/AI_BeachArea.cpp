// I like sand


#include "AI_BeachArea.h"
#include "Components/BoxComponent.h"

// Sets default values
AAI_BeachArea::AAI_BeachArea() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Area = CreateDefaultSubobject<UBoxComponent>(TEXT("Area"));
	RootComponent = Area;
	Area->SetBoxExtent(FVector(SizeX, SizeY, 100));
}

// Called when the game starts or when spawned
void AAI_BeachArea::BeginPlay() {
	Super::BeginPlay();
	RootLocation = GetActorLocation();
}

// Called every frame
void AAI_BeachArea::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

FVector AAI_BeachArea::FindRandomLocation() {
	// FVector Location = GetActorLocation();

	float XMinRange = RootLocation.X - (SizeX);
	float XMaxRange = RootLocation.X + (SizeX);

	float YMinRange = RootLocation.Y - (SizeY);
	float YMaxRange = RootLocation.Y + (SizeY);

	float RandomX = FMath::RandRange(XMinRange, XMaxRange);
	float RandomY = FMath::RandRange(YMinRange, YMaxRange);

	float RandomXMod = (int)RandomX % 50;
	float RandomYMod = (int)RandomY % 50;

	FVector RandomPosition = FVector(RandomX - RandomXMod, RandomY - RandomYMod, RootLocation.Z);
	
	// UE_LOG(LogTemp, Log, TEXT("Returning? %f, %f"), RandomPosition.X,RandomPosition.Y);

	return RandomPosition;
}

FVector AAI_BeachArea::ReservLocation(AAIVisitor_Controller* Visitor) {
	return FVector::ZeroVector;
}

bool AAI_BeachArea::AddBeachVisitor(AAIVisitor_Controller* Visitor, FVector Position, FVector& PositionForHome) {
	float XMod = (int)Position.X % 100;
	float YMod = (int)Position.Y % 100;
	FVector ModPosition = FVector(Position.X - XMod, Position.Y - YMod, 2);
	
	if (OccupiedPositions.Contains(ModPosition) == false) {
		OccupiedPositions.Add(ModPosition, Visitor);
		AIHomePositions.Add(Visitor, ModPosition);
		PositionForHome = ModPosition;
		return true;
	}
	
	return false;
}

void AAI_BeachArea::RemoveBeachVisitor(AAIVisitor_Controller* Visitor) {
	if (AIHomePositions.Contains(Visitor) == true) {
		FVector position;
		AIHomePositions.RemoveAndCopyValue(Visitor, position);
		OccupiedPositions.Remove(position);
	}
}

#if WITH_EDITORONLY_DATA
void AAI_BeachArea::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
	Super::PostEditChangeProperty(PropertyChangedEvent);
	SizeX = SizeX - SizeX % 100;
	SizeY = SizeY - SizeY % 100;
	Area->SetBoxExtent(FVector(SizeX, SizeY, 100));

	SetActorScale3D(FVector::OneVector);
}
#endif