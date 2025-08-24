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

    const int32 BoxSlotIndex = HotbarCount - 1; // last slot = carried box

    // --- Create hotbar widgets if they don’t exist ---
    if (HotbarWidgets.Num() < HotbarCount)
    {
        HotbarWidgets.SetNum(HotbarCount);
        UserItemsBox->ClearChildren();

        for (int32 i = 0; i < HotbarCount; i++)
        {
            UInventorySlotWidget* ItemWidget = CreateWidget<UInventorySlotWidget>(this, InventoryWidgetClass);
            if (ItemWidget)
            {
                UserItemsBox->AddChild(ItemWidget);
                HotbarWidgets[i] = ItemWidget;
            }
        }
    }

    // --- Update each slot ---
    for (int32 i = 0; i < HotbarCount; i++)
    {
        UInventorySlotWidget* Widget = HotbarWidgets[i];
        if (!Widget) continue;

        if (i == BoxSlotIndex)
        {
            // Carried box slot
            if (!CarriedBox.IsEmpty())
            {
                Widget->UpdateSlot(FStoreItem(), CarriedBox.CachedItem, CarriedBox.Quantity);
            }
            else
            {
                Widget->UpdateSlot(FStoreItem(), FStoreItem(), 0);
            }
        }
        else
        {
            // Normal hotbar slot
            if (!InventorySlots.IsValidIndex(i))
            {
                Widget->UpdateSlot(FStoreItem(), FStoreItem(), 0);
                continue;
            }

            FInventorySlot& NewSlot = InventorySlots[i];
            if (!NewSlot.SingleItem.ItemName.IsEmpty())
            {
                Widget->UpdateSlot(NewSlot.SingleItem, FStoreItem(), 1);
            }
            else if (NewSlot.Quantity > 0)
            {
                Widget->UpdateSlot(FStoreItem(), NewSlot.SupplyBoxItem, NewSlot.Quantity);
            }
            else
            {
                Widget->UpdateSlot(FStoreItem(), FStoreItem(), 0);
            }
        }

        // Highlight selected slot
        Widget->SetHighlighted(i == SelectedHotbarIndex);
    }
}

void UPlayerShopHUD::SelectHotbarSlot(int32 SlotIndex)
{
	if (SlotIndex < 0 || SlotIndex >= HotbarCount) return;

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

void UPlayerShopHUD::RefreshHotbar()
{
	BuildInventoryUI();
}

// PlayerShopHUD.cpp
bool UPlayerShopHUD::IsCurrentSlotBox() const
{
	const int32 BoxSlotIndex = HotbarCount - 1;

	if (SelectedHotbarIndex != BoxSlotIndex)
	{
		return false; // Not the box slot
	}

	return !CarriedBox.IsEmpty(); // True if we are carrying a valid box
}

bool UPlayerShopHUD::HandleUseCurrentSlot(FStoreItem& OutItem)
{
    if (SelectedHotbarIndex < 0 || SelectedHotbarIndex >= HotbarWidgets.Num())
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid SelectedHotbarIndex = %d"), SelectedHotbarIndex);
        return false;
    }

    const int32 BoxSlotIndex = HotbarCount-1;

    // --- Carried box slot ---
    if (SelectedHotbarIndex == BoxSlotIndex)
    {
        if (CarriedBox.IsEmpty())
        {
            UE_LOG(LogTemp, Warning, TEXT("Carried box is empty"));
            return false;
        }

        UE_LOG(LogTemp, Log, TEXT("Using item '%s' from carried box, remaining %d"), *CarriedBox.CachedItem.ItemName.ToString(), CarriedBox.Quantity - 1);
        OutItem = CarriedBox.CachedItem;
        CarriedBox.Quantity--;

        if (CarriedBox.Quantity <= 0)
        {
            UE_LOG(LogTemp, Log, TEXT("Carried box is now empty"));
            CarriedBox = FCarriedBox();
        }

        RefreshHotbar();
        return true;
    }

    // --- Normal hotbar slots ---
    if (!InventorySlots.IsValidIndex(SelectedHotbarIndex))
    {
        UE_LOG(LogTemp, Warning, TEXT("Inventory slot %d is invalid"), SelectedHotbarIndex);
        return false;
    }

    FInventorySlot& CurrentSlot = InventorySlots[SelectedHotbarIndex];

    if (CurrentSlot.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("Hotbar slot %d is empty"), SelectedHotbarIndex);
        return false;
    }

    if (!CurrentSlot.SingleItem.ItemName.IsEmpty())
    {
        UE_LOG(LogTemp, Log, TEXT("Using single item '%s' from slot %d"), *CurrentSlot.SingleItem.ItemName.ToString(), SelectedHotbarIndex);
        OutItem = CurrentSlot.SingleItem;
        CurrentSlot.SingleItem = FStoreItem();
        CurrentSlot.Quantity = 0;
    }
    else if (CurrentSlot.Quantity > 0)
    {
        UE_LOG(LogTemp, Log, TEXT("Using supply box item '%s' from slot %d, remaining %d"), *CurrentSlot.SupplyBoxItem.ItemName.ToString(), SelectedHotbarIndex, CurrentSlot.Quantity - 1);
        OutItem = CurrentSlot.SupplyBoxItem;
        CurrentSlot.Quantity--;

        if (CurrentSlot.Quantity <= 0)
        {
            CurrentSlot.SupplyBoxItem = FStoreItem();
        }
    }

    RefreshHotbar();
    return true;
}

bool UPlayerShopHUD::AddBox(const FCarriedBox& NewBox)
{
	const int32 BoxSlotIndex = HotbarCount - 1; // last slot = carried box

	// Ensure the hotbar widget exists
	if (!HotbarWidgets.IsValidIndex(BoxSlotIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("AddBox: Hotbar widget not ready yet!"));
		return false;
	}

	// Prevent picking up another box if already carrying one
	if (!CarriedBox.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot add box: already carrying a box"));
		return false;
	}

	// Assign the new box
	CarriedBox = NewBox;

	// Update UI immediately
	if (UInventorySlotWidget* BoxWidget = HotbarWidgets[BoxSlotIndex])
	{
		BoxWidget->UpdateSlot(FStoreItem(), CarriedBox.CachedItem, CarriedBox.Quantity);
	}

	UE_LOG(LogTemp, Log, TEXT("Picked up box '%s' with %d items"), 
		*CarriedBox.CachedItem.ItemName.ToString(), CarriedBox.Quantity);

	return true;
}
// PlayerShopHUD.cpp
bool UPlayerShopHUD::RemoveBox(FStoreItem& OutItem, int32& OutQuantity, FName& OutBoxType)
{
	if (CarriedBox.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot remove box: no box carried"));
		return false;
	}

	// Extract info before clearing
	OutItem     = CarriedBox.CachedItem;
	OutQuantity = CarriedBox.Quantity;
	OutBoxType  = CarriedBox.BoxType;

	UE_LOG(LogTemp, Log, TEXT("Removed carried box of type '%s' containing '%s', quantity %d"),
		*OutBoxType.ToString(),
		*OutItem.ItemName.ToString(),
		OutQuantity);

	// Clear box
	CarriedBox = FCarriedBox();

	// Update UI
	FInventorySlot SlotData;
	SlotData.SupplyBoxItem = FStoreItem();
	SlotData.Quantity = 0;
	OnInventorySlotUpdated.Broadcast(SlotData);

	RefreshHotbar();
	return true;
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

bool UPlayerShopHUD::RemoveItemFromSlot(int32 SlotIndex, FStoreItem& OutItem)
{
	if (!InventorySlots.IsValidIndex(SlotIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("RemoveItemFromSlot: Invalid slot index %d"), SlotIndex);
		return false;
	}

	FInventorySlot& NewSlot = InventorySlots[SlotIndex];

	if (NewSlot.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("RemoveItemFromSlot: Slot %d is empty"), SlotIndex);
		return false;
	}

	// Remove a single item
	if (!NewSlot.SingleItem.ItemName.IsEmpty())
	{
		OutItem = NewSlot.SingleItem;
		NewSlot.SingleItem = FStoreItem();
		NewSlot.Quantity = 0;
		UE_LOG(LogTemp, Log, TEXT("Removed single item '%s' from slot %d"), *OutItem.ItemName.ToString(), SlotIndex);
	}
	else if (NewSlot.Quantity > 0)
	{
		OutItem = NewSlot.SupplyBoxItem;
		NewSlot.Quantity--;

		if (NewSlot.Quantity <= 0)
		{
			NewSlot.SupplyBoxItem = FStoreItem();
		}

		UE_LOG(LogTemp, Log, TEXT("Removed item from supply box in slot %d, remaining quantity %d"), SlotIndex, NewSlot.Quantity);
	}

	// Notify UI / Blueprint listeners
	OnInventorySlotUpdated.Broadcast(NewSlot);
	RefreshHotbar();

	return true;
}

bool UPlayerShopHUD::TryAddItemToCarriedBox(const FStoreItem& ItemData, int32 Amount)
{
	if (CarriedBox.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("No carried box to add items into"));
		return false;
	}

	if (!CarriedBox.CachedItem.ItemName.EqualTo(ItemData.ItemName))
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot add item '%s' to box of type '%s'"),
			*ItemData.ItemName.ToString(),
			*CarriedBox.CachedItem.ItemName.ToString());
		return false;
	}

	if (CarriedBox.Quantity >= CarriedBox.MaxQuantity)
	{
		UE_LOG(LogTemp, Warning, TEXT("Carried box already full (%d/%d)"),
			CarriedBox.Quantity, CarriedBox.MaxQuantity);
		return false;
	}

	int32 SpaceLeft = CarriedBox.MaxQuantity - CarriedBox.Quantity;
	int32 AmountToAdd = FMath::Min(Amount, SpaceLeft);

	CarriedBox.Quantity += AmountToAdd;

	UE_LOG(LogTemp, Log, TEXT("Added %d of '%s' into carried box. Now at %d/%d"),
		AmountToAdd,
		*ItemData.ItemName.ToString(),
		CarriedBox.Quantity,
		CarriedBox.MaxQuantity);

	RefreshHotbar();
	return true;
}

bool UPlayerShopHUD::HasVacantNormalSlot() const
{
	for (int32 i = 0; i < HotbarCount-1; i++)
	{
		if (InventorySlots.IsValidIndex(i) && InventorySlots[i].IsEmpty())
		{
			UE_LOG(LogTemp, Log, TEXT("Vacant slot found at index %d"), i);
			return true;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("No vacant normal slots available"));
	return false;
}
