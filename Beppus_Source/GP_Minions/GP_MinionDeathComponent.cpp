// Fill out your copyright notice in the Description page of Project Settings.

#include "GP_MinionDeathComponent.h"

#include "GP_Minion.h"
#include "GP4/GP_GameStateBase.h"

// Sets default values for this component's properties
UGP_MinionDeathComponent::UGP_MinionDeathComponent() {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}

// Called when the game starts
void UGP_MinionDeathComponent::BeginPlay() { Super::BeginPlay(); }

AGP_MinionDeath* UGP_MinionDeathComponent::BeginDeath(TSubclassOf<AGP_MinionDeath> DeathType) {
	CurrentDeath = GetWorld()->SpawnActor<AGP_MinionDeath>(DeathType, GetOwner()->GetActorTransform());
	//CurrentDeath->SetActorLocation(GetOwner()->GetActorLocation());
	CurrentDeath->MinionToKill = GetOwner<AGP_Minion>();
	CurrentDeath->MinionDeathCompleteDelegate.AddDynamic(this, &UGP_MinionDeathComponent::EndDeath);
	CurrentDeath->BeginMinionDeath(GetOwner<AGP_Minion>());
	return CurrentDeath;
//	GetOwner<AGP_Minion>()->DestroyMinion();
}

void UGP_MinionDeathComponent::EndDeath() {

	GetOwner<AGP_Minion>()->DestroyMinion();
	GetWorld()->GetGameState<AGP_GameStateBase>()->MasterMind->OnMinionDied.Broadcast();
	
}

// Called every frame
void UGP_MinionDeathComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
}
