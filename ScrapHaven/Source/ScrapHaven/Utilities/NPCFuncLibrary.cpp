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

FShopListData UNPCFuncLibrary::GenerateShoppingListComplex(
    const FDailyBehaviorProfile& Behavior,
    const FPOIData& TargetShop,
    const TArray<FStoreItem>& AvailableItems)
{
    FShopListData ShopData;
    ShopData.POIData = TargetShop;

    // --- Handle needs ---
    for (auto& NeedPair : Behavior.Needs)
    {
        ENPCNeedType NeedType = NeedPair.Need; // TMap<ENPCNeedType, float>
        float Intensity = NeedPair.Intensity;

        if (Intensity < 10.0f && Behavior.Mood != ENPCMoodType::Cheerful)
            continue; // skip negligible needs unless cheerful

        // Find items that satisfy this need
        TArray<FStoreItem> MatchingItems;
        for (const FStoreItem& Item : AvailableItems)
        {
            // Determine if item satisfies the need
            float EffectValue = 0.f;
            switch (NeedType)
            {
                case ENPCNeedType::Hungry:   EffectValue = Item.HungerEffect; break;
                case ENPCNeedType::Thirsty:   EffectValue = Item.ThirstEffect; break;
                case ENPCNeedType::Tired:    EffectValue = Item.EnergyEffect; break;
                default: continue;
            }

            if (EffectValue > 0.f)
                MatchingItems.Add(Item);
        }

        if (MatchingItems.Num() == 0)
            continue;

        // Choose one item randomly
        int32 Index = FMath::RandRange(0, MatchingItems.Num() - 1);
        const FStoreItem& ChosenItem = MatchingItems[Index];

        // Quantity based on intensity vs. effect value
        int32 Quantity = FMath::Clamp(FMath::RoundToInt(Intensity / FMath::Max(ChosenItem.HungerEffect, FMath::Max(ChosenItem.ThirstEffect, ChosenItem.EnergyEffect))), 1, 5);

        FItemPurchasePair Pair;
        Pair.ItemName = ChosenItem.ItemName;
        Pair.Quantity = Quantity;

        ShopData.ShoppingListArray.Add(Pair);
    }

    // --- Mood adjustments ---
    switch (Behavior.Mood)
    {
        case ENPCMoodType::Cheerful:
        {
            // Add one random small treat
            TArray<FStoreItem> Treats;
            for (const FStoreItem& Item : AvailableItems)
            {
                if (Item.IsTreat())
                    Treats.Add(Item);
            }

            if (Treats.Num() > 0)
            {
                int32 Index = FMath::RandRange(0, Treats.Num() - 1);
                FItemPurchasePair Pair;
                Pair.ItemName = Treats[Index].ItemName;
                Pair.Quantity = 1;
                ShopData.ShoppingListArray.Add(Pair);
            }
            break;
        }

        case ENPCMoodType::Irritable:
        {
            // Remove non-essential items with low effect values (<30)
            ShopData.ShoppingListArray.RemoveAll([&](const FItemPurchasePair& Pair)
            {
                const FStoreItem* Item = AvailableItems.FindByPredicate([&](const FStoreItem& SI){ return SI.ItemName == Pair.ItemName; });
                if (!Item) return false;

                float MaxEffect = FMath::Max(Item->HungerEffect, FMath::Max(Item->ThirstEffect, Item->EnergyEffect));
                return MaxEffect < 30.f && !Item->bIsEssential;
            });
            break;
        }

        case ENPCMoodType::Dissociate:
        {
            // Only keep essential items
            ShopData.ShoppingListArray.RemoveAll([&](const FItemPurchasePair& Pair)
            {
                const FStoreItem* Item = AvailableItems.FindByPredicate([&](const FStoreItem& SI){ return SI.ItemName == Pair.ItemName; });
                return Item && !Item->bIsEssential;
            });
            break;
        }

        case ENPCMoodType::Curious:
        {
            // Add one random item from available shop items
            if (AvailableItems.Num() > 0)
            {
                int32 Index = FMath::RandRange(0, AvailableItems.Num() - 1);
                FItemPurchasePair Pair;
                Pair.ItemName = AvailableItems[Index].ItemName;
                Pair.Quantity = 1;
                ShopData.ShoppingListArray.Add(Pair);
            }
            break;
        }

        default:
            break;
    }

    return ShopData;
}

TArray<FItemPurchasePair> UNPCFuncLibrary::GenerateSampleInventory(UDataTable* ItemDataTable, int32 NumItems, int32 MinQuantity, int32 MaxQuantity)
{

	TArray<FItemPurchasePair> Inventory;

	if (!ItemDataTable) 
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemDataTable is null in UStoreSubsystem!"));
		return Inventory;
	}

	// Get all rows from the DataTable
	static const FString ContextString(TEXT("GenerateRandomInventory"));
	TArray<FStoreItem*> AllItems;
	ItemDataTable->GetAllRows(ContextString, AllItems);

	if (AllItems.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No rows in ItemDataTable!"));
		return Inventory;
	}

	// Clamp number of items to avoid out-of-range
	NumItems = FMath::Clamp(NumItems, 1, AllItems.Num());

	// Randomly shuffle and pick items
	TArray<int32> Indices;
	for (int32 i = 0; i < AllItems.Num(); i++) { Indices.Add(i); }
	Indices.Sort([](int32 A, int32 B){ return FMath::RandRange(0,1) == 0; }); // shuffle

	for (int32 i = 0; i < NumItems; i++)
	{
		FStoreItem* StoreItem = AllItems[Indices[i]];
		if (!StoreItem) continue;

		FItemPurchasePair Pair;
		Pair.ItemName = StoreItem->ItemName;
		Pair.Quantity = FMath::RandRange(MinQuantity, MaxQuantity);

		Inventory.Add(Pair);
	}

	return Inventory;
}

TArray<FStoreItem> UNPCFuncLibrary::GenerateSampleInventoryv2(
	UDataTable* ItemDataTable, 
	int32 NumItems, 
	int32 MinQuantity, 
	int32 MaxQuantity)
{
	TArray<FStoreItem> Inventory;

	if (!ItemDataTable) 
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemDataTable is null in UNPCFuncLibrary!"));
		return Inventory;
	}

	// Get all rows from the DataTable
	static const FString ContextString(TEXT("GenerateSampleInventory"));
	TArray<FStoreItem*> AllItems;
	ItemDataTable->GetAllRows(ContextString, AllItems);

	if (AllItems.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No rows in ItemDataTable!"));
		return Inventory;
	}

	// Clamp number of items
	NumItems = FMath::Clamp(NumItems, 1, AllItems.Num());

	// Shuffle indices
	TArray<int32> Indices;
	for (int32 i = 0; i < AllItems.Num(); i++) { Indices.Add(i); }
	Indices.Sort([](int32 A, int32 B){ return FMath::RandBool(); });

	for (int32 i = 0; i < NumItems; i++)
	{
		FStoreItem* StoreItem = AllItems[Indices[i]];
		if (!StoreItem) continue;

		FStoreItem Copy = *StoreItem;
		Copy.Quantity = FMath::RandRange(MinQuantity, MaxQuantity);

		Inventory.Add(Copy);
	}

	return Inventory;
}