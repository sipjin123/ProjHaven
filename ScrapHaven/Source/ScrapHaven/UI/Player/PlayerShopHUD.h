// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/FStoreItem.h"
#include "PlayerShopHUD.generated.h"

class UHorizontalBox;
class UUniformGridPanel;
class UUserWidget;
class AShopItem;
class ASupplyBox;
class UInventorySlotWidget;

USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY()

	/** Single item reference (can be nullptr) */
	UPROPERTY()
	FStoreItem SingleItem;

	/** Abstract representation of a stackable item */
	UPROPERTY()
	FStoreItem SupplyBoxItem;

	/** Quantity of the supply box or stack */
	UPROPERTY()
	int32 Quantity = 0;

	/** Is this slot empty? */
	bool IsEmpty() const { return Quantity <= 0 && SingleItem.ItemName.IsEmpty(); }
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventorySlotUpdated, const FInventorySlot&, SlotData);

UCLASS()
class SCRAPHAVEN_API UPlayerShopHUD : public UUserWidget
{
	GENERATED_BODY()
public:
	
	/** Triggered whenever a slot is updated (box or single item added/removed) */
	UPROPERTY(BlueprintAssignable, Category="Inventory")
	FOnInventorySlotUpdated OnInventorySlotUpdated;
	
	void NativeConstruct();
	/** Build the inventory UI */
	UFUNCTION(BlueprintCallable, Category="UI")
	void BuildInventoryUI();

	/** Add a ShopItem actor to inventory */
	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool AddItem(AShopItem* Item);

	/** Add a SupplyBox actor to inventory */
	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool AddBox(ASupplyBox* Box);

	/** Select a hotbar slot by index (0–4) */
	UFUNCTION(BlueprintCallable, Category="UI")
	void SelectHotbarSlot(int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool HandleUseCurrentSlot(FStoreItem& OutItem);

	UFUNCTION(BlueprintCallable, Category="UI")
	void RefreshHotbar();
protected:
	/** Hotbar (5 slots) */
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UHorizontalBox* UserItemsBox;

	/** Grid inventory (3x3) */
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UUniformGridPanel* InventoryGrid;

	/** Widget class used for items/boxes */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> ItemWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> InventoryWidgetClass;

	/** Internal representation of inventory */
	UPROPERTY()
	TArray<FInventorySlot> InventorySlots;
	
	/** Current hotbar selection */
	UPROPERTY(BlueprintReadOnly, Category="UI")
	int32 SelectedHotbarIndex = INDEX_NONE;

	/** References to the hotbar slot widgets */
	UPROPERTY()
	TArray<UInventorySlotWidget*> HotbarWidgets;
};