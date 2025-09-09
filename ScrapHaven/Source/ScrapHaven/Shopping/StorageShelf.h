// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/FStoreItem.h"
#include "StorageShelf.generated.h"

UCLASS()
class SCRAPHAVEN_API AStorageShelf : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStorageShelf();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category="Store|Debug")
	bool AddItemToSlot(const FName& ItemName);
	UFUNCTION(BlueprintCallable, Category="Store|Debug")
	bool RemoveItemFromSlot(int32 SlotIndex);
	
	// Scene root
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Shelf")
	USceneComponent* Root;
	
	/** Root under which you add child SceneComponents that act as item slots */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shelf")
	USceneComponent* SlotsRoot;
	
	// List of placement nodes (slots for items)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Shelf")
	TArray<USceneComponent*> ItemSlots;
	
	// Which items are placed in which slot
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Shelf")
	TArray<FStoreItem> PlacedItems;
	
	// Each entry = one box/item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Store")
	TArray<FStoreItem> ShelfInventory;

	UFUNCTION(BlueprintCallable, Category="Store|Debug")
	void LogShelfContents() const;
};