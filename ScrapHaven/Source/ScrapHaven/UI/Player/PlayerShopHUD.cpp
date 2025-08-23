#include "PlayerShopHUD.h"
#include "Components/HorizontalBox.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Blueprint/UserWidget.h"
#include "Shopping//ShopItem.h"
#include "Shopping/SupplyBox.h"
#include "UI/Inventory/InventorySlotWidget.h"

void UPlayerShopHUD::NativeConstruct()
{
	Super::NativeConstruct();

	// Ensure inventory always has 14 slots: 5 hotbar + 9 grid
	InventorySlots.SetNum(10);

	// Optionally build UI immediately
	BuildInventoryUI();
}

void UPlayerShopHUD::BuildInventoryUI()
{
	if (!InventoryWidgetClass) return;

	// --- Create widgets if they don’t exist yet ---
	if (HotbarWidgets.Num() < 5)
	{
		HotbarWidgets.SetNum(5);
		UserItemsBox->ClearChildren();

		for (int32 i = 0; i < 5; i++)
		{
			UInventorySlotWidget* ItemWidget = CreateWidget<UInventorySlotWidget>(this, InventoryWidgetClass);
			if (ItemWidget)
			{
				UserItemsBox->AddChild(ItemWidget);
				HotbarWidgets[i] = ItemWidget;
			}
		}
	}

	// --- Update each widget ---
	for (int32 i = 0; i < 5; i++)
	{
		if (!InventorySlots.IsValidIndex(i) || !HotbarWidgets[i]) continue;

		FInventorySlot& CurrentSlot = InventorySlots[i];
		UInventorySlotWidget* ItemWidget = HotbarWidgets[i];

		if (CurrentSlot.SingleItem.ItemName.ToString() != "")
		{
			// Single item present
			ItemWidget->UpdateSlot(CurrentSlot.SingleItem, FStoreItem(), 1);
		}
		else if (CurrentSlot.Quantity > 0)
		{
			// Supply box slot
			ItemWidget->UpdateSlot(FStoreItem(), CurrentSlot.SupplyBoxItem, CurrentSlot.Quantity);
		}
		else
		{
			// Empty slot
			ItemWidget->UpdateSlot(FStoreItem(), FStoreItem(), 0);
		}


		// Highlight selected slot
		ItemWidget->SetHighlighted(i == SelectedHotbarIndex);
	}
}


void UPlayerShopHUD::SelectHotbarSlot(int32 SlotIndex)
{
	if (SlotIndex < 0 || SlotIndex >= 5) return;

	SelectedHotbarIndex = SlotIndex;

	// Update all hotbar highlights
	for (int32 i = 0; i < HotbarWidgets.Num(); i++)
	{
		if (HotbarWidgets[i])
		{
			HotbarWidgets[i]->SetHighlighted(i == SelectedHotbarIndex);
		}
	}
}

bool UPlayerShopHUD::AddBox(ASupplyBox* BoxActor)
{
	if (!BoxActor) return false;

	for (FInventorySlot& InvSlot : InventorySlots)
	{
		if (InvSlot.IsEmpty())
		{
			// Copy the cached item from the box into the slot
			InvSlot.SupplyBoxItem = BoxActor->GetItemType(); // FStoreItem
			InvSlot.Quantity      = BoxActor->GetItemCount();

			// Optional: keep a weak pointer if you still need live updates
			//InvSlot.SupplyBoxActor = TWeakObjectPtr<ASupplyBox>(BoxActor);

			// Notify UI (Blueprints can bind to this)
			OnInventorySlotUpdated.Broadcast(InvSlot);

			BuildInventoryUI();
			return true;
		}
	}

	return false;
}

bool UPlayerShopHUD::AddItem(AShopItem* ItemActor)
{
	if (!ItemActor) return false;

	for (FInventorySlot& InvSlot : InventorySlots)
	{
		if (InvSlot.IsEmpty())
		{
			// Copy the item data from the actor
			InvSlot.SingleItem = ItemActor->StoreItem; // FStoreItem
			InvSlot.Quantity       = 1;

			// Optional: keep a weak pointer to the actor for interaction
			//InvSlot.SingleItemActor = TWeakObjectPtr<AShopItem>(ItemActor);

			// Notify UI (Blueprints can bind to this)
			OnInventorySlotUpdated.Broadcast(InvSlot);

			BuildInventoryUI();
			return true;
		}
	}

	return false;
}

void UPlayerShopHUD::RefreshHotbar()
{
	for (int32 i = 0; i < HotbarWidgets.Num(); i++)
	{
		if (!InventorySlots.IsValidIndex(i)) continue;

		FInventorySlot& CurrentSlot = InventorySlots[i]; // rename local variable
		if (UInventorySlotWidget* Widget = HotbarWidgets[i])
		{
			Widget->UpdateSlot(CurrentSlot.SingleItem, CurrentSlot.SupplyBoxItem, CurrentSlot.Quantity);
		}
	}
}

bool UPlayerShopHUD::HandleUseCurrentSlot(FStoreItem& OutItem)
{
	if (!InventorySlots.IsValidIndex(SelectedHotbarIndex))
		return false;

	// Use a different name to avoid hiding any class member
	FInventorySlot& CurrentSlot = InventorySlots[SelectedHotbarIndex];

	if (CurrentSlot.IsEmpty())
		return false;

	// Return the correct item
	if (!CurrentSlot.SingleItem.ItemName.IsEmpty())
	{
		OutItem = CurrentSlot.SingleItem;
		// Single items are always quantity 1, so clear the slot
		CurrentSlot.SingleItem = FStoreItem();
	}
	else if (CurrentSlot.Quantity > 0)
	{
		// Supply box: return the item and decrement quantity
		OutItem = CurrentSlot.SupplyBoxItem;
		CurrentSlot.Quantity--;

		// If quantity reaches zero, clear the supply box
		if (CurrentSlot.Quantity <= 0)
		{
			CurrentSlot.SupplyBoxItem = FStoreItem();
		}
	}

	RefreshHotbar(); // Update the UI
	return true;
}

