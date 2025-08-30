// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Utilities/HavenEnums.h"
#include "NPCBehaviorGenerator.generated.h"


USTRUCT(BlueprintType)
struct FNeedStatus
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	ENPCNeedType Need = ENPCNeedType::None;

	UPROPERTY(BlueprintReadWrite)
	float Intensity = 0.0f; // 0–100

	// Proper operator== so TArray functions won’t break
	bool operator==(const FNeedStatus& Other) const
	{
		return Need == Other.Need; // Compare only by type
	}
};

USTRUCT(BlueprintType)
struct FDailyBehaviorProfile
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category="Behavior")
	TArray<FNeedStatus> Needs;

	UPROPERTY(BlueprintReadWrite, Category="Behavior")
	ENPCMoodType Mood = ENPCMoodType::None;

	// Safe accessor — no Contains() with mismatched types
	float GetNeedIntensity(ENPCNeedType NeedType) const
	{
		for (const FNeedStatus& Need : Needs)
		{
			if (Need.Need == NeedType)
			{
				return Need.Intensity;
			}
		}
		return 0.f;
	}

	// Adds or updates a need
	void AddNeed(ENPCNeedType NeedType, float Intensity)
	{
		for (FNeedStatus& Need : Needs)
		{
			if (Need.Need == NeedType)
			{
				Need.Intensity = Intensity;
				return;
			}
		}
		Needs.Add({NeedType, Intensity});
	}
};


/**
 * 
 */
UCLASS()
class SCRAPHAVEN_API UNPCBehaviorGenerator : public UObject
{
	GENERATED_BODY()

public:

};