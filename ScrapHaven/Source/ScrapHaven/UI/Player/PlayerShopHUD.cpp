// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerShopHUD.h"
#include "Components/HorizontalBox.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/HorizontalBoxSlot.h"
#include "Blueprint/UserWidget.h"

void UPlayerShopHUD::BuildInventoryUI()
{
	if (!ItemWidgetClass) return;

	// --- Clear old widgets
	if (UserItemsBox)
	{
		UserItemsBox->ClearChildren();
	}

	if (InventoryGrid)
	{
		InventoryGrid->ClearChildren();
	}

	// --- Add 5 items in a row (User items)
	for (int32 i = 0; i < 5; i++)
	{
		UUserWidget* Item = CreateWidget<UUserWidget>(GetWorld(), ItemWidgetClass);
		if (Item && UserItemsBox)
		{
			UserItemsBox->AddChildToHorizontalBox(Item);
		}
	}

	// --- Add 3x3 grid for inventory
	int32 Rows = 3;
	int32 Cols = 3;

	for (int32 Row = 0; Row < Rows; Row++)
	{
		for (int32 Col = 0; Col < Cols; Col++)
		{
			UUserWidget* Item = CreateWidget<UUserWidget>(GetWorld(), InventoryWidgetClass);
			if (Item && InventoryGrid)
			{
				UUniformGridSlot* NewSlot = InventoryGrid->AddChildToUniformGrid(Item, Row, Col);
				if (NewSlot)
				{
					NewSlot->SetHorizontalAlignment(HAlign_Center);
					NewSlot->SetVerticalAlignment(VAlign_Center);
				}
			}
		}
	}
}
