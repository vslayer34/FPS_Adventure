// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AdventureCharacter.h"

// Sets default values
AAdventureCharacter::AAdventureCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAdventureCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(GEngine != nullptr);

	// Get the player controller for this character
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		// Get the enhanced input local player subsystem and add a new input mapping context to it
		if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			EnhancedInputSubsystem->AddMappingContext(FirstPersonMappingContext, 0);
		}
	}
}

// Called every frame
void AAdventureCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAdventureCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Bind Movement Action
		EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &AAdventureCharacter::Move);

		// Bind Jump Actions
		EnhancedInputComponent->BindAction(JumpInputAction, ETriggerEvent::Started, this, &AAdventureCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpInputAction, ETriggerEvent::Completed, this, &AAdventureCharacter::StopJumping);
	}
}

// Handles 2D movement input
void AAdventureCharacter::Move(const FInputActionValue &Value)
{
	const FVector2D InputValue = Value.Get<FVector2D>();

	if (Controller)
	{
		const FVector Right = GetActorRightVector();
		const FVector Forward = GetActorForwardVector();

		AddMovementInput(Right, InputValue.X);
		AddMovementInput(Forward, InputValue.Y);
	}
}