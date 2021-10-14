// Fill out your copyright notice in the Description page of Project Settings.


#include "GP_SignalReceiver_CameraOffset.h"

#include "GP4/GP_Character/GP_CameraPawn.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AGP_SignalReceiver_CameraOffset::AGP_SignalReceiver_CameraOffset() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGP_SignalReceiver_CameraOffset::BeginPlay() {
	Super::BeginPlay();
}

// Called every frame
void AGP_SignalReceiver_CameraOffset::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AGP_SignalReceiver_CameraOffset::SetCameraOffset(FVector Location, float Distance, bool UseRotateToThisActor) {
	AGP_CameraPawn* CameraPawn = Cast<AGP_CameraPawn>(UGameplayStatics::GetActorOfClass(this, AGP_CameraPawn::StaticClass()));
	CameraPawn->SetCameraDistanceWhenPossessingOffset(Distance);
	CameraPawn->SetCameraLocationWhenPossessingOffset(Location, GetActorLocation(), UseRotateToThisActor);
}

