// I like sand


#include "BTTask_AIBeach_NotifyTaskChanged.h"
#include "Team9Assemble/AI/AILifeguard/AILifeGuard_Controller.h"
#include "Team9Assemble/AI/AILifeguard/TaskDriver/AILifeGuardTask_Baywatch.h"
#include "Team9Assemble/AI/AILifeguard/TaskDriver/AILifeGuardTask_BeachWork.h"
#include "Team9Assemble/AI/AILifeguard/TaskDriver/AILifeGuardTask_DoNothing.h"
#include "Team9Assemble/AI/AILifeguard/TaskDriver/AILifeGuardTask_Saving.h"
#include "Team9Assemble/AI/AILifeguard/TaskDriver/AILifeGuardTask_Drinking.h"
#include "Team9Assemble/AI/AILifeguard/TaskDriver/AILifeGuardTask_Eating.h"
#include "Team9Assemble/AI/AILifeguard/TaskDriver/AILifeGuardTask_Excrementing.h"
#include "Team9Assemble/AI/AILifeguard/TaskDriver/AILifeGuardTask_HasSpawned.h"
#include "Team9Assemble/AI/AILifeguard/TaskDriver/AILifeGuardTask_HeldByPlayer.h"
#include "Team9Assemble/AI/AILifeguard/TaskDriver/AILifeGuardTask_Training.h"
#include "Team9Assemble/AI/AILifeguard/TaskDriver/AILifeGuardTask_MoveTo.h"
#include "Team9Assemble/AI/AILifeguard/TaskDriver/AILifeGuardTask_Resting.h"
#include "Team9Assemble/AI/AILifeguard/TaskDriver/AILifeGuardTask_Roaming.h"
#include "Team9Assemble/AI/AIVisitor/AIVisitor_Controller.h"
#include "Team9Assemble/AI/AIVisitor/TaskDriver/AIVisitorTask_Bathing.h"
#include "Team9Assemble/AI/AIVisitor/TaskDriver/AIVisitorTask_Drinking.h"
#include "Team9Assemble/AI/AIVisitor/TaskDriver/AIVisitorTask_Drowning.h"
#include "Team9Assemble/AI/AIVisitor/TaskDriver/AIVisitorTask_Eating.h"
#include "Team9Assemble/AI/AIVisitor/TaskDriver/AIVisitorTask_Excrementing.h"
#include "Team9Assemble/AI/AIVisitor/TaskDriver/AIVisitorTask_FindingBeachPlace.h"
#include "Team9Assemble/AI/AIVisitor/TaskDriver/AIVisitorTask_GoingHome.h"
#include "Team9Assemble/AI/AIVisitor/TaskDriver/AIVisitorTask_MoveTo.h"
#include "Team9Assemble/AI/AIVisitor/TaskDriver/AIVisitorTask_Sunning.h"
#include "Team9Assemble/AI/AIVisitor/TaskDriver/AIVisitorTask_AtHospital.h"
#include "Team9Assemble/AI/AIVisitor/TaskDriver/AIVisitorTask_BeingRescued.h"
#include "Team9Assemble/AI/AIVisitor/TaskDriver/AIVisitorTask_Dead.h"
#include "Team9Assemble/AI/AIVisitor/TaskDriver/AIVisitorTask_TransportingDead.h"


UBTTask_AIBeach_NotifyTaskChanged::UBTTask_AIBeach_NotifyTaskChanged() {
	NodeName = "AIBeach Notify On Task Changed";
}

void UBTTask_AIBeach_NotifyTaskChanged::OnGameplayTaskActivated(UGameplayTask& Task) {}

void UBTTask_AIBeach_NotifyTaskChanged::InitializeFromAsset(UBehaviorTree& Asset) {}

void UBTTask_AIBeach_NotifyTaskChanged::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                                                 float DeltaSeconds) {}


EBTNodeResult::Type UBTTask_AIBeach_NotifyTaskChanged::
ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	if (AIType == EAIType::Visitor) {
		if (SetAIVisitorState(OwnerComp) == true) {
			return EBTNodeResult::Succeeded;
		}
	}
	else {
		if (SetAILifeGuardState(OwnerComp) == true) {
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}


bool UBTTask_AIBeach_NotifyTaskChanged::SetAIVisitorState(UBehaviorTreeComponent& OwnerComp) const {

	AAIVisitor_Controller* Visitor = Cast<AAIVisitor_Controller>(OwnerComp.GetAIOwner());
	if (Visitor != nullptr) {

		switch (VisitorTask) {
		case E_AIVisitor_TaskType::Sunning:
			Visitor->ChangeToState<AIVisitorTask_Sunning>();
			break;
		case E_AIVisitor_TaskType::Bathing:
			Visitor->ChangeToState<AIVisitorTask_Bathing>();
			break;
		case E_AIVisitor_TaskType::Drinking:
			Visitor->ChangeToState<AIVisitorTask_Drinking>();
			break;
		case E_AIVisitor_TaskType::Drowning:
			Visitor->ChangeToState<AIVisitorTask_Drowning>();
			break;
		case E_AIVisitor_TaskType::Eating:
			Visitor->ChangeToState<AIVisitorTask_Eating>();
			break;
		case E_AIVisitor_TaskType::Excrementing:
			Visitor->ChangeToState<AIVisitorTask_Excrementing>();
			break;
		case E_AIVisitor_TaskType::FindingPlace:
			Visitor->ChangeToState<AIVisitorTask_FindingBeachPlace>();
			break;
		case E_AIVisitor_TaskType::GoingHome:
			Visitor->ChangeToState<AIVisitorTask_GoingHome>();
			break;
		case E_AIVisitor_TaskType::Walking:
			Visitor->ChangeToState<AIVisitorTask_MoveTo>();
			break;
		case E_AIVisitor_TaskType::BeingSaved:
			Visitor->ChangeToState<AIVisitorTask_BeingRescued>();
			break;
		case E_AIVisitor_TaskType::Hospital:
			Visitor->ChangeToState<AIVisitorTask_AtHospital>();
			break;
		case E_AIVisitor_TaskType::Dead:
			Visitor->ChangeToState<AIVisitorTask_Dead>();
			break;
		case E_AIVisitor_TaskType::Transported:
			Visitor->ChangeToState<AIVisitorTask_TransportingDead>();
			break;
		default:
			UE_LOG(LogTemp, Error, TEXT("State Transition did not exist for AIVisitor"));
			return false;
		}

		return true;
	}
	return false;
}

bool UBTTask_AIBeach_NotifyTaskChanged::SetAILifeGuardState(UBehaviorTreeComponent& OwnerComp) const {
	AAILifeGuard_Controller* LifeGuard = Cast<AAILifeGuard_Controller>(OwnerComp.GetAIOwner());
	if (LifeGuard != nullptr) {

		switch (LifeGuardTask) {
		case E_AILifeGuard_TaskType::Baywatch:
			LifeGuard->ChangeToState<AILifeGuardTask_Baywatch>();
			break;
		case E_AILifeGuard_TaskType::Saving:
			LifeGuard->ChangeToState<AILifeGuardTask_Saving>();
			break;
		case E_AILifeGuard_TaskType::Drinking:
			LifeGuard->ChangeToState<AILifeGuardTask_Drinking>();
			break;
		case E_AILifeGuard_TaskType::Eating:
			LifeGuard->ChangeToState<AILifeGuardTask_Eating>();
			break;
		case E_AILifeGuard_TaskType::Excrementing:
			LifeGuard->ChangeToState<AILifeGuardTask_Excrementing>();
			break;
		case E_AILifeGuard_TaskType::Training:
			LifeGuard->ChangeToState<AILifeGuardTask_Training>();
			break;
		case E_AILifeGuard_TaskType::BeachWork:
			LifeGuard->ChangeToState<AILifeGuardTask_BeachWork>();
			break;
		case E_AILifeGuard_TaskType::Roaming:
			LifeGuard->ChangeToState<AILifeGuardTask_Roaming>();
			break;
		case E_AILifeGuard_TaskType::Resting:
			LifeGuard->ChangeToState<AILifeGuardTask_Resting>();
			break;
		case E_AILifeGuard_TaskType::MoveTo:
			LifeGuard->ChangeToState<AILifeGuardTask_MoveTo>();
			break;
		case E_AILifeGuard_TaskType::HeldByPlayer:
			LifeGuard->ChangeToState<AILifeGuardTask_HeldByPlayer>();
			break;
		case E_AILifeGuard_TaskType::HasSpawned:
			LifeGuard->ChangeToState<AILifeGuardTask_HasSpawned>();
			break;
		default:
			LifeGuard->ChangeToState<AILifeGuardTask_DoNothing>();
			// UE_LOG(LogTemp, Error, TEXT("State Transition did not exist for AILifeGuard"));
			return true;
		}

		return true;
	}
	return false;
}
