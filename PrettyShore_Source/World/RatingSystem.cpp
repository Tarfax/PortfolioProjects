#include "RatingSystem.h"
#include "Team9Assemble/AI/AIBeach_Observer.h"
#include "BeachGameState.h"
#include "Team9Assemble/Buildings/BuildingBase.h"
#include "Team9Assemble/Buildings/Building_Observer.h"

ARatingSystem::ARatingSystem() {
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.20f;
}

void ARatingSystem::BeginPlay() {
	Super::BeginPlay();
	// Timer = 30.0f;
	ABeachGameState* BeachGameState = Cast<ABeachGameState>(GetWorld()->GetGameState());
	AIObserver = BeachGameState->AIObserver;
	TrashObserver = BeachGameState->TrashObserver;
	BuildingObserver = BeachGameState->BuildingObserver;
}


void ARatingSystem::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

	CalculateHappiness(GetHappinessGoal());
	DecorValue = BuildingObserver->TotalDecoration + TrashObserver->TotalDecoration;
}

float ARatingSystem::GetHappinessGoal() {
	switch (GetBeachStarLevel()) {
	case 1:
		return RatingData->HappinessGoal02;
	case 2:
		return RatingData->HappinessGoal03;
	case 3:
		return RatingData->HappinessGoal04;
	case 4:
		return RatingData->HappinessGoal05;
	default:
		return 0;
	}
}

int ARatingSystem::GetEarningGoal() {
	switch (GetBeachStarLevel()) {
	case 1:
		return RatingData->EarningGoal02;
	case 2:
		return RatingData->EarningGoal03;
	case 3:
		return RatingData->EarningGoal04;
	case 4:
		return RatingData->EarningGoal05;
	default:
		return 0;
	}
}


int ARatingSystem::GetDecorGoal() {
	switch (GetBeachStarLevel()) {
	case 1:
		return RatingData->DecorGoal02;
	case 2:
		return RatingData->DecorGoal03;
	case 3:
		return RatingData->DecorGoal04;
	case 4:
		return RatingData->DecorGoal05;
	default:
		return 0;
	}
}

int ARatingSystem::GetLifeguardGoal() {
	switch (GetBeachStarLevel()) {
	case 1:
		return RatingData->LifeguardsRequired02;
	case 2:
		return RatingData->LifeguardsRequired03;
	case 3:
		return RatingData->LifeguardsRequired04;
	case 4:
		return RatingData->LifeguardsRequired05;
	default:
		return 0;
	}
}


bool ARatingSystem::ShowHappiness() {
	switch (GetBeachStarLevel()) {
		case 1:
			return RatingData->RequireHappiness02;
		case 2:
			return RatingData->RequireHappiness03;
		case 3:
			return RatingData->RequireHappiness04;
		case 4:
			return RatingData->RequireHappiness05;
		default:
			return false;
	}
}

bool ARatingSystem::ShowEarnings() {
	switch (GetBeachStarLevel()) {
	case 1:
		return RatingData->RequireEarnings02;
	case 2:
		return RatingData->RequireEarnings03;
	case 3:
		return RatingData->RequireEarnings04;
	case 4:
		return RatingData->RequireEarnings05;
	default:
		return false;
	}
}

bool ARatingSystem::ShowDecor() {
	switch (GetBeachStarLevel()) {
	case 1:
		return RatingData->RequireDecor02;
	case 2:
		return RatingData->RequireDecor03;
	case 3:
		return RatingData->RequireDecor04;
	case 4:
		return RatingData->RequireDecor05;
	default:
		return false;
	}
}

bool ARatingSystem::ShowBuildables() {
	switch (GetBeachStarLevel()) {
	case 1:
		return RatingData->RequireBuildings02;
	case 2:
		return RatingData->RequireBuildings03;
	case 3:
		return RatingData->RequireBuildings04;
	case 4:
		return RatingData->RequireBuildings05;
	default:
		return false;
	}
}

bool ARatingSystem::ShowLifeguards() {
	switch (GetBeachStarLevel()) {
	case 1:
		return RatingData->RequireLifeguards02;
	case 2:
		return RatingData->RequireLifeguards03;
	case 3:
		return RatingData->RequireLifeguards04;
	case 4:
		return RatingData->RequireLifeguards05;
	default:
		return false;
	}
}

TArray<TSubclassOf<ABuildingBase>> ARatingSystem::GetBuildingRequirements() {
	switch (GetBeachStarLevel()) {
	case 1:
		return RatingData->BuildingsRequired02;
	case 2:
		return RatingData->BuildingsRequired03;
	case 3:
		return RatingData->BuildingsRequired04;
	case 4:
		return RatingData->BuildingsRequired05;
	default:
		return RatingData->BuildingsRequired02;
	}
}

void ARatingSystem::CalculateHappiness(float Percentage) {
	// TArray<float> LowerPercentage;
	// for (int i = 0; i < AIObserver->Visitors.Num(); i++) {
	// 	LowerPercentage.Add(AIObserver->Visitors[i]->Mood.Happiness);
	// }
	// LowerPercentage.HeapSort();
	// if (LowerPercentage.Num() > 5) {
	//
	// 	int total = LowerPercentage.Num();
	// 	int top = total * (1 - Percentage);
	//
	// 	float totalHappiness = 0;
	//
	// 	for (int i = 0; i < total - top; i++) {
	// 		totalHappiness += LowerPercentage[i];
	// 		// UE_LOG(LogTemp, Log, TEXT("Sorted: %f"), Top70[i]);
	// 	}
	//
	// 	if (top != 0) {
	// 		AvarageHappiness = totalHappiness / (total - top);
	// 	}
	// 	else {
	// 		AvarageHappiness = 0;
	// 	}
	// }
	AvarageHappiness = AIObserver->GetVisitorHappiness();
}

bool ARatingSystem::CheckRequirements(int EarningsThreshold, bool RequireEarnings, float HappinessThreshold,
                                      bool RequireHappiness, int DecorThreshold, bool RequireDecor,
                                      TArray<TSubclassOf<ABuildingBase>> BuildingRequirements,
                                      bool RequireBuildings, int CurrentLifeguardCount, bool RequireLifeguard) {
	
	bool Earnings = false;
	bool Happiness = false;
	bool Decor = false;
	bool UseBuildings = false;
	bool LifeguardCounts = false;

	Earnings = Earned >= EarningsThreshold;
	if (RequireEarnings == false) {
		Earnings = true;
	}

	Happiness = AvarageHappiness >= HappinessThreshold;
	if (RequireHappiness == false) {
		Happiness = true;
	}

	Decor = DecorValue >= DecorThreshold;
	if (RequireDecor == false) {
		Decor = true;
	}

	UseBuildings = RequireBuildings;
	if (UseBuildings == true) {
		for (int i = 0; i < BuildingRequirements.Num(); i++) {
			if (BuildingObserver->CountBuildingOfName(
				BuildingRequirements[i].GetDefaultObject()->BuildingData->BuildingType,
				BuildingRequirements[i].GetDefaultObject()->BuildingData->BuildingName) == 0) {
				UseBuildings = false;
			}
		}
	} else {
		UseBuildings = true;
	}

	LifeguardCounts = AIObserver->LifeGuardCount >=  CurrentLifeguardCount;
	if (RequireLifeguard == false) {
		LifeguardCounts = true;
	}
		
	return (Earnings == true && Happiness == true && Decor == true && UseBuildings == true && LifeguardCounts == true);
}

int ARatingSystem::GetBeachStarLevel() {


	if ( CurrentRating == 4 && CheckRequirements(RatingData->EarningGoal05, RatingData->RequireEarnings05, RatingData->HappinessGoal05,
	                      RatingData->RequireHappiness05, RatingData->DecorGoal05, RatingData->RequireDecor05,
	                      RatingData->BuildingsRequired05, RatingData->RequireBuildings05, RatingData->LifeguardsRequired05, RatingData->RequireLifeguards05)) {
		
		if (CurrentRating < 5)
			CurrentRating = 5;
		
		return CurrentRating;
	}

	if (CurrentRating == 3 && CheckRequirements(RatingData->EarningGoal04, RatingData->RequireEarnings04, RatingData->HappinessGoal04,
	                      RatingData->RequireHappiness04, RatingData->DecorGoal04, RatingData->RequireDecor04,
	                      RatingData->BuildingsRequired04, RatingData->RequireBuildings04, RatingData->LifeguardsRequired04, RatingData->RequireLifeguards04)) {
		
		if (CurrentRating < 4)
			CurrentRating = 4;
		
		return CurrentRating;
	}

	if ( CurrentRating == 2 && CheckRequirements( RatingData->EarningGoal03, RatingData->RequireEarnings03, RatingData->HappinessGoal03,
	                      RatingData->RequireHappiness03, RatingData->DecorGoal03, RatingData->RequireDecor03,
	                      RatingData->BuildingsRequired03, RatingData->RequireBuildings03, RatingData->LifeguardsRequired03, RatingData->RequireLifeguards03)) {
		
		if (CurrentRating < 3)
			CurrentRating = 3;
		
		return CurrentRating;
	}

	if (CurrentRating == 1 && CheckRequirements(RatingData->EarningGoal02, RatingData->RequireEarnings02, RatingData->HappinessGoal02,
	                      RatingData->RequireHappiness02, RatingData->DecorGoal02, RatingData->RequireDecor02,
	                      RatingData->BuildingsRequired02, RatingData->RequireBuildings02, RatingData->LifeguardsRequired02, RatingData->RequireLifeguards02)) {
		
		if (CurrentRating < 2)
			CurrentRating = 2;
		
		return CurrentRating;
	}

	if (CurrentRating <= 1)
		CurrentRating = 1;
	
	return CurrentRating;
}

void ARatingSystem::AddEarnings(int Earnings) {
	Earned += Earnings;
}
