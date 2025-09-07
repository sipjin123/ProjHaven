// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC/StructData.h"
#include "UObject/Interface.h"
#include "Trackable.generated.h"

UINTERFACE(MinimalAPI)
class UTrackable : public UInterface
{
	GENERATED_BODY()
};

class SCRAPHAVEN_API ITrackable
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Custom")
	FPOIBasic GetPOIBasic();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Custom")
	void AssignNewPOIData(FPOIBasic NewPOI);
};
