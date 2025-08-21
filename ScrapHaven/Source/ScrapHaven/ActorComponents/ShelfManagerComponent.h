// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/FStoreItem.h"
#include "ShelfManagerComponent.generated.h"

class UBoxComponent;

USTRUCT(BlueprintType)
struct FItemSlot
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector WorldLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bOccupied = false;
	
	// The actor currently in this slot
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TWeakObjectPtr<AActor> OccupyingItem;
};

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SCRAPHAVEN_API UShelfManagerComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	UBoxComponent* ShelfReference;

	UPROPERTY()
	UBoxComponent* ItemReference;

	UPROPERTY()
	TArray<FItemSlot> Slots;

	int32 ItemsPerRow = 1;
	int32 ItemsPerColumn = 1;
	int32 MaxLayers = 1;
public:	
	UShelfManagerComponent();

	/** Initialize shelf slots based on shelf and item sizes */
	UFUNCTION(BlueprintCallable, Category="Shelf")
	void InitializeShelf(UBoxComponent* ShelfBox, UBoxComponent* ItemBox, AShelfSector* ShelfSector, const FStoreItem& StoreItemRef);

	/** Place an item in the next free slot */
	UFUNCTION(BlueprintCallable, Category="Shelf")
	bool PlaceItemInNextSlot(AActor* ItemActor);

	/** Reset shelf slots */
	UFUNCTION(BlueprintCallable, Category="Shelf")
	void ClearShelf();

	UFUNCTION(BlueprintCallable, Category="Shelf")
	AActor* TakeItem(int32 SlotIndex);
	
	UFUNCTION(BlueprintCallable)
	AActor* TakeLastItem();
	
	// Free a slot (when item is removed)
	UFUNCTION(BlueprintCallable, Category="Shelf")
	void FreeSlot(int32 SlotIndex)
	{
		if (Slots.IsValidIndex(SlotIndex))
		{
			Slots[SlotIndex].bOccupied = false;
		}
	}

	UPROPERTY(BlueprintReadWrite)
	int32 TotalItems = 0;

	UPROPERTY(BlueprintReadWrite)
	float BasePrice = 0;
	
	UPROPERTY(BlueprintReadWrite)
	float AssignedPrice = 0;

	UFUNCTION(BlueprintCallable)
	void AddPriceValue(float PriceIncrement);
};