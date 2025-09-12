// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemSlotWidget.generated.h"

// Forward declarations
class UImage;
class UTextBlock;
class UTexture2D;
/**
 * 
 */
UCLASS()
class SCRAPHAVEN_API UItemSlotWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	// Update slot data
	UFUNCTION(BlueprintCallable, Category="Item")
	void SetupSlot(const FName& InName, int32 InQuantity, UTexture2D* InIcon);

protected:
	UPROPERTY(meta=(BindWidget))
	UImage* ItemIcon;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemName;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemQuantity;
};