
#pragma once

#include "CoreMinimal.h"
#include "HavenEnums.generated.h"


UENUM(BlueprintType)
enum class ETimeOfDay : uint8 {
	Morning = 0	UMETA(DisplayName = "Morning"),
	Afternoon  = 1	UMETA(DisplayName = "Afternoon"),
	Night  = 2	UMETA(DisplayName = "Night"),
};

UENUM(BlueprintType)
enum class EFoodCategory : uint8 {
	None = 0       UMETA(DisplayName = "None"),
	Meal = 1       UMETA(DisplayName = "Meal"),
	Snack = 2       UMETA(DisplayName = "Snack"),
	Drink = 3       UMETA(DisplayName = "Drink"),
	Dessert = 4     UMETA(DisplayName = "Dessert"),
	Coffee = 5      UMETA(DisplayName = "Coffee"), // special drink
	Ingredient = 6     UMETA(DisplayName = "Ingredient"),
};

UENUM(BlueprintType)
enum class EBuildingType : uint8 {
	None = 0			UMETA(DisplayName = "None"),
	Haven = 1	UMETA(DisplayName = "Haven"),
	House  = 2	UMETA(DisplayName = "House"),
	TownHall  = 3	UMETA(DisplayName = "TownHall"),
	Smithy  = 4	UMETA(DisplayName = "Smithy"),
	Hospital  = 5	UMETA(DisplayName = "Hospital"),
	Market  = 6	UMETA(DisplayName = "Market"),
	Storage = 7	UMETA(DisplayName = "Storage"),
	Tavern = 8	UMETA(DisplayName = "Tavern"),
};

UENUM(BlueprintType)
enum class EDroneTask : uint8 {
	None = 0			UMETA(DisplayName = "None"),
	Cleaning = 1	UMETA(DisplayName = "Cleaning"),
	Supplying = 2			UMETA(DisplayName = "Supplying"),
	Farming = 3			UMETA(DisplayName = "Farming"),
	Delivery = 4			UMETA(DisplayName = "Delivery"),
	Scavenging = 5			UMETA(DisplayName = "Scavenging"),
};

UENUM(BlueprintType)
enum class EOccupationType : uint8 {
	None = 0			UMETA(DisplayName = "None"),
	Construction = 1	UMETA(DisplayName = "Construction"),
	Miner = 2			UMETA(DisplayName = "Miner"),
	Government = 3			UMETA(DisplayName = "Government"),
	Vendor = 4			UMETA(DisplayName = "Vendor"),
	Scavenger = 5			UMETA(DisplayName = "Scavenger"),
	Civilian = 6			UMETA(DisplayName = "Civilian"),
};

// Needs
UENUM(BlueprintType)
enum class ENPCNeedType : uint8 {
	None     = 0 UMETA(DisplayName="None"),
	Hungry   = 1 UMETA(DisplayName="Hungry"),
	Thirsty  = 2 UMETA(DisplayName="Thirsty"),
	Tired    = 3 UMETA(DisplayName="Tired")
};

// Moods
UENUM(BlueprintType)
enum class ENPCMoodType : uint8 {
	None        = 0 UMETA(DisplayName="None"),
	Cheerful    = 1 UMETA(DisplayName="Cheerful"),   // happy / sociable
	Irritable   = 2 UMETA(DisplayName="Irritable"),  // grumpy / stressed
	Dissociate  = 3 UMETA(DisplayName="Dissociate"), // withdrawn
	Curious     = 4 UMETA(DisplayName="Curious")     // exploratory
};

UENUM(BlueprintType)
enum class ENPCType : uint8 {
	None = 0		UMETA(DisplayName = "None"),
	Regulars = 1	UMETA(DisplayName = "Regulars"),
	Townsfolk  = 2	UMETA(DisplayName = "Townsfolk"),
	Insider  = 3	UMETA(DisplayName = "Insider"),
	Outsider  = 4	UMETA(DisplayName = "Outsider"),
	Roamer  = 5	UMETA(DisplayName = "Roamer"),
};
