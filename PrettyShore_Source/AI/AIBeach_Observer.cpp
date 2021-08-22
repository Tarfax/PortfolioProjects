// I like sand


#include "AIBeach_Observer.h"

#include "AI_BeachCharacter.h"


// Sets default values
UAIBeach_Observer::UAIBeach_Observer() {
	// Visitors.Reserve(50);
	// LifeGuards.Reserve(10);
	//
	// Drowners.Reserve(10);
	// Savers.Reserve(5);
}

float UAIBeach_Observer::GetVisitorHappiness() {
	float sum = 0;
	for (int i = 0; i < Visitors.Num(); i++) {
		sum += Visitors[i]->Mood.Happiness;
	}

	float deadHappiness = DeadVisitors.Num() * DeadHappinessSubtraction;

	if (Visitors.Num() > 0) {
		return (sum / Visitors.Num()) - deadHappiness;
	}
	return 0;
}

float UAIBeach_Observer::GetVisitorNeedBodyTemperature() {
	float sum = 0;

	for (int i = 0; i < Visitors.Num(); i++) {
		sum += Visitors[i]->Need.BodyTemperature;
	}
	if (Visitors.Num() > 0) {
		return sum / Visitors.Num();
	}
	return 0;
}


float UAIBeach_Observer::GetVisitorNeedThirst() {
	float sum = 0;

	for (int i = 0; i < Visitors.Num(); i++) {
		sum += Visitors[i]->Need.Thirst;
	}
	if (Visitors.Num() > 0) {
		return sum / Visitors.Num();
	}
	return 0;
}


float UAIBeach_Observer::GetVisitorNeedHunger() {
	float sum = 0;

	for (int i = 0; i < Visitors.Num(); i++) {
		sum += Visitors[i]->Need.Hunger;
	}
	if (Visitors.Num() > 0) {
		return sum / Visitors.Num();
	}
	return 0;
}


float UAIBeach_Observer::GetVisitorNeedToilet() {
	float sum = 0;

	for (int i = 0; i < Visitors.Num(); i++) {
		sum += Visitors[i]->Need.Toilet;
	}
	if (Visitors.Num() > 0) {
		return sum / Visitors.Num();
	}
	return 0;
}


float UAIBeach_Observer::GetLifeguardNeedRest() {
	float sum = 0;

	for (int i = 0; i < Lifeguards.Num(); i++) {
		sum += Lifeguards[i]->Need.Rest;
	}
	if (Lifeguards.Num() > 0) {
		return sum / Lifeguards.Num();
	}
	return 0;
}

float UAIBeach_Observer::GetLifeguardNeedRecreation() {
	float sum = 0;

	for (int i = 0; i < Lifeguards.Num(); i++) {
		sum += Lifeguards[i]->Need.Recreation;
	}
	if (Lifeguards.Num() > 0) {
		return sum / Lifeguards.Num();
	}
	return 0;
}

float UAIBeach_Observer::GetLifeguardNeedToilet() {
	float sum = 0;

	for (int i = 0; i < Lifeguards.Num(); i++) {
		sum += Lifeguards[i]->Need.Toilet;
	}
	if (Lifeguards.Num() > 0) {
		return sum / Lifeguards.Num();
	}
	return 0;
}


float UAIBeach_Observer::GetLifeguardMoodMorale() {
	float sum = 0;

	for (int i = 0; i < Lifeguards.Num(); i++) {
		sum += Lifeguards[i]->Mood.Morale;
	}
	if (Lifeguards.Num() > 0) {
		return sum / Lifeguards.Num();
	}
	return 0;
}

int UAIBeach_Observer::GetVisitorCount() {
	return VisitorCount;
}

void UAIBeach_Observer::NewVisitor(AAIVisitor_Controller* Visitor) {
	if (Visitor != nullptr) {
		// UE_LOG(LogTemp, Log, TEXT("Added a Visotor!:D "));
		Visitors.Add(Visitor);
		VisitorCount = Visitors.Num();
		VisitorTotalCount++;
	}
}

void UAIBeach_Observer::NewLifeGuard(AAILifeGuard_Controller* LifeGuard) {
	if (LifeGuard != nullptr) {
		Lifeguards.Add(LifeGuard);
		LifeGuardCount = Lifeguards.Num();
		LifeGuardTotalCount++;

		OnLifeguardHired.Broadcast();
	}
}

void UAIBeach_Observer::RemoveVisitor(AAIVisitor_Controller* Visitor) {
	if (Visitor != nullptr) {
		Visitors.Remove(Visitor);
		VisitorCount = Visitors.Num();
	}
}

void UAIBeach_Observer::RemoveLifeGuard(AAILifeGuard_Controller* LifeGuard) {
	if (LifeGuard != nullptr) {
		if (Lifeguards.Contains(LifeGuard) == true) {
			Lifeguards.Remove(LifeGuard);
			LifeGuardCount = Lifeguards.Num();
		}
	}
}

void UAIBeach_Observer::AddDrowner(AAIVisitor_Controller* Drowner) {
	Drowners.Add(Drowner);
	if (Lifeguards.Num() > 0) {
		for (int i = 0; i < Lifeguards.Num(); i++) {
			Lifeguards[i]->SomeoneIsDrowning(true);
		}
	}
}

void UAIBeach_Observer::RemoveDrowner(AAIVisitor_Controller* Drowner) {
	Drowners.Remove(Drowner);
	if (Drowners.Num() == 0) {
		for (int i = 0; i < Lifeguards.Num(); i++) {
			Lifeguards[i]->SomeoneIsDrowning(false);
		}
	}
}

void UAIBeach_Observer::AddDeadVisitor(AAIVisitor_Controller* Visitor) {
	DeadVisitors.Add(Visitor);
	DeadVisitorCount++;
}

void UAIBeach_Observer::RemoveDeadVisitor(AAIVisitor_Controller* Visitor) {
	DeadVisitors.Remove(Visitor);
}

void UAIBeach_Observer::NotifyOnVisitorDeath() {
	for (int i = 0; i < Visitors.Num(); i++) {
		Visitors[i]->SetModifier(E_AIVisitor_ModifierType::DeathOnBeach);
	}
}


void UAIBeach_Observer::StartSaveDrowner(AAIVisitor_Controller* Drowner, AAILifeGuard_Controller* LifeGuard) {
	Savers.Add(Drowner, LifeGuard);
}

void UAIBeach_Observer::FinishSaveDrowner(AAIVisitor_Controller* Drowner) {
	Savers.Remove(Drowner);
	Drowners.Remove(Drowner);
}


void UAIBeach_Observer::CancelSaveDrowner(AAIVisitor_Controller* Drowner) {
	Savers.Remove(Drowner);
}


TArray<AAIVisitor_Controller*> UAIBeach_Observer::GetDrowners() {
	TArray<AAIVisitor_Controller*> drowners;
	TArray<int> RemoveTheseNullPtr;
	for (int i = 0; i < Drowners.Num(); i++) {
		if (Drowners[i] == nullptr) {
			RemoveTheseNullPtr.Add(i);
			continue;
		}
		if (Savers.Contains(Drowners[i]) == false) {
			drowners.Add(Drowners[i]);
		}
	}
	for (int i = 0; i < RemoveTheseNullPtr.Num(); i++) {
		Drowners.RemoveAt(RemoveTheseNullPtr[i]);
	}
	return drowners;
}

AAILifeGuard_Controller* UAIBeach_Observer::GetClosestAvailableLifeguard(FVector Location) {
	float ClosestDistance = 999999999999;
	AAILifeGuard_Controller* Rescuer = nullptr;

	for (int i = 0; i < Lifeguards.Num(); i++) {
		if (FVector::DistSquared(Lifeguards[i]->AI_BeachCharacter->GetActorLocation(), Location) < ClosestDistance) {
			if (Lifeguards[i]->PerformingTask != E_AILifeGuard_TaskType::Saving && Lifeguards[i]->PerformingTask !=
				E_AILifeGuard_TaskType::HasSpawned && Lifeguards[i]->PerformingTask !=
				E_AILifeGuard_TaskType::HeldByPlayer)
				ClosestDistance = FVector::DistSquared(Lifeguards[i]->AI_BeachCharacter->GetActorLocation(), Location);
			Rescuer = Lifeguards[i];
		}
	}
	return Rescuer;
}
