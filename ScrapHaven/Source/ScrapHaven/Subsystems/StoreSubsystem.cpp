// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/StoreSubsystem.h"

#include "Shopping/ShelfSector.h"

void UStoreSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (ItemDataTable)
	{
		TArray<FStoreItem*> Rows;
		ItemDataTable->GetAllRows<FStoreItem>(TEXT("StoreSubsystem Load"), Rows);

		AllItems.Empty();
		for (FStoreItem* Row : Rows)
		{
			if (Row && Row->bUnlocked)
			{
				AllItems.Add(*Row); // copy struct into array
			}
		}
	}
}

FStoreItem UStoreSubsystem::GetItemByName(FName RowName) const
{
	if (ItemDataTable)
	{
		if (const FStoreItem* Row = ItemDataTable->FindRow<FStoreItem>(RowName, TEXT("StoreSubsystem Lookup")))
		{
			return *Row; // return a copy
		}
	}
	return FStoreItem(); // default
}

void UStoreSubsystem::RegisterShelf(FName ItemName, AShelfSector* Shelf)
{
	UE_LOG(LogTemp, Error, TEXT("❌ Try Register SHELF NOW===================================!"));

	if (!Shelf) return;

	// Remove this shelf from all current entries first
	for (auto& Pair : ItemToShelves)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ Remove SHELF NOW===================================!"));
		Pair.Value.Shelves.Remove(Shelf);
	}

	// Check the shelf’s current item
	if (Shelf->StoreItemRefData.ItemName.IsNone())
	{
		// Shelf is empty — don’t re-add it
		return;
	}

	// Add this shelf under the correct item entry
	FShelfList& List = ItemToShelves.FindOrAdd(Shelf->StoreItemRefData.ItemName);
	if (!List.Shelves.Contains(Shelf))
	{
		
		UE_LOG(LogTemp, Error, TEXT("❌ DONE SHELF NOW===================================!"));
		List.Shelves.Add(Shelf);
	}
}

AShelfSector* UStoreSubsystem::FindShelfWithItem(FName ItemName)
{
	if (const FShelfList* List = ItemToShelves.Find(ItemName))
	{
		const TArray<TObjectPtr<AShelfSector>>& Shelves = List->Shelves;
		if (Shelves.Num() > 0)
		{
			return Shelves[FMath::RandHelper(Shelves.Num())];
		}
	}
	return nullptr;
}

void UStoreSubsystem::LogAllShelves() const
{
	UE_LOG(LogTemp, Log, TEXT("==== Shelf Debug Log ===="));

	for (const TPair<FName, FShelfList>& Pair : ItemToShelves)
	{
		const FName& ItemName = Pair.Key;
		const FShelfList& ShelfList = Pair.Value;

		for (AShelfSector* Shelf : ShelfList.Shelves)
		{
			if (!Shelf) continue;

			UE_LOG(LogTemp, Log, TEXT("Shelf: %s (Item: %s)"), 
				*Shelf->GetName(), *ItemName.ToString());

			UE_LOG(LogTemp, Log, TEXT("   - Item: %s | Qty: %d"), 
				*Shelf->StoreItemRefData.ItemName.ToString(), Shelf->StoreItemRefData.Quantity);

			// Optional: also print to screen for quick dev check
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, 
					FString::Printf(TEXT("%s has %d items"), 
						*Shelf->GetName(), Shelf->StoreItemRefData.Quantity));
			}
		}
	}

	UE_LOG(LogTemp, Log, TEXT("==== End Shelf Log ===="));
}

void UStoreSubsystem::UnlockItem(FName ItemName)
{
	if (FStoreItem* Row = ItemDataTable->FindRow<FStoreItem>(ItemName, TEXT("UnlockItem")))
	{
		Row->bUnlocked = true;   // mark as unlocked
		UE_LOG(LogTemp, Log, TEXT("Unlocked item: %s"), *ItemName.ToString());
	}
}

void UStoreSubsystem::LockItem(FName ItemName)
{
	if (FStoreItem* Row = ItemDataTable->FindRow<FStoreItem>(ItemName, TEXT("LockItem")))
	{
		Row->bUnlocked = false;  // mark as locked
		UE_LOG(LogTemp, Log, TEXT("Locked item: %s"), *ItemName.ToString());
	}
}

bool UStoreSubsystem::IsItemUnlocked(FName ItemName) const
{
	if (const FStoreItem* Row = ItemDataTable->FindRow<FStoreItem>(ItemName, TEXT("IsItemUnlocked")))
	{
		return Row->bUnlocked;
	}
	return false;
}