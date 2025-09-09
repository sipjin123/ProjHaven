// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/WorldSimSubsystem.h"

void UWorldSimulationSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	CurrentTimeOfDay = 6.f; // default 6 AM
	CurrentDay = 1;
	CurrentDayState = ETimeOfDay::Morning;
}

void UWorldSimulationSubsystem::Initialize(FSubsystemCollectionBase& collection)
{
	Super::Initialize(collection);
}

void UWorldSimulationSubsystem::StartDay(bool bIsNewDay)
{
	// If this is called after a "day ended" popup
	if (bIsNewDay)
	{
		CurrentDay++;
		OnNewDay.Broadcast();
	}

	// Reset to morning
	CurrentTimeOfDay = 6.f;
	CurrentDayState = ETimeOfDay::Morning;
	bIsRunning = true;

	OnDayStateChanged.Broadcast(CurrentDayState);
}

void UWorldSimulationSubsystem::Tick(float DeltaTime)
{
	if (!bIsRunning) return;

	float SecondsPerDay = 16.f * 60.f; // 16 minutes = 960 seconds
	float HoursPerSecond = 16.f / SecondsPerDay; // ~0.0166 hours/sec

	CurrentTimeOfDay += DeltaTime * HoursPerSecond;

	if (CurrentTimeOfDay >= 22.f) // End of day
	{
		CurrentTimeOfDay = 6.f; // restart next day
		CurrentDay++;
		OnNewDay.Broadcast();
		OnDayStateChanged.Broadcast(ETimeOfDay::Morning);
		CurrentDayState = ETimeOfDay::Morning;
	}

	UpdateDayState();
}

void UWorldSimulationSubsystem::UpdateDayState()
{
	ETimeOfDay NewState;

	if (CurrentTimeOfDay >= 6.f && CurrentTimeOfDay < 12.f)
		NewState = ETimeOfDay::Morning;
	else if (CurrentTimeOfDay >= 12.f && CurrentTimeOfDay < 18.f)
		NewState = ETimeOfDay::Afternoon;
	else
		NewState = ETimeOfDay::Night;

	if (NewState != CurrentDayState)
	{
		CurrentDayState = NewState;
		OnDayStateChanged.Broadcast(CurrentDayState);
	}
}

void UWorldSimulationSubsystem::SkipToNextState()
{
	if (!bIsRunning) return;

	if (CurrentDayState == ETimeOfDay::Morning)
	{
		CurrentTimeOfDay = 12.f; // noon
		CurrentDayState = ETimeOfDay::Afternoon;
		OnDayStateChanged.Broadcast(CurrentDayState);
	}
	else if (CurrentDayState == ETimeOfDay::Afternoon)
	{
		CurrentTimeOfDay = 18.f; // evening
		CurrentDayState = ETimeOfDay::Night;
		OnDayStateChanged.Broadcast(CurrentDayState);
	}
	else if (CurrentDayState == ETimeOfDay::Night)
	{
		// Stop time progression until StartNewDay is called
		bIsRunning = false;

		// Freeze at "end of day" marker
		CurrentTimeOfDay = 22.f;

		// Let listeners know day ended, UI can show popup
		FOnEndDay.Broadcast(); 
	}
}
