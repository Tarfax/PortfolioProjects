// Fill out your copyright notice in the Description page of Project Settings.

#include "GP_DynamicObjectHandlerComponent.h"
#include "GP_RestartComp.h"
#include "Containers/Map.h"
#include "GP4/GP4.h"
#include "GP4/GP4GameModeBase.h"
#include "GP4/GP_Puzzles/GP_DynamicObject.h"

UGP_DynamicObjectHandlerComponent::UGP_DynamicObjectHandlerComponent(){
	PrimaryComponentTick.bCanEverTick = false;
	bAutoActivate = true;
}

void UGP_DynamicObjectHandlerComponent::BeginPlay(){
	Super::BeginPlay();
	PriorityMap.Reserve(3);
	ensure(ResetLevelShakeClass!=nullptr);
}

void UGP_DynamicObjectHandlerComponent::ReceiveAnnounce( UGP_RestartComp* Object ){
	ensure(Object->IsValidLowLevelFast());
	if ( !PriorityMap.Contains(Object->Priority) ){
		PriorityMap.Emplace(Object->Priority, FGP_PriorityGroup{Object->Priority, TArray<UGP_RestartComp*>()});
	}
	PriorityMap[Object->Priority].Objects.Add(Object);
}

void UGP_DynamicObjectHandlerComponent::ReceiveUnAnnounce( UGP_RestartComp* Object ){
	ensure(Object->IsValidLowLevelFast());
	ensure(!Object->IsPendingKillOrUnreachable());

	if (Object->IsPendingKillOrUnreachable() == true) {
		UE_LOG(LogTemp, Log, TEXT("One object is pending kill or unreachable but can we reach it? %s"), *Object->GetOwner()->GetName());
	}
	
	if ( PriorityMap.Contains(Object->Priority) ){
		if ( PriorityMap[Object->Priority].Objects.Contains(Object) ){
			PriorityMap[Object->Priority].Objects.RemoveSingle(Object);
		}
		else{
			UE_LOG(LogTemp, Log, TEXT("%s is not in PriorityMap"), *Object->GetName())
		}
	}
	//
	if ( RequestedObjects.Contains(Object) ){
		//If the object is in the loop of being loaded/unloaded, we must remove it and then check if we are done with the load/unload again.
		RequestedObjects.RemoveSingle(Object);
		CheckIfCallbackComplete();
	}
}

void UGP_DynamicObjectHandlerComponent::RequestLoad(){
	ensure(PriorityMap.Num()>0);
	bIsLoading = true;
	CurrentKey = PriorityMap[0].priority;
	RequestedObjects.Empty();
	LoadPriorityGroup(CurrentKey);
}

void UGP_DynamicObjectHandlerComponent::LoadPriorityGroup( int PriorityKey ){
	CurrentKey = PriorityKey;
	for ( auto& Object : PriorityMap[CurrentKey].Objects ){
		RequestedObjects.Add(Object);
	}

	for ( auto& Object : PriorityMap[CurrentKey].Objects ){
		Object->HandleLoadRequest();
	}
}

void UGP_DynamicObjectHandlerComponent::LoadCompleteCallback( UGP_RestartComp* DynamicObject ){
	if ( RequestedObjects.Contains(DynamicObject) ){
		RequestedObjects.RemoveSingle(DynamicObject);

	}
	CheckIfCallbackComplete();
}

void UGP_DynamicObjectHandlerComponent::RequestUnload( bool bWithResetAfterUnload = false ){
	bWithReset = bWithResetAfterUnload;
	ensure(PriorityMap.Num()>0);
	CurrentKey = PriorityMap[0].priority;
	RequestedObjects.Empty();
	if ( PriorityMap.Num() > 0 ){
		//GetOwner<AGP4GameModeBase>()->OnLevelStartedUnloadingDelegate.Broadcast();

		UnloadPriorityGroup(CurrentKey);
	}
}

void UGP_DynamicObjectHandlerComponent::UnloadPriorityGroup( int PriorityKey ){
	CurrentKey = PriorityKey;
	for ( auto& Object : PriorityMap[CurrentKey].Objects ){
		RequestedObjects.Emplace(Object);
	}
	for ( int i = 0; i < RequestedObjects.Num(); i++ ){
		GetOwner<AGP4GameModeBase>()->CompsWaitingForSignal.Add(RequestedObjects[i]->GetOwner());
	}
	for ( auto& Object : PriorityMap[CurrentKey].Objects ){
		Object->HandleUnloadRequest();
	}
}

void UGP_DynamicObjectHandlerComponent::UnloadCompleteCallback( UGP_RestartComp* DynamicObject ){
	if ( RequestedObjects.Contains(DynamicObject) ){
		RequestedObjects.RemoveSingle(DynamicObject);
	}
	CheckIfCallbackComplete();
}

void UGP_DynamicObjectHandlerComponent::CheckIfCallbackComplete(){
	if ( RequestedObjects.Num() <= 0 ){
		do{
			CurrentKey++;
			if ( PriorityMap.Contains(CurrentKey) ){
				if ( bIsLoading == true ){
					LoadPriorityGroup(CurrentKey);
				}
				else{
					UnloadPriorityGroup(CurrentKey);
				}
				return;
			}
		}
		while ( CurrentKey <= PriorityMap.Num() );
		if ( CurrentKey > PriorityMap.Num() ){
			if ( bIsLoading == true ){
				HandleAllObjectsLoaded();
			}
			else{
				HandleAllObjectsUnloaded();
			}
		}
	}
}

void UGP_DynamicObjectHandlerComponent::HandleAllObjectsUnloaded(){
	OnAllObjectsUnloaded.Broadcast();
	if ( bWithReset ){
		RequestLoad();
	}
}

void UGP_DynamicObjectHandlerComponent::ResetDynamicObjects(){
	RequestUnload(true);
}

void UGP_DynamicObjectHandlerComponent::HandleAllObjectsLoaded(){
	bIsLoading = false;
	OnAllObjectsLoaded.Broadcast();
	if ( bWithReset ){
		HandleAllObjectsReset();
	}
}

void UGP_DynamicObjectHandlerComponent::HandleAllObjectsReset(){
	OnAllObjectsReset.Broadcast();
	bWithReset = false;
}
