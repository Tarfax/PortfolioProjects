// I like sand


#include "VisionSensing.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Team9Assemble/AI/AILifeguard/AILifeGuard_Controller.h"
#include "Team9Assemble/AI/AI_BeachCharacter.h"


UVisionSensing::UVisionSensing() {
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UVisionSensing::BeginPlay() {
	Super::BeginPlay();
}


// Called every frame
void UVisionSensing::TickComponent(float DeltaTime, ELevelTick TickType,
                                   FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	if (DebugVision == true && AILifeGuard != nullptr) {
		const FVector Direction = FVector::ForwardVector;
		const FVector Origin = AILifeGuard->AI_BeachCharacter->GetActorLocation();

		const FVector Right = Direction.RotateAngleAxis(VisionAngle, FVector::UpVector);
		const FVector Left = Direction.RotateAngleAxis(-VisionAngle, FVector::UpVector);

		UKismetSystemLibrary::DrawDebugLine(GetWorld(), Origin, Origin + Right* VisionLength, FLinearColor::Yellow, 0, 5);
		UKismetSystemLibrary::DrawDebugLine(GetWorld(), Origin, Origin + Left * VisionLength, FLinearColor::Yellow, 0, 5);
		UKismetSystemLibrary::DrawDebugLine(GetWorld(), Origin + Right * VisionLength, Origin + Left * VisionLength, FLinearColor::Yellow, 0, 5);
	}

	
	
}

float UVisionSensing::GetVisionInRadians() const {
	return FMath::Cos(FMath::DegreesToRadians(VisionAngle));
}

bool UVisionSensing::IsAIVisitorVisible(FVector& PointToTest, FVector& Origin, FVector& Direction, float DistanceMinimum) const {
	const float DistanceMinimumSquared = FMath::Square(DistanceMinimum);

	if (FVector::DistSquared(Origin, PointToTest) > DistanceMinimumSquared) {
		UE_LOG(LogTemp, Log, TEXT("UVisionSensing::IsAIVisitorVisible: DistSquared:, %f"), FVector::DistSquared(Origin, PointToTest));
		UE_LOG(LogTemp, Log, TEXT("UVisionSensing::IsAIVisitorVisible: DistanceMinimumSquared, %f"), DistanceMinimumSquared);
		UE_LOG(LogTemp, Log, TEXT("UVisionSensing::IsAIVisitorVisible: return false"));
		return false;
	}

	const FVector DirectionToTarget = (PointToTest - Origin).GetSafeNormal();

	const float AsHalfRadians = FMath::Cos(FMath::DegreesToRadians(VisionAngle));
	const float Dot = FVector::DotProduct(Direction, DirectionToTarget);

	const bool IsValid = Dot > AsHalfRadians;
	return IsValid;
	
}
