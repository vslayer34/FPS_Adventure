// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SphereComponent.h"
#include "Characters/AdventureCharacter.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickUpBase.generated.h"

UCLASS(BlueprintType, Blueprintable)
class FPS_ADVENTURE_API APickUpBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickUpBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditInstanceOnly, Category="Pickup | Item Table")
	FName PickupItemID;

	UPROPERTY(EditInstanceOnly, Category="Pickup | Item Table")
	TSoftObjectPtr<UDataTable> PickupDataTable;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Initializes this pickup with values from the data table.
	void InitializePickUp();
};
