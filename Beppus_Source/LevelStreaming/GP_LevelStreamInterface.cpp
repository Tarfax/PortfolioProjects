// Fill out your copyright notice in the Description page of Project Settings.


#include "GP_LevelStreamInterface.h"



#include "Kismet/GameplayStatics.h"


// Add default functionality here for any IGP_LevelStreamInterface functions that are not pure virtual.
void IGP_LevelStreamInterface::RestartLevel_Implementation(  ){

}
void IGP_LevelStreamInterface::GoToPreviousLevel_Implementation(  ){
}
void IGP_LevelStreamInterface::GoToNextLevel_Implementation( ){
}
void IGP_LevelStreamInterface::LoadLevel_Implementation(FName LevelToLoad ){
	
//TODO Sub to corresponding world events in the matching function calls so we keep track of whats being loaded and where.
	//FWorldDelegates::OnWorldBeginTearDown.AddUFunction(this, FName("OnTearDownBegin"));
}
void IGP_LevelStreamInterface::UnLoadLevel_Implementation(FName LevelToUnload){
}
