// I like sand

#pragma once

#include "CoreMinimal.h"

#include "Models/BeachWork.h"
#include "UObject/Object.h"
#include "AITrash_Observer.generated.h"

class AAI_Trash;

UCLASS(Blueprintable)
class TEAM9ASSEMBLE_API UAITrash_Observer : public UObject {
	GENERATED_BODY()

	public:

	void AddTrash(AAI_Trash* Trash);
	void RemoveTrash(AAI_Trash* Trash);
	UBeachWork* GetWork();
	bool HasWork() const;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int TotalDecoration;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<AAI_Trash*> TrashObjects;

};
