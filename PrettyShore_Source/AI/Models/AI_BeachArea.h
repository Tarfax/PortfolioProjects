// I like sand

#pragma once

#include "CoreMinimal.h"

#include "Team9Assemble/AI//AIVisitor/AIVisitor_Controller.h"
#include "GameFramework/Actor.h"
#include "AI_BeachArea.generated.h"

class AAIVisitor_Controller;

UCLASS()
class AAI_BeachArea : public AActor {
	GENERATED_BODY()

public:
	AAI_BeachArea();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
#if WITH_EDITORONLY_DATA
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public: //Functions
	UFUNCTION(BlueprintCallable)
	FVector FindRandomLocation();

	UFUNCTION(BlueprintCallable)
	FVector ReservLocation(AAIVisitor_Controller* Visitor);
	
	UFUNCTION(BlueprintCallable)
	bool AddBeachVisitor(AAIVisitor_Controller* Visitor, FVector Position, FVector& PositionForHome);

	UFUNCTION(BlueprintCallable)
	void RemoveBeachVisitor(AAIVisitor_Controller* Visitor);

public: //Variables
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBoxComponent* Area;

	UPROPERTY(EditInstanceOnly, Category="Beach Area")
	int SizeX = 3000;
	UPROPERTY(EditInstanceOnly, Category="Beach Area")
	int SizeY = 12000;

	UPROPERTY(VisibleAnywhere, Category="Beach Area")
	TMap<AAIVisitor_Controller*, FVector> AIHomePositions;

	UPROPERTY(VisibleAnywhere, Category="Beach Area")
	TMap<FVector, AAIVisitor_Controller*> OccupiedPositions;
	
	UPROPERTY(VisibleAnywhere, Category="Beach Area")
	TMap<AAIVisitor_Controller*, FVector> ReservedBeach;
	
	FVector RootLocation;

};
