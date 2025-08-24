// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Shopping/SupplyBox.h"
#include "HavenBPFLibrary.generated.h"

/**
 * 
 */
UCLASS()
class SCRAPHAVEN_API UHavenBPFLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Store", meta=(WorldContext="WorldContextObject"))
	static ASupplyBox* SpawnSupplyBoxActor(
		UObject* WorldContextObject,
		TSubclassOf<ASupplyBox> BoxClass,
		const FTransform& SpawnTransform,
		FStoreItem ItemData,
		FName BoxName,
		int32 Quantity
		);

	UFUNCTION(BlueprintCallable, Category="Store")
	static TSubclassOf<AActor> GetSpawnClassForItem(const FStoreItem& ItemData);
	
	UFUNCTION(BlueprintPure, Category="Floor Trace", meta=(DefaultToSelf="Actor"))
	 static bool GetFloorInFront(AActor* Actor, FVector& OutFloorLocation, float ForwardDistance = 100.f, float TraceDistance = 500.f);
};