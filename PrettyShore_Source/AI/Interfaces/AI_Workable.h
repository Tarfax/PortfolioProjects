// I like sand

#pragma once

#include "UObject/Interface.h"
#include "AI_Workable.generated.h"

class UBeachWork;

// This class does not need to be modified.
UINTERFACE()
class UAI_Workable : public UInterface {
	GENERATED_BODY()
};

/**
 * 
 */
class TEAM9ASSEMBLE_API IAI_Workable {
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void FinishWork(UBeachWork* BeachWork);

};