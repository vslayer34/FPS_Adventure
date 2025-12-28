// Fill out your copyright notice in the Description page of Project Settings.


#include "AdventureCharacter.h"

// Sets default values
AAdventureCharacter::AAdventureCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	check(CameraComponent != nullptr);

	FpsMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("FirstPersonMesh");
	check(FpsMeshComponent != nullptr);

	// Attach the FirstPerson mesh to the Skeletal Mesh
	FpsMeshComponent->SetupAttachment(GetMesh());

	FpsMeshComponent->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

	CameraComponent->SetupAttachment(FpsMeshComponent, FName("head"));

	// Position the camera slightly above the eyes and rotate it to behind the player's head
	CameraComponent->SetRelativeLocationAndRotation(FpsCameraOffset, FRotator(0.0f, 90.0f, -90.0f));

	// Enable the pawn to control camera rotation.
	CameraComponent->bUsePawnControlRotation = true;

	// Enable first-person rendering and set default FOV and scale values
	CameraComponent->bEnableFirstPersonFieldOfView = true;
	CameraComponent->bEnableFirstPersonScale = true;
	CameraComponent->FirstPersonFieldOfView = FpsCameraFov;
	CameraComponent->FirstPersonScale = FpsScale;
}

// Called when the game starts or when spawned
void AAdventureCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	check(GEngine != nullptr);

	// Only the owning player sees the first-person mesh
	FpsMeshComponent->SetOnlyOwnerSee(true);

	// Get the player controller for this character
	if (APlayerController* playerController = Cast<APlayerController>(Controller))
	{
		// Get the enhanced input local player subsystem and add a new input mapping context to it
		if (UEnhancedInputLocalPlayerSubsystem* subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer()))
		{
			subSystem->AddMappingContext(FirstPersonContext, 0);
		}
	}

	GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Cyan, TEXT("I'm using the custom character class"));
}

// Called every frame
void AAdventureCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAdventureCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* enhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Bind Movement action
		enhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAdventureCharacter::Move);

		// Bind Jump action
		enhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		enhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::Jump);

		// Bind look input
		enhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AAdventureCharacter::Look);
	}
}


void AAdventureCharacter::Move(const FInputActionValue& value)
{
	// 2D Vector of movement values returned from the input action
	const FVector2D movementValue = value.Get<FVector2D>();

	// Check if the controller possessing this Actor is valid
	if (Controller)
	{
		const FVector rightVector = GetActorRightVector();
		const FVector forwardVector = GetActorForwardVector();

		AddMovementInput(rightVector, movementValue.X);
		AddMovementInput(forwardVector, movementValue.Y);
	}
}

void AAdventureCharacter::Look(const FInputActionValue& value)
{
	const FVector2D LookAxisValue = value.Get<FVector2D>();

	if (Controller)
	{
		AddControllerYawInput(LookAxisValue.X);
		AddControllerPitchInput(LookAxisValue.Y);
	}
}

