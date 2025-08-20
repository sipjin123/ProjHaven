// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "NPCManagerSubsystem.generated.h"

/**
 * 
 */

UCLASS(Abstract, Blueprintable)
class SCRAPHAVEN_API UNPCManagerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<TWeakObjectPtr<AActor>> TrackedNPCs;

	void RegisterNPC(AActor* NPC) { TrackedNPCs.Add(NPC); }
	void UnregisterNPC(AActor* NPC) { TrackedNPCs.Remove(NPC); }
};