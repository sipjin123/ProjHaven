// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HavenEnums.h"
#include "Data/FStoreItem.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "NPC/NPCBehaviorGenerator.h"
#include "NPC/StructData.h"
#include "NPCFuncLibrary.generated.h"

/**
 * 
 */
UCLASS()
class SCRAPHAVEN_API UNPCFuncLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category="NPC|Behavior")
	float GetShopVisitChance(const FDailyBehaviorProfile& Profile);
	UFUNCTION(BlueprintCallable, Category="NPC|Behavior")
	static FDailyBehaviorProfile GenerateDailyBehaviors();


	UFUNCTION(BlueprintCallable, BlueprintPure, Category="NPC|Behavior")
	static FString GetNeedAsName(ENPCNeedType Need);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="NPC|Behavior")
	static FString GetMoodAsName(ENPCMoodType Mood);
	FString CleanupEnumString(const UEnum* EnumPtr, int64 Value);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="NPC|Behavior")
	static FShopListData GenerateShoppingList(const FDailyBehaviorProfile& Behavior, const FPOIData& TargetShop);
};
