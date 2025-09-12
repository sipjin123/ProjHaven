// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StoreWidget.generated.h"

// Forward declare UI classes
class UUniformGridPanel;
class UTextBlock;
class UImage;
class UButton;
class UItemSlotWidget;
/**
 * 
 */
UCLASS()
class SCRAPHAVEN_API UStoreWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	// Populate inventory
	UFUNCTION(BlueprintCallable, Category="Inventory")
	void PopulateInventory(const TArray<FStoreItem>& Items, int32 Columns = 4);

protected:
	UPROPERTY(meta=(BindWidget))
	UUniformGridPanel* GridPanel;

	UPROPERTY(EditAnywhere, Category="Inventory")
	TSubclassOf<UItemSlotWidget> ItemSlotClass;
};