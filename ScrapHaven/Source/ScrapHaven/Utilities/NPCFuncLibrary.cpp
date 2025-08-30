// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/NPCFuncLibrary.h"
#include "HavenEnums.h"
#include "Data/FStoreItem.h"
#include "NPC/NPCBehaviorGenerator.h"
#include "NPC/StructData.h"

float UNPCFuncLibrary::GetShopVisitChance(const FDailyBehaviorProfile& Profile)
{
	// -------------------
	// Base chance
	// -------------------
	float Chance = 10.f; // baseline chance that anyone visits a shop

	// -------------------
	// Needs influence
	// -------------------
	for (const FNeedStatus& Need : Profile.Needs)
	{
		switch (Need.Need)
		{
		case ENPCNeedType::Hungry:
			// Higher intensity hunger pushes toward shopping (food items)
			Chance += Need.Intensity * 0.4f; 
			break;

		case ENPCNeedType::Thirsty:
			// Thirst also pushes shopping, but weaker than hunger
			Chance += Need.Intensity * 0.3f;
			break;

		case ENPCNeedType::Tired:
			// If tired, NPC is LESS likely to shop
			Chance -= Need.Intensity * 0.5f;
			break;

		default:
			break;
		}
	}

	// -------------------
	// Mood influence
	// -------------------
	switch (Profile.Mood)
	{
	case ENPCMoodType::Cheerful:
		// Cheerful → more social/shopping
		Chance += 20.f;
		break;

	case ENPCMoodType::Curious:
		// Curious → more exploring shops
		Chance += 15.f;
		break;

	case ENPCMoodType::Irritable:
		// Irritable → less patient to shop
		Chance -= 15.f;
		break;

	case ENPCMoodType::Dissociate:
		// Withdrawn → unlikely to shop
		Chance -= 25.f;
		break;

	default:
		break;
	}

	// -------------------
	// Clamp result
	// -------------------
	Chance = FMath::Clamp(Chance, 0.f, 100.f);
	return Chance;
}

FDailyBehaviorProfile UNPCFuncLibrary::GenerateDailyBehaviors()
{
	FDailyBehaviorProfile Profile;

	// -------------------
	// Generate Needs
	// -------------------
	// Each need has a chance to appear, with random intensity (0–100)
	if (FMath::RandRange(0, 99) < 50) // 50% chance hungry
	{
		FNeedStatus Hunger;
		Hunger.Need = ENPCNeedType::Hungry;
		Hunger.Intensity = FMath::FRandRange(30.f, 100.f); // mild → severe
		Profile.Needs.Add(Hunger);
	}

	if (FMath::RandRange(0, 99) < 40) // 40% chance thirsty
	{
		FNeedStatus Thirst;
		Thirst.Need = ENPCNeedType::Thirsty;
		Thirst.Intensity = FMath::FRandRange(20.f, 100.f);
		Profile.Needs.Add(Thirst);
	}

	if (FMath::RandRange(0, 99) < 30) // 30% chance tired
	{
		FNeedStatus Tired;
		Tired.Need = ENPCNeedType::Tired;
		Tired.Intensity = FMath::FRandRange(10.f, 100.f);
		Profile.Needs.Add(Tired);
	}

	// -------------------
	// Mood Roll (Weighted by Needs)
	// -------------------
	// Grab intensities (helper lambda for safety)
	auto GetNeedIntensity = [&](ENPCNeedType NeedType) -> float
	{
		const FNeedStatus* Found = Profile.Needs.FindByPredicate(
			[&](const FNeedStatus& N) { return N.Need == NeedType; });

		return Found ? Found->Intensity : 0.f;
	};

	float Hunger = GetNeedIntensity(ENPCNeedType::Hungry);
	float Thirst = GetNeedIntensity(ENPCNeedType::Thirsty);
	float Tired  = GetNeedIntensity(ENPCNeedType::Tired);

	// Assign weights (tweak these to taste)
	float IrritableWeight   = (Hunger + Thirst) * 0.6f; // hungry/thirsty → irritable
	float DissociateWeight  = Tired * 0.8f;             // tired → dissociate
	float CheerfulWeight    = 50.f;                     // baseline happiness
	float CuriousWeight     = 30.f;                     // baseline curiosity

	// Normalize weights into one pool
	float TotalWeight = IrritableWeight + DissociateWeight + CheerfulWeight + CuriousWeight;
	float Roll = FMath::FRandRange(0.f, TotalWeight);

	// Weighted selection
	if (Roll < IrritableWeight)
		Profile.Mood = ENPCMoodType::Irritable;
	else if (Roll < IrritableWeight + DissociateWeight)
		Profile.Mood = ENPCMoodType::Dissociate;
	else if (Roll < IrritableWeight + DissociateWeight + CheerfulWeight)
		Profile.Mood = ENPCMoodType::Cheerful;
	else
		Profile.Mood = ENPCMoodType::Curious;

	return Profile;
}

FString UNPCFuncLibrary::GetNeedAsName(ENPCNeedType Need)
{
	const UEnum* EnumPtr = StaticEnum<ENPCNeedType>();
	if (!EnumPtr) return TEXT("Invalid");

	return EnumPtr->GetDisplayNameTextByValue((int64)Need).ToString();
}

FString UNPCFuncLibrary::GetMoodAsName(ENPCMoodType Mood)
{
	const UEnum* EnumPtr = StaticEnum<ENPCMoodType>();
	if (!EnumPtr) return TEXT("Invalid");

	return EnumPtr->GetDisplayNameTextByValue((int64)Mood).ToString();
}
	
FString UNPCFuncLibrary::CleanupEnumString(const UEnum* EnumPtr, int64 Value)
{
	if (!EnumPtr) return TEXT("Invalid");

	// Try the UMETA(DisplayName)
	FString Nice = EnumPtr->GetDisplayNameTextByValue(Value).ToString();

	// If UE still returns "ENPCNeedType::Hungry", strip the scope
	if (Nice.Contains(TEXT("::")))
	{
		Nice = EnumPtr->GetNameStringByValue(Value); // e.g. "ENPCNeedType::Hungry"
		int32 ScopeIdx;
		if (Nice.FindLastChar(':', ScopeIdx))        // last ':' in "::"
			{
			Nice = Nice.Mid(ScopeIdx + 1);           // -> "Hungry"
			}
	}

	return Nice;
}

FShopListData UNPCFuncLibrary::GenerateShoppingList(const FDailyBehaviorProfile& Behavior, const FPOIData& TargetShop)
{
	FShopListData Result;
	Result.POIData = TargetShop;

	// Loop through needs and decide items
	for (const FNeedStatus& NeedStatus : Behavior.Needs)
	{
		if (NeedStatus.Intensity <= 0.0f) continue;

		FItemPurchasePair Purchase;

		switch (NeedStatus.Need)
		{
		case ENPCNeedType::Hungry:
			Purchase.ItemName = "Food_Bread";
			Purchase.Quantity = FMath::RandRange(1, 3);
			break;

		case ENPCNeedType::Thirsty:
			Purchase.ItemName = "Drink_Water";
			Purchase.Quantity = FMath::RandRange(1, 2);
			break;

		case ENPCNeedType::Tired:
			Purchase.ItemName = "Item_Coffee";
			Purchase.Quantity = 1;
			break;

		default:
			break;
		}

		if (!Purchase.ItemName.IsNone())
		{
			Result.ShoppingListArray.Add(Purchase);
		}
	}

	// Mood influences "extras"
	switch (Behavior.Mood)
	{
	case ENPCMoodType::Cheerful:
		{
			FItemPurchasePair Treat;
			Treat.ItemName = "Food_Cake";
			Treat.Quantity = 1;
			Result.ShoppingListArray.Add(Treat);
			break;
		}
	case ENPCMoodType::Curious:
		{
			FItemPurchasePair RandomItem;
			TArray<FName> CuriosityItems = { "Book", "Toy", "Tool" };
			RandomItem.ItemName = CuriosityItems[FMath::RandHelper(CuriosityItems.Num())];
			RandomItem.Quantity = 1;
			Result.ShoppingListArray.Add(RandomItem);
			break;
		}
	case ENPCMoodType::Irritable:
		{
			FItemPurchasePair Comfort;
			Comfort.ItemName = "Snack_Chips";
			Comfort.Quantity = FMath::RandRange(1, 2);
			Result.ShoppingListArray.Add(Comfort);
			break;
		}
	case ENPCMoodType::Dissociate:
		{
			FItemPurchasePair Simple;
			Simple.ItemName = "InstantNoodles";
			Simple.Quantity = 1;
			Result.ShoppingListArray.Add(Simple);
			break;
		}
	default:
		break;
	}

	return Result;
}
