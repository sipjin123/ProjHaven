// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShelfManagerComponent.generated.h"

class UBoxComponent;

USTRUCT(BlueprintType)
struct FItemSlot
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector WorldLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bOccupied = false;
};

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SCRAPHAVEN_API UShelfManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UShelfManagerComponent();

	/** Initialize shelf slots based on shelf and item sizes */
	UFUNCTION(BlueprintCallable, Category="Shelf")
	void InitializeShelf(UBoxComponent* ShelfBox, UBoxComponent* ItemBox);

	/** Place an item in the next free slot */
	UFUNCTION(BlueprintCallable, Category="Shelf")
	bool PlaceItemInNextSlot(AActor* ItemActor);

	/** Reset shelf slots */
	UFUNCTION(BlueprintCallable, Category="Shelf")
	void ClearShelf();

protected:
	UPROPERTY()
	UBoxComponent* ShelfReference;

	UPROPERTY()
	UBoxComponent* ItemReference;

	UPROPERTY()
	TArray<FItemSlot> Slots;

	int32 ItemsPerRow = 1;
	int32 ItemsPerColumn = 1;
	int32 MaxLayers = 1;
};