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

	// Otherwise, add a new entry
	FItemPurchasePair NewPair;
	NewPair.ItemName = ItemName;
	NewPair.Quantity = Quantity;
	ShoppingBasket.Add(NewPair);
}
