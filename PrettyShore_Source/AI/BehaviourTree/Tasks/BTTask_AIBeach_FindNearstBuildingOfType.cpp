// I like sand


#include "BTTask_AIBeach_FindNearstBuildingOfType.h"
#include "AIController.h"
#include "Team9Assemble/Buildings/BuildingBase.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "GameFramework/Character.h"
#include "Team9Assemble/AI/AIBeach_Controller_Base.h"
#include "Team9Assemble/Buildings/Building_Observer.h"
#include "Team9Assemble/World/BeachGameState.h"

UBTTask_AIBeach_FindNearstBuildingOfType::UBTTask_AIBeach_FindNearstBuildingOfType() {
	NodeName = "AIBeach Find Building";
}

void UBTTask_AIBeach_FindNearstBuildingOfType::OnGameplayTaskActivated(UGameplayTask& Task) { }

EBTNodeResult::Type UBTTask_AIBeach_FindNearstBuildingOfType::SearchForAvailableHome(
	UBehaviorTreeComponent& OwnerComp, UBlackboardComponent* BlackboardComp, const FVector CharacterPosition,
	UBuilding_Observer* BuildingObserver) {
	ABuildingBase* Building = BuildingObserver->GetAvailableHome();
	if (Building != nullptr) {
		AAIBeach_Controller_Base* AIController = Cast<AAIBeach_Controller_Base>(OwnerComp.GetOwner());
		AIController->ReservBuilding(Building);
		
		BlackboardComp->SetValue<UBlackboardKeyType_Object>(BuildingComponentKey.SelectedKeyName, Building);
		BlackboardComp->SetValue<UBlackboardKeyType_Vector>(DestinationKey.SelectedKeyName,
		                                                    /*Building->GetActorLocation() +*/
		                                                    Building->ArrowEnter->GetComponentLocation());

		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}

EBTNodeResult::Type UBTTask_AIBeach_FindNearstBuildingOfType::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
                                                                          uint8* NodeMemory) {
	Super::ExecuteTask(OwnerComp, NodeMemory);
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	const FVector CharacterPosition = OwnerComp.GetAIOwner()->GetCharacter()->GetActorLocation();

	UBuilding_Observer* BuildingObserver = Cast<ABeachGameState>(GetWorld()->GetGameState())->BuildingObserver;

	if (SearchForHome == true) {
		return SearchForAvailableHome(OwnerComp, BlackboardComp, CharacterPosition, BuildingObserver);
	}

	AAIBeach_Controller_Base* AIController = Cast<AAIBeach_Controller_Base>(OwnerComp.GetOwner());
	ABuildingBase* Building = BuildingObserver->GetClosestBuildingOfType(
		SearchForBuildingType, CharacterPosition, IgnoreBuildingMaximumUsers, AIController);
	if (Building != nullptr) {
		BlackboardComp->SetValue<UBlackboardKeyType_Object>(BuildingComponentKey.SelectedKeyName, Building);
		BlackboardComp->SetValue<UBlackboardKeyType_Vector>(DestinationKey.SelectedKeyName,
		                                                    /*Building->GetActorLocation() +*/
		                                                    Building->ArrowEnter->GetComponentLocation());

		AIController->ReservBuilding(Building);

		return EBTNodeResult::Succeeded;
	}

	// UE_LOG(LogTemp, Warning, TEXT("FindNearstBuildingOfType::ExecuteTask:: Getting building FAILED"));
	return EBTNodeResult::Failed;
}

void UBTTask_AIBeach_FindNearstBuildingOfType::InitializeFromAsset(UBehaviorTree& Asset) {
	Super::InitializeFromAsset(Asset);

}

void UBTTask_AIBeach_FindNearstBuildingOfType::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                                                        float DeltaSeconds) {}
