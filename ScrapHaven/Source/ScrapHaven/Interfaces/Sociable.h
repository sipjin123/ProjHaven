// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Sociable.generated.h"

/**
 * 
 */

UINTERFACE(MinimalAPI)
class USociable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SCRAPHAVEN_API ISociable
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Custom")
	void Socialize();
	
};
