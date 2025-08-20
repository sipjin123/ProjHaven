// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/FStoreItem.h"
#include "GameFramework/Actor.h"
#include "ShelfSector.generated.h"

UCLASS()
class SCRAPHAVEN_API AShelfSector : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShelfSector();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Store")
	FStoreItem StoreItemRefData;

	UFUNCTION(BlueprintCallable)
	void RegisterStoreItem(const FStoreItem& StoreItemRef);
};
