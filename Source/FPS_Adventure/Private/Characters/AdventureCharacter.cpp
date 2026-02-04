// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AdventureCharacter.h"

// Sets default values
AAdventureCharacter::AAdventureCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create first person camera component
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPErsonCamera"));
	check(CameraComponent != nullptr);

	// Create first person mesh componet
	FirstPersonMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("FirstPersonMesh");
	check(FirstPersonMeshComponent != nullptr);

	// Attach the first-person mesh to the third-person mesh
	FirstPersonMeshComponent->SetupAttachment(GetMesh());

	// Include the first-person mesh in First Person Rendering so camera FOV and scale settings apply to the mesh  
	FirstPersonMeshComponent->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;

	// Set the first person mesh to not collide with other objects
	FirstPersonMeshComponent->SetCollisionProfileName(FName("NoCollision"));

	// Treat the 3rd-person mesh as a regular world object
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

	// Attach the camera component to the first-person Skeletal Mesh.
	CameraComponent->SetupAttachment(FirstPersonMeshComponent, FName("head"));

	// Position the camera slightly above the eyes and rotate it to behind the player's head
	CameraComponent->SetRelativeLocationAndRotation(FirstPersonCameraOffset, FRotator(0.0f, 90.0f, -90.0f));

	// Enable the pawn to control camera rotation.
	CameraComponent->bUsePawnControlRotation = true;

	// Enable first-person rendering and set default FOV and scale values
	CameraComponent->bEnableFirstPersonFieldOfView = true;
	CameraComponent->FirstPersonFieldOfView = FirstPersonFOV;
	CameraComponent->bEnableFirstPersonScale = true;
	CameraComponent->FirstPersonScale = FirstPersonScale;
}

// Called when the game starts or when spawned
void AAdventureCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(GEngine != nullptr);
	// Set the animations on the first person mesh.
	FirstPersonMeshComponent->SetAnimInstanceClass(FirstPersonDefaultAnim->GeneratedClass);
	// Set the animations on the third-person mesh.
	GetMesh()->SetAnimInstanceClass(FirstPersonDefaultAnim->GeneratedClass);

	// Only the owning player sees the first-person mesh. Hide the 3rd-person mesh from the owning player
	FirstPersonMeshComponent->SetOnlyOwnerSee(true);
	GetMesh()->SetOwnerNoSee(true);

	// Make the 3rd-person mesh cast a shadow
	GetMesh()->CastShadow = true;
	GetMesh()->bCastHiddenShadow = true;

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

		// Bind Look Actions
		EnhancedInputComponent->BindAction(LookInputAction, ETriggerEvent::Triggered, this, &AAdventureCharacter::Look);
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

void AAdventureCharacter::Look(const FInputActionValue &Value)
{
	const FVector LookInputValue = Value.Get<FVector>();

	if (Controller)
	{
		AddControllerYawInput(LookInputValue.X);
		AddControllerPitchInput(LookInputValue.Y);
	}
}