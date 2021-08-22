#include "BTTask_AILifeguard_BeachWork_FindNearest.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Team9Assemble/AI/AILifeguard/AILifeGuard_Controller.h"
#include "Team9Assemble/AI/AI_BeachCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Team9Assemble/AI/Models/AI_Trash.h"
#include "Team9Assemble/Buildings/BuildingBase.h"

UBTTask_AILifeguard_BeachWork_FindNearest::UBTTask_AILifeguard_BeachWork_FindNearest() {}
void UBTTask_AILifeguard_BeachWork_FindNearest::OnGameplayTaskActivated(UGameplayTask& Task) {}

EBTNodeResult::Type UBTTask_AILifeguard_BeachWork_FindNearest::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
                                                                           uint8* NodeMemory) {

	TArray<AActor*> OverlappedActors;
	AAILifeGuard_Controller* Controller = Cast<AAILifeGuard_Controller>(OwnerComp.GetAIOwner());
	UKismetSystemLibrary::SphereOverlapActors(Controller, Controller->AI_BeachCharacter->GetActorLocation(), SearchRadius,
	                                          OverlapObjectTypes, AActor::StaticClass(), TArray<AActor*>(),
	                                          OverlappedActors);

	// UKismetSystemLibrary::DrawDebugSphere(Controller, Controller->AI_BeachCharacter->GetActorLocation(), SearchRadius, 12, FLinearColor::White, 15, 15);
	
	
	for (AActor* Actor : OverlappedActors) {
		if (WorkTypeToSearchFor == EBeachWorkType::PickupGarbage) {
			if (SearchForTrash(Actor, Controller, OwnerComp) != nullptr) {
				return EBTNodeResult::Succeeded;
			}
		}

		if (WorkTypeToSearchFor == EBeachWorkType::RemoveTowel) {
			if (SearchForTowel(Actor, Controller, OwnerComp) != nullptr) {
				return EBTNodeResult::Succeeded;
			}
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BeachWorkKey.SelectedKeyName, nullptr);
	return EBTNodeResult::Failed;
}

void UBTTask_AILifeguard_BeachWork_FindNearest::InitializeFromAsset(UBehaviorTree& Asset) {}

void UBTTask_AILifeguard_BeachWork_FindNearest::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                                                         float DeltaSeconds) {}

AAI_Trash* UBTTask_AILifeguard_BeachWork_FindNearest::SearchForTrash(AActor* Actor, AAILifeGuard_Controller* Controller,
                                                                     UBehaviorTreeComponent& OwnerComp) const {
	AAI_Trash* Trash = Cast<AAI_Trash>(Actor);
	if (Trash != nullptr) {
		UBeachWork* BeachWork = Trash->BeachWork;
		if (BeachWork != nullptr && BeachWork->WorkOwner == nullptr) {
			Controller->AssignBeachWork(BeachWork);
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(BeachWorkKey.SelectedKeyName, BeachWork);
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(DestinationKey.SelectedKeyName,
			                                                     BeachWork->WorkLocation.GetLocation());

			return Trash;
		}
	}

	return nullptr;
}

ABuildingBase* UBTTask_AILifeguard_BeachWork_FindNearest::SearchForTowel(AActor* Actor,
                                                                         AAILifeGuard_Controller* Controller,
                                                                         UBehaviorTreeComponent& OwnerComp) const {
	ABuildingBase* Building = Cast<ABuildingBase>(Actor);
	if (Building != nullptr) {
		for (int i = 0; i < Building->BeachWorks.Num(); i++) {
			if (Building->BeachWorks[i]->WorkType == EBeachWorkType::RemoveTowel) {
				UBeachWork* BeachWork = Building->BeachWorks[i];
				if (BeachWork != nullptr && BeachWork->WorkOwner == nullptr) {
					Controller->AssignBeachWork(BeachWork);
					OwnerComp.GetBlackboardComponent()->SetValueAsObject(BeachWorkKey.SelectedKeyName, BeachWork);
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(DestinationKey.SelectedKeyName,
					                                                     BeachWork->WorkLocation.GetLocation());

					return Building;
				}
			}
		}
	}
	return nullptr;
}
