// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/InventorySlotWidget.h"
#include "Shopping//ShopItem.h"
#include "Shopping/SupplyBox.h"

void UInventorySlotWidget::InitSlot(AShopItem* SingleItem, ASupplyBox* SupplyBox, int32 Quantity)
{
	// Cache data for later use
	RepresentedItem = SingleItem;
	RepresentedBox = SupplyBox;
	RepresentedQuantity = Quantity;

	// Forward to Blueprint so you can update visuals
	SetupSlot(SingleItem, SupplyBox, Quantity);
}
