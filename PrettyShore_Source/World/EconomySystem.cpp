#include "EconomySystem.h"

#include <string>

#include "BeachGameState.h"
#include "World_Time.h"
#include "Time_Economy_Data.h"
#include "Team9Assemble/AI/AIBeach_Observer.h"
#include "Team9Assemble/Buildings/BuildingBase.h"
#include "Team9Assemble/Buildings/Building_Observer.h"


AEconomySystem::AEconomySystem()
{
	UE_LOG(LogTemp, Log, TEXT("EconomySystem has been constructed."))
}

void AEconomySystem::BeginPlay()
{
	UE_LOG(LogTemp, Log, TEXT("EconomySystem: BeginPlay Start! <---"))
	
	Super::BeginPlay();

	GameState = Cast<ABeachGameState>(GetWorld()->GetGameState());
	
	if ( GameState == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("EconomySystem: GameState was a nullptr"))
	}
	else
	{
		Data = GameState->TimeEconomyData;
		PlayerSalary = Data->PlayerSalary;
		PlayerBankTotal = Data->PlayerStartSum;
		PlayerBankDayBefore = PlayerBankTotal;
	
		GameState->SetEconomySystem(this);

		// UE_LOG(LogTemp, Log, TEXT("EconomySystem: BeginPlay End! <---"))
		// UE_LOG(LogTemp, Log, TEXT("Money balance: %i"), PlayerBankTotal);
	}
	
}

void AEconomySystem::EndOfDayUpdate()
{
	UE_LOG(LogTemp, Log, TEXT("EconomySystem: Sending EndOfDay Update..."))
	
	OnEndOfDayUpdate.Broadcast(this);

	// Calculate the bank total.
	int Income = 0;
	int Expenditure = 0;


	Income += PlayerSalary;

	const int LifeguardSalary = GetLifeguardSalary();
	
	Expenditure += LifeguardSalary;
	Expenditure += UpkeepAmount;

	TotalIncome += Income;
	TotalExpenditure += Expenditure;
	PlayerBankTotal += Income - Expenditure;
	
	// Calculate the difference in total bank from last day.
	PlayerBankDifferenceFromDayBefore = PlayerBankTotal - PlayerBankDayBefore;
	LatestDiff = PlayerBankDifferenceFromDayBefore;

	// Print some useful info to log.
	// UE_LOG(LogTemp, Log, TEXT("----- EconomySystem EndOfDay Summary -----"))
	// UE_LOG(LogTemp, Log, TEXT("EconomySystem: Bank Today: %i"), PlayerBankTotal)
	// UE_LOG(LogTemp, Log, TEXT("EconomySystem: Bank Yesterday: %i"), PlayerBankDayBefore)
	// UE_LOG(LogTemp, Log, TEXT("EconomySystem: Bank Difference: %i"), PlayerBankDifferenceFromDayBefore)

	GameState->SendLogMessage(TEXT("End of day log for day " + FString::FromInt(GameState->WorldTime->AmountOfDaysSinceStartOfGame)), FColor::White, this);
	
	GameState->SendLogMessage(TEXT("Upkeep cost for buildings: " + FString::FromInt(UpkeepAmount)), FColor::Red, this);
	
	GameState->SendLogMessage(TEXT("Salary for Lifeguards: " + FString::FromInt(LifeguardSalary)), FColor::Red, this);
	
	const FColor MessageColor = PlayerBankDifferenceFromDayBefore >= 0 ? FColor::Green : FColor::Red;
	GameState->SendLogMessage(TEXT("Total difference since yesterday: " + FString::FromInt(PlayerBankDifferenceFromDayBefore)), MessageColor, this);

	UpdateUI(true);
	
	UpkeepAmount = 0;
	PlayerBankDayBefore = PlayerBankTotal;
}

void AEconomySystem::AddToIncome(const int32 Amount, AActor* Sender)
{
	TotalIncome += Amount;
	PlayerBankTotal += Amount;
	LatestDiff = Amount;

	//auto* Building = Cast<ABuildingBase>(Sender);
	if (Cast<ABuildingBase>(Sender) && Amount != 0)
	{
		GameState->SendLogMessage(TEXT("has generated income: $" + FString::FromInt(Amount)), FColor::Green, Sender);
	}
	
	UpdateUI(false);
}

void AEconomySystem::AddToExpenses(const int32 Amount, AActor* Sender)
{
	TotalExpenditure += Amount;
	PlayerBankTotal -= Amount;
	LatestDiff = -Amount;

	//auto* Building = Cast<ABuildingBase>(Sender);
	if (Cast<ABuildingBase>(Sender))
	{
		GameState->SendLogMessage(TEXT("has added to expenses: $" + FString::FromInt(Amount)), FColor::Red, Sender);
	}
	else if(Cast<AAILifeGuard_Controller>(Sender))
	{
		GameState->SendLogMessage(TEXT("cost to hire: $" + FString::FromInt(Amount)), FColor::Red, Sender);
	}
	
	UpdateUI(false);
}

void AEconomySystem::AddToUpkeep(const int32 Amount)
{
	UpkeepAmount += Amount;
}

void AEconomySystem::CompleteSetup()
{
	GameState->WorldTime->EndOfDayPing.AddDynamic(this, &AEconomySystem::EndOfDayUpdate);

	// UE_LOG(LogTemp, Log, TEXT("EconomySystem has completed its Setup! <---"))
}

void AEconomySystem::UpdateUI(const bool bIsEndOfDay)
{
	// Send the PlayerBankTotal to the ui.
	// UE_LOG(LogTemp, Log, TEXT("EconomySystem just updated the UI."))
	OnUiUpdate.Broadcast(this, bIsEndOfDay);
}

int AEconomySystem::GetLifeguardSalary()
{
	int TotalSalary{0};
	auto Lifeguards = GameState->AIObserver->Lifeguards;
	if (Lifeguards.Num() != 0)
	{
		for (auto* Guard : Lifeguards)
		{
			
			TotalSalary += Guard->Info.Salary;

			// UE_LOG(LogTemp, Log, TEXT("A Lifeguard took: %i in salary"), Guard->Info.Salary);
		}
	}
	
	return TotalSalary;
}

int AEconomySystem::GetUpKeepCost()
{
	int TotalUpkeep{0};
	TotalUpkeep += GetLifeguardSalary();
	UE_LOG(LogTemp, Log, TEXT("GetUpKeepCost()"));
	auto Buildings = GameState->BuildingObserver->Buildings;
	if (Buildings.Num() != 0)
	{
		for (auto* Building : Buildings)
		{
			TotalUpkeep += Building->BuildingData->Inventory.UpkeepCost;
		}
	}
	
	return TotalUpkeep;
}
