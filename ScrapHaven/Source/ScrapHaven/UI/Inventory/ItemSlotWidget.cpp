// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/ItemSlotWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UItemSlotWidget::SetupSlot(const FName& InName, int32 InQuantity, UTexture2D* InIcon)
{
	if (ItemName)
		ItemName->SetText(FText::FromName(InName));

	if (ItemQuantity)
		ItemQuantity->SetText(FText::AsNumber(InQuantity));

	if (ItemIcon && InIcon)
		ItemIcon->SetBrushFromTexture(InIcon);
}
