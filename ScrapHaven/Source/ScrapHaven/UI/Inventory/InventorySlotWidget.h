#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotWidget.generated.h"

class AShopItem;
class ASupplyBox;

/**
 * Widget representing a single inventory slot
 */
UCLASS()
class SCRAPHAVEN_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Setup this slot’s visuals */
	UFUNCTION(BlueprintImplementableEvent, Category="UI")
	void SetupSlot(AShopItem* SingleItem, ASupplyBox* SupplyBox, int32 Quantity);

	/** Store references so Blueprint can also query later if needed */
	UPROPERTY(BlueprintReadOnly, Category="Inventory")
	AShopItem* RepresentedItem = nullptr;

	UPROPERTY(BlueprintReadOnly, Category="Inventory")
	ASupplyBox* RepresentedBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category="Inventory")
	int32 RepresentedQuantity = 0;

	/** Helper you can call to initialize and cache values */
	UFUNCTION(BlueprintCallable, Category="UI")
	void InitSlot(AShopItem* SingleItem, ASupplyBox* SupplyBox, int32 Quantity);
	
	/** Highlight the slot (called when selected) */
	UFUNCTION(BlueprintImplementableEvent, Category="UI")
	void SetHighlighted(bool bHighlighted);
	
	UFUNCTION(BlueprintImplementableEvent, Category="UI")
	void UpdateSlot(FStoreItem SingleItem, FStoreItem BoxItem, int32 Quantity);

};
