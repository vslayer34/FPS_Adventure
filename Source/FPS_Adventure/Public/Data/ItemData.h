// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemData.generated.h"

/**
 * 
 */

class UItemDefinition;

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Tool UMETA(DisplayName="Tool"),
	Consumable UMETA(DisplayName="Consumable")
};

// Contains textual data about the item.
USTRUCT(BlueprintType)
struct FItemText
{
	GENERATED_BODY()

	// The text name of the item
	UPROPERTY(EditAnywhere)
	FText Name;

	// the text description of the item
	UPROPERTY(EditAnywhere)
	FText Description;
};

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Item Data")
	FName ID;

	UPROPERTY(EditAnywhere, Category="Item Data")
	EItemType ItemType;

	UPROPERTY(EditAnywhere, Category="Item Data")
	FItemText ItemText;

	UPROPERTY(EditAnywhere, Category="Item Data")
	TSoftObjectPtr<UItemDefinition> ItemBase;
};