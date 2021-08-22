#pragma once
#include "RatingData.generated.h"
class ABuildingBase;
class AAILifeGuard_Controller;

UCLASS(Blueprintable, BlueprintType)
class URatingData : public UDataAsset {
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Two Star")
	float HappinessGoal02 = 0.5f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Two Star")
	bool RequireHappiness02 = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Two Star")
	int EarningGoal02 = 15000;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Two Star")
	bool RequireEarnings02 = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Two Star")
	int DecorGoal02 = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Two Star")
	bool RequireDecor02 = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Two Star")
	int LifeguardsRequired02;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Two Star")
	bool RequireLifeguards02 = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Two Star")
	TArray<TSubclassOf<ABuildingBase>> BuildingsRequired02;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Two Star")
	bool RequireBuildings02 = false;
	
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Three Star")
	float HappinessGoal03 = 0.6f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Three Star")
	bool RequireHappiness03 = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Three Star")
	int EarningGoal03 = 30000;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Three Star")
	bool RequireEarnings03 = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Three Star")
	int DecorGoal03 = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Three Star")
	bool RequireDecor03 = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Three Star")
	int LifeguardsRequired03;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Three Star")
	bool RequireLifeguards03 = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Three Star")
	TArray<TSubclassOf<ABuildingBase>> BuildingsRequired03;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Three Star")
	bool RequireBuildings03 = false;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Four Star")
	float HappinessGoal04 = 0.7f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Four Star")
	bool RequireHappiness04 = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Four Star")
	int EarningGoal04 = 60000;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Four Star")
	bool RequireEarnings04 = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Four Star")
	int DecorGoal04 = 100;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Four Star")
	bool RequireDecor04 = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Four Star")
	int LifeguardsRequired04;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Four Star")
	bool RequireLifeguards04 = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Four Star")
	TArray<TSubclassOf<ABuildingBase>> BuildingsRequired04;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Four Star")
	bool RequireBuildings04 = false;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Five Star")
	float HappinessGoal05 = 0.8f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Five Star")
	bool RequireHappiness05 = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Five Star")
	int EarningGoal05 = 120000;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Five Star")
	bool RequireEarnings05 = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Five Star")
	int DecorGoal05 = 200;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Five Star")
	bool RequireDecor05 = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Five Star")
	int LifeguardsRequired05;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Five Star")
	bool RequireLifeguards05 = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Five Star")
	TArray<TSubclassOf<ABuildingBase>> BuildingsRequired05;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Five Star")
	bool RequireBuildings05 = false;
	
};
