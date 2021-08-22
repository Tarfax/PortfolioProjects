#include "AILifeGuardTask_HeldByPlayer.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Team9Assemble/AI/AIBeach_Observer.h"
#include "Team9Assemble/AI/AI_BeachCharacter.h"
#include "Team9Assemble/AI/Models/AI_Trash.h"
#include "Team9Assemble/Buildings/BuildingBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Team9Assemble/Buildings/Building_Observer.h"
// #include "Kismet/KismetMathLibrary.h"

void AILifeGuardTask_HeldByPlayer::Enter() {
	Owner->SetDebugText.Broadcast("Held By Player");
	Owner->DoingTask = "Held By Player";

	Owner->AI_BeachCharacter->GetCapsuleComponent()->SetEnableGravity(false);
}

void AILifeGuardTask_HeldByPlayer::OnTick(float DeltaTime) {
	if (Owner->IsPlayerControlled == false) {
		Owner->MakeDecision();

		// CheckForTask();

		// FVector Position;
		// FVector Direction;
		// Owner->GetWorld()->GetFirstPlayerController()->DeprojectMousePositionToWorld(Position, Direction);
		// FHitResult HitResult;
		// ECollisionChannel CollisionChannel = ECC_GameTraceChannel3;
		// Owner->GetWorld()->LineTraceSingleByChannel(HitResult, Position, Position + (Direction * 40000),
		// 											CollisionChannel);
		//
		// const float LockedHeight = 400.f;
		// const float CamToCloseThreshold = 3000.f;
		//
		// const FVector HitLocation = HitResult.ImpactPoint;
		// const FVector StartLocation = HitResult.TraceStart;
		// const float DistToCam = (StartLocation - HitLocation).Size();
		//
		// // Calculate the angle of the -Direction vector, which points from the Hit.ImpactPoint back towards the camera.
		// FVector VecA = FVector::UpVector;
		// FVector VecB = -Direction.GetSafeNormal();
		// const float AngA = acosf((VecA.X * VecB.X + VecA.Y * VecB.Y + VecA.Z * VecB.Z) /
		// 	(sqrtf(powf(VecA.X,2) + powf(VecA.Y,2) + powf(VecA.Z,2)) *
		// 	sqrtf(powf(VecB.X,2) + powf(VecB.Y,2) + powf(VecB.Z,2))));
		//
		// const float AngB = 1.570796f; // 90 degrees in radians.
		// const float AngC = UKismetMathLibrary::GetPI() - AngA - AngB; // pi = 180 degrees in radians.
		//
		// const float LengthAlongGround = LockedHeight * (sinf(AngA) / sinf(AngC));
		// const float LAG = DistToCam < CamToCloseThreshold ? 0.f : LengthAlongGround;
		//
		// const FVector DirToCamXY = ((StartLocation * FVector(1.f, 1.f, 0.f)) -
		// 	(HitLocation * FVector(1.f, 1.f, 0.f))).GetSafeNormal();
		//
		// const FVector HoldActorOffset = FVector(0.f, 0.f, -200.f);
		//
		// const FVector HoverLocation = HitLocation + FVector(LAG * DirToCamXY.X,
		// 	LAG * DirToCamXY.Y, LockedHeight) + HoldActorOffset;
		//
		// const FVector LerpLocation = UKismetMathLibrary::VLerp(
		// 	Owner->AI_BeachCharacter->GetActorLocation(), HoverLocation, .1f);
		//
		// // Only move the position of the Lifeguard if inside beach area.
		// if (HitResult.bBlockingHit)
		// 	Owner->AI_BeachCharacter->SetActorLocation(LerpLocation);


	}
	else {
		CheckForTask();
		// FVector ActorLocation = Owner->AI_BeachCharacter->GetActorLocation();
		// if (ActorLocation.Z < 100) {
		// 	ActorLocation.Z = 100;
		// 	Owner->AI_BeachCharacter->SetActorLocation(ActorLocation);
		// }
		// if (SetSavingTask() == false) {
		// 	if (SetBeachWorkTask() == false) {
		// }
		// }
	}
}

void AILifeGuardTask_HeldByPlayer::Exit() {
	Owner->AI_BeachCharacter->GetCapsuleComponent()->SetEnableGravity(true);
	if (CanSave == true) {
		SetSavingTask();
	} else if (HasWork == true) {
		SetBeachWorkTask();
	}
}

void AILifeGuardTask_HeldByPlayer::Reset() { }

void AILifeGuardTask_HeldByPlayer::CheckForTask() {

	FVector SearchPosition = Owner->AI_BeachCharacter->GetActorLocation();

	// UKismetSystemLibrary::DrawDebugSphere(Owner, SearchPosition, 2500,
	// 											25.0f, FLinearColor::White, 0, 15.0f);
	//
	TArray<AActor*> OverlappedActors;
	UKismetSystemLibrary::SphereOverlapActors(Owner, Owner->AI_BeachCharacter->GetActorLocation(), 2500,
	                                          Owner->OverlapTaskTypes, AActor::StaticClass(), TArray<AActor*>(),
	                                          OverlappedActors);


	float ClosestDistance = 9999999.101010101f;

	AAIVisitor_Controller* ADrowner = nullptr;

	// Trash = nullptr;
	// Building = nullptr;
	// Drowner = nullptr;

	ABeachGameState* BeachGameState = Cast<ABeachGameState>(Owner->GetWorld()->GetGameState());

	CanSave = false;
	HasWork = false;

	AAI_Trash* closestTrash = nullptr;
	ABuildingBase* closestBuilding = nullptr;

	Task = E_AILifeGuard_TaskType::Nothing;
	for (auto* Actor : OverlappedActors) {
		AAI_Trash* aTrash = nullptr;
		ABuildingBase* aBuilding = nullptr;
		if (Owner->CanDoBeachWork == true) {
			aTrash = Cast<AAI_Trash>(Actor);
			aBuilding = Cast<ABuildingBase>(Actor);
		}

		const float Distance = FVector::Distance(Actor->GetActorLocation(), SearchPosition);

		if (Distance < ClosestDistance) {


			if (aTrash != nullptr) {
				UE_LOG(LogTemp, Log, TEXT("TRASH!"));
				closestTrash = aTrash;

				ClosestDistance = Distance;
				HasWork = true;
			}

			if (BeachGameState->BuildingObserver->HasWork() == true) {
				if (aBuilding != nullptr && aBuilding->BeachWorks.Num() > 0) {
					UE_LOG(LogTemp, Log, TEXT("BUILDING"));

					closestBuilding = aBuilding;

					ClosestDistance = Distance;
					HasWork = true;
				}
			}
			// else {
			// 	Building = nullptr;
			// }

			Task = E_AILifeGuard_TaskType::BeachWork;
		}

		if (Owner->CanRescue == false) {
			AAIVisitor_Controller* visitor = Cast<AAIVisitor_Controller>(Actor);
			if (visitor != nullptr && visitor->IsBeingRescued == false && visitor->IsDrowning == true) {
				ADrowner = visitor;
				CanSave = true;
			}
		}
	}

	Trash = closestTrash;
	Building = closestBuilding;
	Drowner = ADrowner;

	if (Drowner != nullptr) {
		UKismetSystemLibrary::DrawDebugArrow(Owner, SearchPosition, Drowner->AI_BeachCharacter->GetActorLocation(),
		                                     25.0f, FLinearColor::White, 0, 5.0f);

		UE_LOG(LogTemp, Log, TEXT("Drowner!=nullptr"));
	}
	else if (HasWork == true && Building != nullptr) {
		UKismetSystemLibrary::DrawDebugArrow(Owner, SearchPosition, Building->GetActorLocation(),
		                                     25.0f, FLinearColor::White, 0, 5.0f);
		UE_LOG(LogTemp, Log, TEXT("Building!=nullptr"));

	}
	else if (HasWork == true && Trash != nullptr) {
		UKismetSystemLibrary::DrawDebugArrow(Owner, SearchPosition, Trash->GetActorLocation(),
		                                     25.0f, FLinearColor::White, 0, 5.0f);
		UE_LOG(LogTemp, Log, TEXT("Trash!=nullptr"));
	}
}

bool AILifeGuardTask_HeldByPlayer::SetSavingTask() {
	if (Drowner != nullptr) {
		Owner->RescuingVisitor = Drowner;
		Owner->RescuingVisitor->IsBeingRescued = true;

		Owner->AIObserver->StartSaveDrowner(Drowner, Owner);
		Owner->BlackBoardAccessor->SetValue<UBlackboardKeyType_Object>(
			Owner->BlackBoardAccessor->GetKeyID("DrowningVisitor"),
			Drowner);
		Owner->PerformingTask = E_AILifeGuard_TaskType::Saving;
		Owner->SetNewTaskInBehaviourTree(Owner->PerformingTask);
		Owner->CanBePickedUp = false;
		return true;
	}
	return false;
}

bool AILifeGuardTask_HeldByPlayer::SetBeachWorkTask() {

	if (Building != nullptr) {
		UBeachWork* BeachWork = nullptr;

		for (int i = 0; i < Building->BeachWorks.Num(); i++) {
			if (Building->BeachWorks[i] != nullptr) {
				if (Building->BeachWorks[i]->WorkOwner == nullptr) {
					BeachWork = Building->BeachWorks[i];
					break;
				}
			}
		}

		if (BeachWork != nullptr) {

			Owner->AssignBeachWork(BeachWork);

			Owner->BlackBoardAccessor->SetValueAsObject("BeachWork", Trash->BeachWork);
			Owner->BlackBoardAccessor->SetValueAsVector("Destination",
			                                            Trash->BeachWork->WorkLocation.GetLocation());
			Owner->BlackBoardAccessor->SetValueAsEnum("BeachWorkType",
			                                          static_cast<UBlackboardKeyType_Enum::FDataType>(Trash->BeachWork->
				                                          WorkType));

			Owner->SetNewTaskInBehaviourTree(E_AILifeGuard_TaskType::BeachWork);

			UE_LOG(LogTemp, Log, TEXT("BeachWork BUILDING should now take place."));

			return true;
		}
	}


	if (Trash != nullptr) {
		Owner->AssignBeachWork(Trash->BeachWork);

		Owner->BlackBoardAccessor->SetValueAsObject("BeachWork", Trash->BeachWork);
		Owner->BlackBoardAccessor->SetValueAsVector("Destination",
		                                            Trash->BeachWork->WorkLocation.GetLocation());
		Owner->BlackBoardAccessor->SetValueAsEnum("BeachWorkType",
		                                          static_cast<UBlackboardKeyType_Enum::FDataType>(Trash->BeachWork->
			                                          WorkType));

		Owner->SetNewTaskInBehaviourTree(E_AILifeGuard_TaskType::BeachWork);

		UE_LOG(LogTemp, Log, TEXT("BeachWork TRASH should now take place."));

		return true;
	}
	return false;
}
