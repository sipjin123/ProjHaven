// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/FStoreItem.h"
#include "Engine/DataTable.h"
#include "Shopping/ShelfSector.h"
#include "Shopping/StorageShelf.h"
#include "Shopping/SupplyBox.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "StoreSubsystem.generated.h"

USTRUCT(BlueprintType)
struct FShelfList
{
	GENERATED_BODY()

	// Use TObjectPtr for reflected UObject references (UE5+).
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<AShelfSector>> Shelves;
};
USTRUCT(BlueprintType)
struct FDeliveryOrder
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName ItemName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Quantity = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 DeliveryDay = 0; // in-game day number when it should arrive
};
/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class SCRAPHAVEN_API UStoreSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// Initialize when the subsystem is created
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// Data table reference (assign in editor via defaults, or load from path)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Store")
	UDataTable* ItemDataTable;

	// Cached items
	UPROPERTY(BlueprintReadOnly, Category="Store")
	TArray<FStoreItem> AllItems;

	// Lookup by Name
	UFUNCTION(BlueprintCallable, Category="Store")
	FStoreItem GetItemByName(FName RowName) const;
	void RegisterShelf(FName ItemName, AShelfSector* Shelf);
	
	UFUNCTION(BlueprintCallable, Category="Store")
	AShelfSector* FindShelfWithItem(FName ItemName);

	UFUNCTION(BlueprintCallable, Category="Store")
	void LogAllShelves() const;

	UFUNCTION(BlueprintCallable, Category="Store")
	void UnlockItem(FName ItemName);

	UFUNCTION(BlueprintCallable, Category="Store")
	void LockItem(FName ItemName);

	UFUNCTION(BlueprintCallable, Category="Store")
	bool IsItemUnlocked(FName ItemName) const;

	// Map: ItemName -> list of shelves that can serve it
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Store")
	TMap<FName, FShelfList> ItemToShelves;
	
	UFUNCTION(BlueprintCallable, Category="Store")
	bool AddItemQuantity(FName ItemName, int32 Quantity)
	{
		for (FStoreItem& Item : AllItems)
		{
			if (Item.ItemName == ItemName)
			{
				Item.Quantity += Quantity;
				return true;
			}
		}
		return false;
	}
	// Maximum slots in store inventory
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Store")
	int32 MaxInventorySlots = 10;

	// Items currently in stock
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Store")
	TArray<FStoreItem> CurrentWarehouseInventory;

	// Orders waiting for delivery
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Store")
	TArray<FDeliveryOrder> PendingOrders;
	
	// Helper: total slots used = inventory + pending
	UFUNCTION(BlueprintCallable, Category="Store")
	int32 GetUsedInventorySlots() const
	{
		int32 Total = 0;

		for (const FStoreItem& Item : CurrentWarehouseInventory)
		{
			Total += Item.Quantity;
		}

		for (const FDeliveryOrder& Order : PendingOrders)
		{
			Total += Order.Quantity;
		}

		return Total;
	};
	UFUNCTION(BlueprintCallable, Category="Store")
	bool PlaceOrder(FName ItemName, int32 Quantity, int32 CurrentDay);
	
	UFUNCTION(BlueprintCallable, Category="Store")
	void ProcessDeliveries(int32 CurrentDay);
	
	UFUNCTION(BlueprintCallable, Category="Store")
	void AddToWarehouseInventory(FName ItemName, int32 Quantity);

	UFUNCTION(BlueprintCallable, Category="Store|Debug")
	void LogStoreStatus() const;

	UFUNCTION(BlueprintCallable, Category="Store")
	void AddToInventory(const FName& ItemName, int32 Quantity);

	UFUNCTION(BlueprintCallable, Category="Store")
	void RemoveFromInventory(const FName& ItemName, int32 Quantity);
	
	// Class reference to the box actor that shelves should spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="World")
	TSubclassOf<ASupplyBox> ItemBoxClass;

	// Convenience spawn function
	ASupplyBox* SpawnItemBox(UWorld* World, const FStoreItem& ItemData, const FTransform& Transform);
	// Registered warehouse shelf
	
	UPROPERTY(BlueprintReadOnly, Category="Warehouse")
	TWeakObjectPtr<class AStorageShelf> WarehouseShelf;
	
	UFUNCTION(BlueprintCallable, Category="Store")
	void RegisterShelfActor(AStorageShelf* Shelf) { WarehouseShelf = Shelf; }
};