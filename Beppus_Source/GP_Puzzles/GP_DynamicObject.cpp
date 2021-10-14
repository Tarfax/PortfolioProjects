// Fill out your copyright notice in the Description page of Project Settings.


#include "GP_DynamicObject.h"


#include "GP4/LevelStreaming/GP_RestartComp.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AGP_DynamicObject::AGP_DynamicObject(){
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RestartComp = CreateDefaultSubobject<UGP_RestartComp>(TEXT("RestartComp"));

	if (CenterX == true) {
		FVector Location = GetActorLocation();
		Location.X = 0.0f;
		SetActorLocation(Location);
	}
}

// Called when the game starts or when spawned
void AGP_DynamicObject::BeginPlay(){
	TravelOffset = FMath::RandRange(TravelOffsetZMin, TravelOffsetZMax);
	AnimationTime = UKismetMathLibrary::RandomFloatInRange(RandomMin, RandomMax);
	Super::BeginPlay();
}

// Called every frame
void AGP_DynamicObject::Tick( float DeltaTime ){
	Super::Tick(DeltaTime);

}
