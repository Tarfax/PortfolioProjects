// GP4 -> GP4 -> EGP_MinionCommand.h
// 2021 © Vilhelm Stokstad

#pragma once

UENUM(BlueprintType)
enum class EGP_MinionCommand : uint8{
	None = 0,
	Idle = 1,
	UnPossess = 2,
	Possess = 3,
	Explode = 4,
	MoveTo = 5
};
