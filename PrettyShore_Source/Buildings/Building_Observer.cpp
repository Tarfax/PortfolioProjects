// I like sand


#include "Building_Observer.h"
#include "BuildingBase.h"
#include "Team9Assemble/AI/Models/BeachWork.h"

UBuilding_Observer::UBuilding_Observer() {
	
}

void UBuilding_Observer::AddBuilding(ABuildingBase* Building) {
	if (Building != nullptr) {
		// GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green,
		//                                  TEXT("Added a building of type "+Building->BuildingData->BuildingName));
		Buildings.Add(Building);
		if (BuildingTypes.Contains(Building->BuildingData->BuildingType) == false) {
			TArray<ABuildingBase*> list;
			list.Add(Building);
			BuildingTypes.Add(Building->BuildingData->BuildingType, list);
		}
		else {
			BuildingTypes[Building->BuildingData->BuildingType].Add(Building);
		}
		TotalDecoration += Building->BuildingData->DecorationValue;
		
		if (Building ->BuildingData->BuildingType == E_BuildingType::WatchTower) {
			if (BuildingTypes.Contains(E_BuildingType::Hospital) == false) {
				
				if (GameState != nullptr) {
					GameState->SendLogMessage("You must buy a Medical Tent to have a fully functional Watchtower.",FLinearColor::Red, Building);
				}
			}
		}

		OnBuildingConstructed.Broadcast();
	}

}

void UBuilding_Observer::RemoveBuilding(ABuildingBase* Building) {
	if (Building != nullptr) {
		Buildings.Remove(Building);
		BuildingTypes[Building->BuildingData->BuildingType].Remove(Building);
		// NeedsRestocking.Remove(Building);
		// NeedsCleaning.Remove(Building);
		TotalDecoration -= Building->BuildingData->DecorationValue;
	}
	OnBuildingConstructed.Broadcast();
}

void UBuilding_Observer::AddNeedsStocking(UBeachWork* Building) {
	UE_LOG(LogTemp, Log, TEXT("AddNeedsRestock"));
	NeedsRestocking.AddUnique(Building);
}

void UBuilding_Observer::RemoveNeedsStocking(UBeachWork* Building) {
	UE_LOG(LogTemp, Log, TEXT("RemovedNeedsRestock"));
	NeedsRestocking.Remove(Building);
}

void UBuilding_Observer::AddNeedsCleaning(UBeachWork* Building) {
	UE_LOG(LogTemp, Log, TEXT("AddNeedsCleanBuilding"));
	NeedsCleaning.AddUnique(Building);
	// TotalDecoration -= 6;
}

void UBuilding_Observer::RemoveNeedsCleaning(UBeachWork* Building) {
	UE_LOG(LogTemp, Log, TEXT("RemovedNeedsCleanBuilding"));
	NeedsCleaning.Remove(Building);
	// TotalDecoration += 6;
}

void UBuilding_Observer::AddNeedsHomeRemoval(UBeachWork* Building) {
	UE_LOG(LogTemp, Log, TEXT("AddNeedsHomeRemoval"));
	NeedsHomeRemoval.AddUnique(Building);
}

void UBuilding_Observer::RemoveNeedsHomeRemoval(UBeachWork* Building) {
	UE_LOG(LogTemp, Log, TEXT("RemoveNeedsHomeRemoval"));
	NeedsHomeRemoval.Remove(Building);
}


int UBuilding_Observer::CountBuildingOfType(E_BuildingType Type) {
	if (BuildingTypes.Contains(Type) == false) {
		return 0;
	}
	return BuildingTypes[Type].Num();
}

int UBuilding_Observer::CountBuildingOfName(E_BuildingType Type, FString BuildingName) {
	int amount = 0;
	if (BuildingTypes.Contains(Type) != false)
	{
		for (int i = 0; i < BuildingTypes[Type].Num(); i++) {
			if (BuildingTypes[Type][i]->BuildingData->BuildingName == BuildingName) {
				amount++;
			}
		}
	}
	return amount;
}

ABuildingBase* UBuilding_Observer::GetClosestBuildingOfType(E_BuildingType Type, FVector SearchPosition,
                                                            bool IgnoreMaxUsers, AAIBeach_Controller_Base* AIController) {
	if (BuildingTypes.Contains(Type) == false) {
		return nullptr;
	}

	float ClosestDistance = 9999999999999999.101010101f;
	ABuildingBase* ClosestBuilding = nullptr;

	for (int i = 0; i < BuildingTypes[Type].Num(); i++) {
		ABuildingBase* Building = BuildingTypes[Type][i];
		if (Building->ReservedUsers.Contains(AIController) == true) {
			return Building;
		}
		if (Building->Users.Num() + Building->ReservedUsers.Num() < Building->BuildingData->MaxUsers
			|| IgnoreMaxUsers == true || Building->BuildingData->MaxUsers < 1) {
			const float Distance = FVector::Distance(Building->GetActorLocation(), SearchPosition);
			if (Distance < ClosestDistance) {
				ClosestDistance = Distance;
				ClosestBuilding = Building;
			}
		}
		else if (Building->BuildingData == nullptr) {
			FString msg = "Could not iterate through BuildingBehaviour Data Asset. Is it null?? Building: ";
			msg += *Building->GetOwner()->GetName();
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT(""+msg));
		}
	}

	return ClosestBuilding;
}

ABuildingBase* UBuilding_Observer::GetAvailableHome() {
	E_BuildingType Type = E_BuildingType::VisitorHome;
	if (BuildingTypes.Contains(Type) == false) {
		return nullptr;
	}

	for (int i = 0; i < BuildingTypes[Type].Num(); i++) {
		ABuildingBase* Building = BuildingTypes[Type][i];
		if (Building->BuildingData->BuildingType == E_BuildingType::VisitorHome) {
			if (Building->AIVisitor_Owner == nullptr && Building->Uses == 0 && Building->ReservedUsers.Num() == 0) {
				return Building;
			}
		}
	}
	return nullptr;
}

bool UBuilding_Observer::HasWork() const {
	if (NeedsRestocking.Num() > 0 || NeedsCleaning.Num() > 0 || NeedsHomeRemoval.Num() > 0) {
		return true;
	}
	return false;
}

UBeachWork* UBuilding_Observer::GetWork(FVector StartLocation) {
	for (int i = 0; i < NeedsRestocking.Num(); i++) {
		if (NeedsRestocking[i]->WorkOwner == nullptr) {
			return NeedsRestocking[i];
		}
	}

	for (int i = 0; i < NeedsCleaning.Num(); i++) {
		if (NeedsCleaning[i]->WorkOwner == nullptr) {
			return NeedsCleaning[i];
		}
	}

	float ShortestDistance = 999999999999999;
	UBeachWork* ClosestWork = nullptr;
	for (int i = 0; i < NeedsHomeRemoval.Num(); i++) {
		float Distance = FVector::DistSquared(StartLocation, NeedsHomeRemoval[i]->WorkLocation.GetLocation());
		if (NeedsHomeRemoval[i]->WorkOwner == nullptr && Distance < ShortestDistance) {
			ShortestDistance = Distance;
			ClosestWork = NeedsHomeRemoval[i];
		}
	}

	return ClosestWork;
}
