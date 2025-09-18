// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/CustomerCharacter.h"

// Sets default values
ACustomerCharacter::ACustomerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACustomerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACustomerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACustomerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACustomerCharacter::AddToShoppingBasket(FName ItemName, int32 Quantity)
{
	if (ItemName.IsNone() || Quantity <= 0)
	{
		return;
	}

	// Look for existing entry
	for (FItemPurchasePair& Pair : ShoppingBasket)
	{
		if (Pair.ItemName == ItemName)
		{
			// If item already exists, increase quantity
			Pair.Quantity += Quantity;
			return;
		}
	}

	UE_LOG(LogTemp, Verbose, TEXT("Added New ITem to Basket =%s"), *ItemName.ToString());
	// Otherwise, add a new entry
	FItemPurchasePair NewPair;
	NewPair.ItemName = ItemName;
	NewPair.Quantity = Quantity;
	ShoppingBasket.Add(NewPair);
}

bool ACustomerCharacter::ShouldWanderToOtherShelf(const FDailyBehaviorProfile& Profile, float BaseWanderChance = 0.2f)
{
	float WanderChance = BaseWanderChance;

	// Influence from Needs
	float Hungry  = Profile.GetNeedIntensity(ENPCNeedType::Hungry);
	float Thirsty = Profile.GetNeedIntensity(ENPCNeedType::Thirsty);
	float Tired   = Profile.GetNeedIntensity(ENPCNeedType::Tired);

	// Needs influence — adjust weights as you like
	WanderChance += Hungry  * 0.15f;  // hungry: wander for food
	WanderChance += Thirsty * 0.20f;  // thirsty: wander for drinks
	WanderChance -= Tired   * 0.25f;  // tired: less likely to wander

	// Influence from Mood
	switch (Profile.Mood)
	{
	case ENPCMoodType::Cheerful:
		WanderChance += 0.15f; // happy → more likely to explore
		break;
	case ENPCMoodType::Irritable:
		WanderChance -= 0.2f; // grumpy → less likely to explore
		break;
	case ENPCMoodType::Dissociate:
		WanderChance -= 0.3f; // withdrawn → avoids wandering
		break;
	case ENPCMoodType::Curious:
		WanderChance += 0.35f; // explicitly curious → strong wander bias
		break;
	default:
		break;
	}

	// Clamp 0–1
	WanderChance = FMath::Clamp(WanderChance, 0.f, 1.f);

	float Roll = FMath::FRand();

	UE_LOG(LogTemp, Verbose, TEXT("NPC Wander? Roll=%.2f vs Chance=%.2f | Mood=%d"), Roll, WanderChance, (int32)Profile.Mood);

	return Roll < WanderChance;
}

void ACustomerCharacter::OnWanderBuyItem(const FName& ItemName)
{
	if (!CanWanderBuy())
	{
		UE_LOG(LogTemp, Log, TEXT("%s cannot wander buy anymore."), *GetName());
		return;
	}

	// Add to basket
	//ShoppingBasket.Add({ ItemName, 1 });

	// Count usage
	WanderBuysSoFar++;

	UE_LOG(LogTemp, Log, TEXT("%s wander bought %s (%d/%d)"), 
		*GetName(), *ItemName.ToString(), WanderBuysSoFar, MaxWanderBuys);
}