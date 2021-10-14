// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
// #include "CoreMinimal.h"
#include "GP4.generated.h"

#define LOG(string) UE_LOG(LogTemp, Log, TEXT(string))

USTRUCT(BlueprintType, Blueprintable)
struct F_Vector2 {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Y;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Z;
};

USTRUCT(BlueprintType, Blueprintable)
struct F_LevelStreamBounds {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	F_Vector2 TopLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	F_Vector2 BottomRight;
};
