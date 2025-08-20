// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "WorldSimSubsystem.generated.h"

UCLASS(Abstract, Blueprintable)
class SCRAPHAVEN_API UWorldSimulationSubsystem : public UWorldSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

public:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Initialize(FSubsystemCollectionBase& collection) override;

public:
	// Time
	UPROPERTY(BlueprintReadWrite, Category="Time")
	int32 CurrentDay = 1;

	UPROPERTY(BlueprintReadWrite, Category="Time")
	float CurrentTimeOfDay = 6.0f; // hours (0-24)

	UPROPERTY(EditAnywhere, Category="Time")
	float DayLengthSeconds = 600.0f; // 10 minutes per full day

	// Events
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNewDay);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpecialEvent, FName, EventName);

	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnNewDay OnNewDay;

	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnSpecialEvent OnSpecialEvent;

	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override { return true; }
	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(UWorldSimulationSubsystem, STATGROUP_Tickables); }

	void ScheduleEvent(int32 Day, float Hour, FName EventName);

private:
	struct FWorldEvent { int32 Day; float Hour; FName EventName; };
	TArray<FWorldEvent> ScheduledEvents;
};