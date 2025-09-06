// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/FStoreItem.h"
#include "Engine/DataTable.h"
#include "Shopping/ShelfSector.h"
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

};