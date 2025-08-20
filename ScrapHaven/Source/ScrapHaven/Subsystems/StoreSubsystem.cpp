// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/StoreSubsystem.h"

void UStoreSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (ItemDataTable)
	{
		TArray<FStoreItem*> Rows;
		ItemDataTable->GetAllRows<FStoreItem>(TEXT("StoreSubsystem Load"), Rows);

		AllItems.Empty();
		for (FStoreItem* Row : Rows)
		{
			if (Row)
			{
				AllItems.Add(*Row); // copy struct into array
			}
		}
	}
}

FStoreItem UStoreSubsystem::GetItemByName(FName RowName) const
{
	if (ItemDataTable)
	{
		if (const FStoreItem* Row = ItemDataTable->FindRow<FStoreItem>(RowName, TEXT("StoreSubsystem Lookup")))
		{
			return *Row; // return a copy
		}
	}
	return FStoreItem(); // default
}