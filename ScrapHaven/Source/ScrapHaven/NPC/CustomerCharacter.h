// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCBehaviorGenerator.h"
#include "StructData.h"
#include "GameFramework/Character.h"
#include "Interfaces/Sociable.h"
#include "CustomerCharacter.generated.h"

UCLASS()
class SCRAPHAVEN_API ACustomerCharacter : public ACharacter, public ISociable
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Status")
	FName NpcName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Status")
	FNPCData NPCData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Status")
	FDailyBehaviorProfile DailyBehaviorProfile;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Status")
	FShopListData ShopListData;
};
