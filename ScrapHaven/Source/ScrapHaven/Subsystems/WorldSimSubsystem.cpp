// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/WorldSimSubsystem.h"

void UWorldSimulationSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	CurrentTimeOfDay = 6.f; // Start at 6 AM
}

void UWorldSimulationSubsystem::Initialize(FSubsystemCollectionBase& collection)
{
	Super::Initialize(collection);
}

void UWorldSimulationSubsystem::Tick(float DeltaTime)
{
	// Inside Tick
	float SecondsPerDay = 16.f * 60.f; // 16 minutes = 960 seconds
	float HoursPerSecond = 16.f / SecondsPerDay; // 0.01666... hours per real second

	CurrentTimeOfDay += DeltaTime * HoursPerSecond;

	// Clamp between 6 and 22
	if (CurrentTimeOfDay >= 22.f)
	{
		CurrentTimeOfDay = 6.f; // restart next day
		CurrentDay++;
		OnNewDay.Broadcast();
	}
}
