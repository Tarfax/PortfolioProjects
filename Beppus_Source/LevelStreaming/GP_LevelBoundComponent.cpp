// Fill out your copyright notice in the Description page of Project Settings.


#include "GP_LevelBoundComponent.h"
#include "GP_LevelInstanceData.h"


// Sets default values for this component's properties
UGP_LevelBoundComponent::UGP_LevelBoundComponent() {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}
#if WITH_EDITOR
void UGP_LevelBoundComponent::PostEditComponentMove(bool bFinished) {
	Super::PostEditComponentMove(bFinished);
	GetOwner<AGP_LevelInstanceData>()->SetBounds();
}
#endif


// Called when the game starts
void UGP_LevelBoundComponent::BeginPlay() {
	Super::BeginPlay();

	// ...

}


// Called every frame
void UGP_LevelBoundComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
