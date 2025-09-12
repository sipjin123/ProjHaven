// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/StoreWidget.h"
#include "ItemSlotWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Data/FStoreItem.h"
#include "NPC/StructData.h"

void UStoreWidget::PopulateInventory(const TArray<FStoreItem>& Items, int32 Columns)
{
	if (!GridPanel || !ItemSlotClass) return;

	GridPanel->ClearChildren();

	for (int32 i = 0; i < Items.Num(); i++)
	{
		const FStoreItem& Data = Items[i];

		if (UItemSlotWidget* ItemWidget = CreateWidget<UItemSlotWidget>(this, ItemSlotClass))
		{
			ItemWidget->SetupSlot(Data.ItemName, Data.Quantity, Data.Icon);

			int32 Row = i / Columns;
			int32 Col = i % Columns;

			if (UUniformGridSlot* GridSlot = GridPanel->AddChildToUniformGrid(ItemWidget, Row, Col))
			{
				GridSlot->SetHorizontalAlignment(HAlign_Fill);
				GridSlot->SetVerticalAlignment(VAlign_Fill);
			}
		}
	}
}
