// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FCitizenNeedsModifiers.generated.h"

USTRUCT(BlueprintType)
struct FCitizenNeedsModifiers
{
	GENERATED_BODY()

public:
	// Rate multipliers (1.0 = normal)
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Needs|Modifiers", meta=(ClampMin="0.0", ClampMax="10.0", UIMin="0.0", UIMax="10.0"))
	float HungerRate = 1.0f; // >1.0 = gets hungry faster, <1.0 = slower

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Needs|Modifiers", meta=(ClampMin="0.0", ClampMax="10.0", UIMin="0.0", UIMax="10.0"))
	float EnergyDrainRate = 1.0f; // >1.0 = tires faster

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Needs|Modifiers", meta=(ClampMin="0.0", ClampMax="10.0", UIMin="0.0", UIMax="10.0"))
	float HealthDecayRate = 1.0f; // >1.0 = gets sick easily

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Needs|Modifiers", meta=(ClampMin="0.0", ClampMax="10.0", UIMin="0.0", UIMax="10.0"))
	float HappinessSensitivity = 1.0f; // >1.0 = mood changes easily

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Needs|Modifiers", meta=(ClampMin="0.0", ClampMax="10.0", UIMin="0.0", UIMax="10.0"))
	float SocialNeedRate = 1.0f; // >1.0 = craves socializing more often

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Needs|Modifiers", meta=(ClampMin="0.0", ClampMax="10.0", UIMin="0.0", UIMax="10.0"))
	float WealthSpendingRate = 1.0f; // >1.0 = spends money faster
};

/*
Glutton (HungerRate = 2.0, SocialNeedRate = 0.8)
Workaholic (EnergyDrainRate = 1.5, SocialNeedRate = 0.5)
Fragile (HealthDecayRate = 2.0, HappinessSensitivity = 1.5)
Cheerful (HappinessSensitivity = 0.5, SocialNeedRate = 1.2)
*/