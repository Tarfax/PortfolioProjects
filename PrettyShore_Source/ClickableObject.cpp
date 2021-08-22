// I like sand


#include "ClickableObject.h"

// Sets default values for this component's properties
UClickableObject::UClickableObject()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}

void UClickableObject::CallEvent(AActor* ActorReference, U_Building_DataAsset* DataAsset)
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, TEXT("CALLING AN EVENT " + GetOwner()->GetActorLabel()));
	TriggerInteractEvent.Broadcast(ActorReference, DataAsset);
}

///Connect this with the actor and event tick
void UClickableObject::UpdateUIElementPosition()
{
	ClearUI.Broadcast(nullptr, nullptr);
}
