// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FCitizenAttributes.generated.h"

USTRUCT(BlueprintType)
struct FCitizenAttributes
{
	GENERATED_BODY()

public:

	// How hungry the citizen is (0 = starving, 100 = full)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Needs", meta=(ClampMin="0.0", ClampMax="100.0", UIMin="0.0", UIMax="100.0"))
	float Hunger = 100.f;

	// How much energy they have (0 = exhausted, 100 = fully rested)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Needs", meta=(ClampMin="0.0", ClampMax="100.0", UIMin="0.0", UIMax="100.0"))
	float Energy = 100.f;

	// Overall health of the citizen (0 = dead/sick, 100 = perfectly healthy)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Needs", meta=(ClampMin="0.0", ClampMax="100.0", UIMin="0.0", UIMax="100.0"))
	float Health = 100.f;

	// General happiness or morale (0 = miserable, 100 = very happy)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Needs", meta=(ClampMin="0.0", ClampMax="100.0", UIMin="0.0", UIMax="100.0"))
	float Happiness = 100.f;

	// Social need (0 = lonely, 100 = socially fulfilled)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Needs", meta=(ClampMin="0.0", ClampMax="100.0", UIMin="0.0", UIMax="100.0"))
	float Social = 100.f;

	// Wealth in coins/money the citizen can spend
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Needs", meta=(ClampMin="0.0", ClampMax="100.0", UIMin="0.0", UIMax="100.0"))
	int32 Wealth = 0;
};