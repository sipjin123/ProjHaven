// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCBehaviorGenerator.h"
#include "StructData.h"
#include "GameFramework/Character.h"
#include "Interfaces/Sociable.h"
#include "Interfaces/Trackable.h"
#include "CustomerCharacter.generated.h"

UCLASS()
class SCRAPHAVEN_API ACustomerCharacter : public ACharacter, public ISociable, public ITrackable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACustomerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintCallable, Category="Shopping")
	void AddToShoppingBasket(FName ItemName, int32 Quantity);
	
	// Inside your NPC class
	UFUNCTION(BlueprintCallable, Category="AI|Behavior")
	bool ShouldWanderToOtherShelf(const FDailyBehaviorProfile& Profile, float BaseWanderChance);
	
	UFUNCTION(BlueprintCallable, Category="AI|Behavior")
	void OnWanderBuyItem(const FName& ItemName);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Status")
	FName NpcName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Status")
	FNPCData NPCData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Status")
	FDailyBehaviorProfile DailyBehaviorProfile;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Status")
	FShopListData ShopListData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Coord")
	TArray<FItemPurchasePair> ShoppingBasket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Coord")
	EBuildingType CurrentLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Coord")
	int32 CurrentLocationVariant;
	
	// Max times this NPC is allowed to wander/buy extra items in a day
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Behavior")
	int32 MaxWanderBuys = 2;

	// Current times NPC already wandered off and bought something
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Behavior")
	int32 WanderBuysSoFar = 0;
	
	UFUNCTION(BlueprintCallable, Category="Shop")
	bool ContainsItem(const FName& ItemName, const TArray<FItemPurchasePair>& ShoppingListArray) const
	{
		for (const FItemPurchasePair& Pair : ShoppingListArray)
		{
			if (Pair.ItemName == ItemName)
			{
				return true;
			}
		}
		return false;
	}
	
	UFUNCTION(BlueprintCallable, Category="Shop")
	bool ContainsItemWithQuantity(const TArray<FItemPurchasePair>& ShoppingListArray) const
	{
		for (const FItemPurchasePair& Pair : ShoppingListArray)
		{
			if (Pair.Quantity > 0)
			{
				return true;
			}
		}
		return false;
	}
	
	UFUNCTION(BlueprintCallable, Category="Behavior")
	bool CanWanderBuy() const
	{
		return WanderBuysSoFar < MaxWanderBuys;
	}
	
	virtual FPOIBasic GetPOIBasic_Implementation() override
	{
		FPOIBasic Result;
		Result.BuildingType = CurrentLocation;
		Result.UniqueId = CurrentLocationVariant;
		return Result;
	};

	virtual void AssignNewPOIData_Implementation(FPOIBasic NewPOI) override
	{
		CurrentLocation = NewPOI.BuildingType;
		CurrentLocationVariant = NewPOI.UniqueId;
	}
};
