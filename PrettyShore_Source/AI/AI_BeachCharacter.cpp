// I like sand


#include "AI_BeachCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Models/AI_WaterArea.h"
#include "Team9Assemble/ClickableObject.h"


AAI_BeachCharacter::AAI_BeachCharacter() {


	ClickableComp = CreateDefaultSubobject<UClickableObject>(TEXT("ClickableObject"));

}

void AAI_BeachCharacter::BeginPlay() {
	Super::BeginPlay();
	MovementComponent = GetCharacterMovement();
	
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AAI_BeachCharacter::HandleBeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AAI_BeachCharacter::HandleEndOverlap);
}

void AAI_BeachCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	// if (bIsInWater == true && GetCapsuleComponent()->GetComponentLocation().Z-90.0f > 0) {
	// 	OnSetInWater(false);
	// 	bIsInWater = false;
	// }
}

void AAI_BeachCharacter::HandleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                            UPrimitiveComponent* OtherComp, int32 BodyIndex, bool bFromSweep,
                                            const FHitResult& SweepHit) {

		// OnSetInWater(true);
		// bIsInWater = true;
}

void AAI_BeachCharacter::HandleEndOverlap(UPrimitiveComponent* OnComponentEndOverlap, AActor* OtherActor,
                                          UPrimitiveComponent* OverlappedComponent, int32 OtherBodyIndex) {
		// OnSetInWater(false);
		// bIsInWater = false;
}
