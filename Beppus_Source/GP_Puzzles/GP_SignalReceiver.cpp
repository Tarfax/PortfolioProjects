// Fill out your copyright notice in the Description page of Project Settings.


#include "GP_SignalReceiver.h"


// Sets default values
AGP_SignalReceiver::AGP_SignalReceiver() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGP_SignalReceiver::BeginPlay() {
	Super::BeginPlay();
	
}

// Called every frame
void AGP_SignalReceiver::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

void AGP_SignalReceiver::Interact(TArray<AActor*> InteractingActors) {
	FListOfActors ListOfActors;
	ListOfActors.Actors = InteractingActors;
	OnInteract(ListOfActors);
}
