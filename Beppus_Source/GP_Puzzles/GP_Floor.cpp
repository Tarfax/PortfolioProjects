// Fill out your copyright notice in the Description page of Project Settings.


#include "GP_Floor.h"


// Sets default values
AGP_Floor::AGP_Floor(){
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGP_Floor::BeginPlay(){
	Super::BeginPlay();

}

// Called every frame
void AGP_Floor::Tick( float DeltaTime ){
	Super::Tick(DeltaTime);

}
