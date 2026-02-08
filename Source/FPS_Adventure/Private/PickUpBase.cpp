// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpBase.h"
#include "Data/ItemDefinition.h"

// Sets default values
APickUpBase::APickUpBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APickUpBase::BeginPlay()
{
	Super::BeginPlay();
	
	InitializePickUp();
}

// Called every frame
void APickUpBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickUpBase::InitializePickUp()
{
	if (PickupDataTable && !PickupItemID.IsNone())
	{
		const FItemData* ItemDataRow = PickupDataTable->FindRow<FItemData>(PickupItemID, PickupItemID.ToString());

		ReferenceItem = NewObject<UItemDefinition>(this, UItemDefinition::StaticClass());

		ReferenceItem->ID = ItemDataRow->ID;
		ReferenceItem->ItemType = ItemDataRow->ItemType;
		ReferenceItem->ItemText = ItemDataRow->ItemText;
		ReferenceItem->WorldMesh = ItemDataRow->ItemBase->WorldMesh;
	}
}

