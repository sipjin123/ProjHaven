// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "HavenPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class SCRAPHAVEN_API AHavenPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Stats")
	int TotalCredits;

	void AddCredit(int NewQuantity) { TotalCredits += NewQuantity; }
	int GetTotalCredits() const { return TotalCredits; }
};
