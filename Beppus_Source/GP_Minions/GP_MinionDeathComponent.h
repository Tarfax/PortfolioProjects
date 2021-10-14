// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GP_MinionDeath.h"
#include "Components/ActorComponent.h"
#include "GP_MinionCommands/EGP_MinionCommand.h"
#include "GP_MinionDeathComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable, BlueprintType)
class GP4_API UGP_MinionDeathComponent : public UActorComponent {
	GENERATED_BODY()

public:
	UGP_MinionDeathComponent();

protected:

	virtual void BeginPlay() override;
public:
	UFUNCTION(BlueprintCallable)
	AGP_MinionDeath* BeginDeath(TSubclassOf<AGP_MinionDeath> DeathType);

	UFUNCTION()
	void EndDeath();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AGP_MinionDeath* CurrentDeath;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

};
