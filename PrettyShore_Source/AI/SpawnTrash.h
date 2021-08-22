// I like sand

#pragma once

#include "CoreMinimal.h"


#include "Components/ActorComponent.h"
#include "SpawnTrash.generated.h"
class UAITrash_Observer;
class AActor;
class AAI_Trash;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAM9ASSEMBLE_API USpawnTrash : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpawnTrash();
	void BeginPlay() override;
	
	// UPROPERTY(VisibleAnywhere)
	// TArray<AActor*> VisitorTrash;

	// UPROPERTY(VisibleAnywhere)
	// UAITrash_Observer* TrashObserver;
	
	UFUNCTION(BlueprintCallable, Category = "AI Trash")
	void SpawnTrash(TSubclassOf<AAI_Trash> TrashObject);
};
