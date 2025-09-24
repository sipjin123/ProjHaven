// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FQuestData.generated.h"

USTRUCT(BlueprintType)
struct FQuestData : public FTableRowBase
{
	GENERATED_BODY();

	// -------------------------
	// Metadata
	// -------------------------

	// Unique ID for quest (used as RowName in DataTable ideally)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest")
	FName QuestID;

	// Display name (localizable)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest")
	FText QuestName;

	// Short description (localizable)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest")
	FText Description;

	// Optional: narrative flavor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest")
	FText JournalEntry;

	// -------------------------
	// Structure
	// -------------------------

	// Prerequisite quest(s)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest")
	TArray<FName> PrerequisiteQuests;

	// Quest steps (can be expanded into another DataTable or a struct array)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest")
	TArray<FName> Objectives;  

	// Whether quest can be repeated
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest")
	bool bRepeatable = false;

	// -------------------------
	// Rewards
	// -------------------------

	// Items to grant upon completion
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest")
	TArray<FName> RewardItems;

	// Currency, XP, etc.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest")
	int32 RewardXP = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quest")
	int32 RewardCurrency = 0;
};