// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "AdventureCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class UInputComponent;
class UCameraComponent;
class UAnimBlueprint;

UCLASS()
class FPS_ADVENTURE_API AAdventureCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAdventureCharacter();

	UPROPERTY(EditAnywhere, Category="Animation")
	TObjectPtr<UAnimBlueprint> FirstPersonDefaultAnim;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputMappingContext> FirstPersonMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> MoveInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> JumpInputAction;

	// Look Input Actions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> LookInputAction;

	// First Person Camera
	UPROPERTY(VisibleAnywhere, Category="Camera")
	TObjectPtr<UCameraComponent> CameraComponent;

	// Offset for the first camera component
	UPROPERTY(EditAnywhere, Category="Camera")
	FVector FirstPersonCameraOffset = FVector(15.0f, 12.0f, 0.0f);

	// First-person primitives field of view
	UPROPERTY(EditAnywhere, Category="Camera")
	float FirstPersonFOV = 70.0f;

	// First-person primitives view scale
	UPROPERTY(EditAnywhere, Category="Camera")
	float FirstPersonScale = 0.6f;

	// First-person mesh, visible only to the owning player
	UPROPERTY(VisibleAnywhere, Category="Mesh")
	TObjectPtr<USkeletalMeshComponent> FirstPersonMeshComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Handles 2D movement input
	UFUNCTION()
	void Move(const FInputActionValue &Value);

	// Handles look input
	void Look(const FInputActionValue &Value);
};
