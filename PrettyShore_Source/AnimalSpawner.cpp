// I like sand

#include "AnimalSpawner.h"

// Sets default values
AAnimalSpawner::AAnimalSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootObject = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	//RootComponent(RootObject);
	
	//Seagull Stuff
	SeagullPathsParent = CreateDefaultSubobject<USceneComponent>(TEXT("Seagull Paths"));
	SeagullPathsParent->SetupAttachment(RootObject);

	//Crab Stuff
	CrabPathsParent = CreateDefaultSubobject<USceneComponent>(TEXT("Crab Paths"));
	CrabPathsParent->SetupAttachment(RootObject);

	FishPathParent = CreateDefaultSubobject<USceneComponent>(TEXT("Fish Paths"));
	FishPathParent->SetupAttachment(RootObject);
}

void AAnimalSpawner::GetAllSplines(USceneComponent* SceneComponent, TArray<USplineComponent*>& ReturnValue)
{
	for(int i = 0; i < SceneComponent->GetNumChildrenComponents(); i++)
	{
		ReturnValue.Add(Cast<USplineComponent>(SceneComponent->GetChildComponent(i)));
	}
}

