#include "AIBeach_Controller_Base.h"
#include "Team9Assemble/AI/Models/AI_Trash.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Team9Assemble/Buildings/BuildingBase.h"
#include "AI_BeachCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

AAIBeach_Controller_Base::AAIBeach_Controller_Base() {
	PrimaryActorTick.bCanEverTick = true;
}

void AAIBeach_Controller_Base::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

	DecorationValueTimer -= DeltaSeconds;
	if (DecorationValueTimer <= 0.0f) {
		SetDecorationValue();
		DecorationValueTimer = DecorationValueTime;
	}
}

bool AAIBeach_Controller_Base::ReservBuilding(ABuildingBase* Building) {
	if (Building->AnnounceIncoming(this) == true) {
		ReservedBuilding = Building;
		IsHeadingForBuilding = true;
		return true;
	}
	return false;
}

bool AAIBeach_Controller_Base::StartInteractWithBuilding() {
	IsHeadingForBuilding = false;
	if (ReservedBuilding != nullptr) {
		if (ReservedBuilding->Enter(this) == false) {
			ReservedBuilding = nullptr;
			IsInteractingWithBuilding = false;
			
			return false;
		}
		
		InteractingBuilding = ReservedBuilding;
		ReservedBuilding = nullptr;
		IsInteractingWithBuilding = true;
		return true;
	}
	return false;
}

void AAIBeach_Controller_Base::StopInteractWithBuilding() {
	if (IsHeadingForBuilding == true) {
		ReservedBuilding->CancelIncoming(this);
		ReservedBuilding = nullptr;
		IsHeadingForBuilding = false;
	}
	
	if (InteractingBuilding != nullptr) {
		InteractingBuilding->Leave(this);
		Blackboard->SetValueAsObject("Building", nullptr);
		InteractingBuilding = nullptr;
		IsInteractingWithBuilding = false;
	}
}

void AAIBeach_Controller_Base::OnMovingStateChanged(bool NewState) {
	Blackboard->SetValueAsBool("IsInMovingState", NewState);
	OnMovingStateChangedEvent(NewState);
}

void AAIBeach_Controller_Base::OnIsInWaterChanged(bool NewState) {
	Blackboard->SetValueAsBool("IsInWater", NewState);
	OnIsInWaterChangedEvent(NewState);
}


void AAIBeach_Controller_Base::SetDecorationValue() {
	TArray<AActor*> OverlappedActors;
	UKismetSystemLibrary::SphereOverlapActors(this, AI_BeachCharacter->GetActorLocation(), DecorationSearchRadius,
											OverlapObjectTypes, AActor::StaticClass(), TArray<AActor*>(),
											OverlappedActors);

	int DecoValue = 0;
	for (auto* Actor : OverlappedActors) {
		AAI_Trash* Trash = Cast<AAI_Trash>(Actor);
		ABuildingBase* DecoBuilding= nullptr;
		if (Trash == nullptr) {
			DecoBuilding = Cast<ABuildingBase>(Actor);
		}
			
		if (Trash != nullptr) {
			DecoValue += Trash->DecorationValue;
		} else if (DecoBuilding != nullptr) {
			DecoValue += DecoBuilding->BuildingData->DecorationValue;
		}
	}

	DecorationValue = DecoValue;
}
