// I like sand

#pragma once

#include "CoreMinimal.h"

#include "AIVisitor/AIVisitor_Controller.h"
#include "AILifeGuard/AILifeGuard_Controller.h"
#include "AIBeach_Observer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLifeguardHired);

UCLASS(BlueprintType,Blueprintable)
class TEAM9ASSEMBLE_API UAIBeach_Observer : public UObject {
	GENERATED_BODY()

public:
	UAIBeach_Observer();

	UFUNCTION(BlueprintPure)
	float GetVisitorHappiness();
	
	UFUNCTION(BlueprintPure)
	float GetVisitorNeedBodyTemperature();
	
	UFUNCTION(BlueprintPure)
	float GetVisitorNeedThirst();

	UFUNCTION(BlueprintPure)
	float GetVisitorNeedHunger(); 
	
	UFUNCTION(BlueprintPure)
	float GetVisitorNeedToilet();

	UFUNCTION(BlueprintPure)
	int GetVisitorCount();

	UFUNCTION(BlueprintPure)
	float GetLifeguardNeedRest();
	
	UFUNCTION(BlueprintPure)
	float GetLifeguardNeedRecreation();

	UFUNCTION(BlueprintPure)
	float GetLifeguardNeedToilet(); 
	
	UFUNCTION(BlueprintPure)
	float GetLifeguardMoodMorale();
	
	void NewVisitor(AAIVisitor_Controller* Visitor);
	void NewLifeGuard(AAILifeGuard_Controller* LifeGuard);

	void RemoveVisitor(AAIVisitor_Controller* Visitor);
	void RemoveLifeGuard(AAILifeGuard_Controller* LifeGuard);

	void AddDrowner(AAIVisitor_Controller* Drowner);
	void RemoveDrowner(AAIVisitor_Controller* Drowner);

	void AddDeadVisitor(AAIVisitor_Controller* Visitor);

	void RemoveDeadVisitor(AAIVisitor_Controller* Visitor);
	void NotifyOnVisitorDeath();

	void StartSaveDrowner(AAIVisitor_Controller* Drowner, AAILifeGuard_Controller* LifeGuard);
	void FinishSaveDrowner(AAIVisitor_Controller* Drowner);
	void CancelSaveDrowner(AAIVisitor_Controller* Drowner);

	TArray<AAIVisitor_Controller*> GetDrowners();

	UPROPERTY(BlueprintAssignable)
	FOnLifeguardHired OnLifeguardHired;
	
	UPROPERTY(VisibleAnywhere, Category="AI Visitor Info")
	TArray<AAIVisitor_Controller*> Visitors;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="AI Visitor Info")
	int VisitorCount;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="AI Visitor Info")
	int VisitorTotalCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Visitor Data")
	float DeadHappinessSubtraction = -0.05f;

	UPROPERTY(VisibleAnywhere, Category="AI Life Guard Info")
	TArray<AAILifeGuard_Controller*> Lifeguards;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="AI Life Guard Info")
	int LifeGuardCount;
	UPROPERTY(VisibleAnywhere, Category="AI Visitor Info")
	int LifeGuardTotalCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI Drowner Info")
	TArray<AAIVisitor_Controller*> DeadVisitors;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI Drowner Info")
	int DeadVisitorCount;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="AI Drowner Info")
	TArray<AAIVisitor_Controller*> Drowners;
	UPROPERTY(VisibleAnywhere, Category="AI Drowner Info")
	TMap<AAIVisitor_Controller*, AAILifeGuard_Controller*> Savers;

	UPROPERTY(EditDefaultsOnly, Category="AIVisitor")
	UBehaviorTree* AIVisitor_BehaviourTree;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="AIVisitor DataAssets")
	U_AIVistor_TaskData* AIVisitor_TaskData;

	UPROPERTY(EditDefaultsOnly, Category="AILifeGuard")
	UBehaviorTree* AILifeGuard_BehaviourTree;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="AILifeGuard DataAssets", meta = (ScriptName = "Need Normal Rate"))
	U_AILifeGuard_TaskData* AILifeGuard_TaskData;

	UFUNCTION(BlueprintCallable)
	AAILifeGuard_Controller* GetClosestAvailableLifeguard(FVector Location);
};