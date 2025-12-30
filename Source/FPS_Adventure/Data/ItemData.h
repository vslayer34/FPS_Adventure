// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemData.generated.h"


class UItemDefinition;

// Defines the type of the item.
UENUM()
enum class EItemType : uint8
{
	Tool UMETA(DisplayName = "Tool"),
	Consumable UMETA(DisplayName = "Consumable")
};

// Contains textual data about the item.
USTRUCT()
struct FItemText
{
	GENERATED_BODY();

	// The text name of the item.
	UPROPERTY()
};