// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/FStoreItem.h"
#include "Engine/DataTable.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "StoreSubsystem.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class SCRAPHAVEN_API UStoreSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// Initialize when the subsystem is created
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// Data table reference (assign in editor via defaults, or load from path)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Store")
	UDataTable* ItemDataTable;

	// Cached items
	UPROPERTY(BlueprintReadOnly, Category="Store")
	TArray<FStoreItem> AllItems;

	// Lookup by Name
	UFUNCTION(BlueprintCallable, Category="Store")
	FStoreItem GetItemByName(FName RowName) const;
};