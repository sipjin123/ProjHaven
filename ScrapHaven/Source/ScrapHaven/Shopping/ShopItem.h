// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/FStoreItem.h"
#include "GameFramework/Actor.h"
#include "ShopItem.generated.h"

UCLASS()
class SCRAPHAVEN_API AShopItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShopItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Row name in the Store DataTable
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Store")
	FName StoreItemRow;

	// Actual resolved data
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Store")
	FStoreItem StoreItem;
};
