// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FCitizenAttributes.h"
#include "FCitizenNeedsModifiers.h"
#include "FCitizenArchetypeRow.generated.h"

USTRUCT(BlueprintType)
struct FCitizenArchetypeRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	// Citizen identity
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Citizen")
	FName CitizenName; // e.g. "John Doe"

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Citizen")
	FString Archetype; // e.g. "Glutton", "Workaholic", "Fragile"

	// Base stats (starting needs)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Needs")
	FCitizenAttributes BaseNeeds;

	// How their needs evolve
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Needs|Modifiers")
	FCitizenNeedsModifiers Modifiers;

	// Optional flavor tags
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Citizen")
	FString Description; // For lore or UI tooltip
};