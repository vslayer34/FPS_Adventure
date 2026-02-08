// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpBase.h"
#include "Data/ItemDefinition.h"

// Sets default values
APickUpBase::APickUpBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	check(MeshComponent != nullptr);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	check(SphereComponent != nullptr);

	SphereComponent->SetupAttachment(MeshComponent);
	SphereComponent->SetSphereRadius(32.0f);
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
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("I'm Called"));
	if (PickupDataTable && !PickupItemID.IsNone())
	{
		const FItemData* ItemDataRow = PickupDataTable->FindRow<FItemData>(PickupItemID, PickupItemID.ToString());

		ReferenceItem = NewObject<UItemDefinition>(this, UItemDefinition::StaticClass());

		ReferenceItem->ID = ItemDataRow->ID;
		ReferenceItem->ItemType = ItemDataRow->ItemType;
		ReferenceItem->ItemText = ItemDataRow->ItemText;
		ReferenceItem->WorldMesh = ItemDataRow->ItemBase->WorldMesh;

		UItemDefinition* TempItemDefinition = ItemDataRow->ItemBase.Get();

		// Check if the mesh is currently loaded by calling IsValid().
		if (TempItemDefinition->WorldMesh.IsValid())
		{
			// Set the pickup's mesh to the associated item's mesh
			MeshComponent->SetStaticMesh(TempItemDefinition->WorldMesh.Get());
		}
		else
		{
			// If the mesh isn't loaded, load it by calling LoadSynchronous().
			UStaticMesh* WorldMesh = TempItemDefinition->WorldMesh.LoadSynchronous();
			MeshComponent->SetStaticMesh(WorldMesh);
		}

		MeshComponent->SetVisibility(true);
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		// Register the Overlap Event
		SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &APickUpBase::OnSphereBeginOverlap);
	}
}

void APickUpBase::OnSphereBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Attempting a pickup collision"));

	AAdventureCharacter* Character = Cast<AAdventureCharacter>(OtherActor);

	if (Character != nullptr)
	{
		// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Character is here"));

		FString VisibilityStatus = MeshComponent->IsVisible() ? TEXT("true") : TEXT("false");
		// Unregister from the Overlap Event so it is no longer triggered
		SphereComponent->OnComponentBeginOverlap.RemoveAll(this);

		MeshComponent->SetVisibility(false);

		VisibilityStatus = MeshComponent->IsVisible() ? TEXT("true") : TEXT("false");

		MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (bSholudRespawn)
	{
		GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &APickUpBase::InitializePickUp, RespawnTimer, false);

		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("I'm gonna respawn"));
	}
}

void APickUpBase::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent)
{
	// Handle parent class property changes
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName ChangedPropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	// Verify that the changed property exists in this class and that the PickupDataTable is valid.
	if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(APickUpBase, PickupItemID) && PickupDataTable)
	{
		// Retrieve the associated ItemData for this pickup.
		if (const FItemData* ItemDataRow = PickupDataTable->FindRow<FItemData>(PickupItemID, PickupItemID.ToString()))
		{
			UItemDefinition* TempItemDefinition = ItemDataRow->ItemBase.Get();
			MeshComponent->SetStaticMesh(TempItemDefinition->WorldMesh.Get());
			SphereComponent->SetSphereRadius(32.0f);
		}
	}
}
