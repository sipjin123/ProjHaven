// Fill out your copyright notice in the Description page of Project Settings.


#include "Shopping/ShopItem.h"

#include "Subsystems/StoreSubsystem.h"

// Sets default values
AShopItem::AShopItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AShopItem::BeginPlay()
{
	Super::BeginPlay();

	if (UStoreSubsystem* StoreSubsystem = GetGameInstance()->GetSubsystem<UStoreSubsystem>())
	{
		if (StoreSubsystem->ItemDataTable && StoreItemRow != NAME_None)
		{
			FStoreItem* Found = StoreSubsystem->ItemDataTable->FindRow<FStoreItem>(StoreItemRow, TEXT("AShopItem Lookup"));
			if (Found)
			{
				StoreItem = *Found; // copy into our instance
			}
		}
	}
}

// Called every frame
void AShopItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
