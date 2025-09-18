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

FShelfList UStoreSubsystem::GetRandomShelfList() const
{
	if (ItemToShelves.Num() == 0)
	{
		return FShelfList();
	}

	// Collect valid keys (those with at least one shelf that has stock)
	TArray<FName> ValidKeys;
	for (const TPair<FName, FShelfList>& Pair : ItemToShelves)
	{
		for (AShelfSector* Shelf : Pair.Value.Shelves)
		{
			if (Shelf && Shelf->ShelfManager->TotalItems > 0) // assumes HasStock() exists in AShelfSector
			{
				ValidKeys.Add(Pair.Key);
				break; // one shelf with stock is enough to consider this key valid
			}
		}
	}

	if (ValidKeys.Num() == 0)
	{
		return FShelfList();
	}

	// Pick random key
	int32 RandIndex = FMath::RandRange(0, ValidKeys.Num() - 1);
	const FName& RandomKey = ValidKeys[RandIndex];

	// Guaranteed to exist
	return *ItemToShelves.Find(RandomKey);
}

AShelfSector* UStoreSubsystem::GetRandomShelfWithItem() const
{
	if (ItemToShelves.Num() == 0)
	{
		return nullptr;
	}

	// Collect all shelves that have stock
	TArray<AShelfSector*> ValidShelves;
	for (const TPair<FName, FShelfList>& Pair : ItemToShelves)
	{
		for (AShelfSector* Shelf : Pair.Value.Shelves)
		{
			if (Shelf && Shelf->ShelfManager->TotalItems > 0) // your shelf must implement this
			{
				ValidShelves.Add(Shelf);
			}
		}
	}

	if (ValidShelves.Num() == 0)
	{
		return nullptr;
	}

	// Pick one at random
	int32 RandIndex = FMath::RandRange(0, ValidShelves.Num() - 1);
	return ValidShelves[RandIndex];
}

bool UStoreSubsystem::PlaceOrder(FName ItemName, int32 Quantity, int32 CurrentDay)
{
	if (ItemName.IsNone() || Quantity <= 0)
	{
		return false;
	}

	int32 UsedSlots = GetUsedInventorySlots();
	int32 FreeSlots = MaxInventorySlots - UsedSlots;

	if (FreeSlots <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Order failed: no space left in inventory."));
		return false;
	}

	// Clamp order to free space
	int32 FinalQuantity = FMath::Min(Quantity, FreeSlots);

	if (FinalQuantity <= 0)
	{
		return false;
	}

	FDeliveryOrder Order;
	Order.ItemName = ItemName;
	Order.Quantity = FinalQuantity;
	Order.DeliveryDay = CurrentDay + 1;
	PendingOrders.Add(Order);

	UE_LOG(LogTemp, Log, TEXT("Placed order: %d of %s (slots left: %d)"),
		FinalQuantity, *ItemName.ToString(), FreeSlots - FinalQuantity);

	return true;
}

void UStoreSubsystem::ProcessDeliveries(int32 CurrentDay)
{
	for (int32 i = PendingOrders.Num() - 1; i >= 0; --i)
	{
		if (PendingOrders[i].DeliveryDay == CurrentDay)
		{
			AddToWarehouseInventory(PendingOrders[i].ItemName, PendingOrders[i].Quantity);
			PendingOrders.RemoveAt(i);
		}
	}
}

void UStoreSubsystem::AddToWarehouseInventory(FName ItemName, int32 Quantity)
{
	if (ItemName.IsNone() || Quantity <= 0) return;

	for (int32 i = 0; i < Quantity; i++)
	{
		// Create a new FStoreItem for **each unit**
		FStoreItem NewItem;
		NewItem.ItemName = ItemName;

		// Add to warehouse inventory array
		CurrentWarehouseInventory.Add(NewItem);

		// Add to shelf slot (spawns physical box)
		bool bAdded = WarehouseShelf->AddItemToSlot(ItemName);
		if (!bAdded)
		{
			UE_LOG(LogTemp, Warning, TEXT("No free slot to add item %s"), *ItemName.ToString());
			break; // stop if shelf is full
		}

		UE_LOG(LogTemp, Log, TEXT("Added item to warehouse: %s"), *ItemName.ToString());
	}
}

void UStoreSubsystem::LogStoreStatus() const
{
	UE_LOG(LogTemp, Log, TEXT("====== STORE STATUS ======"));

	// Log inventory
	UE_LOG(LogTemp, Log, TEXT("Current Inventory:"));
	if (CurrentWarehouseInventory.Num() == 0)
	{
		UE_LOG(LogTemp, Log, TEXT("  [Empty]"));
	}
	else
	{
		for (const FStoreItem& Item : CurrentWarehouseInventory)
		{
			UE_LOG(LogTemp, Log, TEXT("  Item: %s | Quantity: %d"),
				*Item.ItemName.ToString(),
				Item.Quantity);
		}
	}

	// Log pending orders
	UE_LOG(LogTemp, Log, TEXT("Pending Orders:"));
	if (PendingOrders.Num() == 0)
	{
		UE_LOG(LogTemp, Log, TEXT("  [None]"));
	}
	else
	{
		for (const FDeliveryOrder& Order : PendingOrders)
		{
			UE_LOG(LogTemp, Log, TEXT("  Item: %s | Quantity: %d | Delivery Day: %d"),
				*Order.ItemName.ToString(),
				Order.Quantity,
				Order.DeliveryDay);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("=========================="));
}
void UStoreSubsystem::AddToInventory(const FName& ItemName, int32 Quantity)
{
	if (ItemName.IsNone() || Quantity <= 0) return;

	for (FStoreItem& Item : CurrentWarehouseInventory)
	{
		if (Item.ItemName == ItemName)
		{
			Item.Quantity += Quantity;
			return;
		}
	}

	// If not found, add new entry
	FStoreItem NewItem;
	NewItem.ItemName = ItemName;
	NewItem.Quantity = Quantity;
	CurrentWarehouseInventory.Add(NewItem);
}

void UStoreSubsystem::RemoveFromInventory(const FName& ItemName, int32 Quantity)
{
	if (ItemName.IsNone() || Quantity <= 0) return;

	for (int32 i = 0; i < CurrentWarehouseInventory.Num(); i++)
	{
		if (CurrentWarehouseInventory[i].ItemName == ItemName)
		{
			CurrentWarehouseInventory[i].Quantity -= Quantity;
			if (CurrentWarehouseInventory[i].Quantity <= 0)
			{
				CurrentWarehouseInventory.RemoveAt(i);
			}
			return;
		}
	}
}

ASupplyBox* UStoreSubsystem::SpawnItemBox(UWorld* World, const FStoreItem& ItemData, const FTransform& Transform)
{
	
	if (!World || !ItemBoxClass) return nullptr;
	UE_LOG(LogTemp, Log, TEXT("Try to spawn box order: %s"), *ItemData.ItemName.ToString());

	FActorSpawnParameters SpawnParams;
	FTransform SpawnTransform = Transform;

	// Deferred spawn
	ASupplyBox* Box = World->SpawnActorDeferred<ASupplyBox>(ItemBoxClass, SpawnTransform);
	if (Box)
	{
		// Set properties exposed on spawn
		Box->ItemRowName = ItemData.ItemName;
		Box->BoxTypeRowName = "Regular"; // optional
		Box->CachedItem = ItemData;

		// Finish spawn so it calls BeginPlay, etc.
		Box->FinishSpawning(SpawnTransform);
	}
	
	return Box;
}
