// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NPC/StructData.h"
#include "EstablishmentBase.generated.h"

USTRUCT(BlueprintType)
struct FTestStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Test")
	FVector Location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Test")
	int32 ID = 0;
};

UCLASS()
class SCRAPHAVEN_API AEstablishmentBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEstablishmentBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Coord")
	FPOIData POIData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Coord", meta=(ShowOnlyInnerProperties))
	FPOIData DebugPOIData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Test")
	FTestStruct TestData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Test")
	FDebugStruct DebugStruct;
};
