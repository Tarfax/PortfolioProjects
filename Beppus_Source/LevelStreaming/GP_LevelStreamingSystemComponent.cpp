// Fill out your copyright notice in the Description page of Project Settings.


#include "GP_LevelStreamingSystemComponent.h"


// Sets default values for this component's properties
UGP_LevelStreamingSystemComponent::UGP_LevelStreamingSystemComponent() {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGP_LevelStreamingSystemComponent::BeginPlay() {
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGP_LevelStreamingSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

