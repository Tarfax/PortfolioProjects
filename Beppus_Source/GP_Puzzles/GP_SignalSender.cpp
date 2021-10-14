// Fill out your copyright notice in the Description page of Project Settings.


#include "GP_SignalSender.h"
#include "GP_SignalReceiver.h"
#include "GP4/GP4.h"


// Sets default values
AGP_SignalSender::AGP_SignalSender(){
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AGP_SignalSender::BeginPlay(){
	Super::BeginPlay();

}

// Called every frame
void AGP_SignalSender::Tick( float DeltaTime ){
	Super::Tick(DeltaTime);

}

void AGP_SignalSender::Interact(TArray<AActor*> Actors){
	if ( IsBroken == false ){
		for ( int i = 0; i < SignalReceivers.Num(); i++ ){
			if (SignalReceivers[i] == nullptr) {
				continue;
			}
			SignalReceivers[i]->Interact(Actors);
		}
		OnInteract();
	}

}

void AGP_SignalSender::Break(){
	if ( IsActive == true ){
		for ( int i = 0; i < SignalReceivers.Num(); i++ ) {
			const TArray<AActor*> Actors;
			SignalReceivers[i]->Interact(Actors);
		}
	}
	IsBroken = true;
	OnBreak();
}
