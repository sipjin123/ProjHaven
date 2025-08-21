// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerShopHUD.generated.h"

// Forward declarations
class UHorizontalBox;
class UUniformGridPanel;
class UUserWidget;
/**
 * 
 */
UCLASS()
class SCRAPHAVEN_API UPlayerShopHUD : public UUserWidget
{
	GENERATED_BODY()
public:
	/** Build the inventory UI with sample items */
	UFUNCTION(BlueprintCallable, Category="UI")
	void BuildInventoryUI();

protected:
	/** Box that holds the player’s items (row of 5) */
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UHorizontalBox* UserItemsBox;

	/** Grid for inventory (3x3) */
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UUniformGridPanel* InventoryGrid;

	/** Widget class used for items */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> ItemWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> InventoryWidgetClass;
};
