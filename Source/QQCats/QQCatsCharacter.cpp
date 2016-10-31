// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "QQCats.h"
#include "QQCatsCharacter.h"
#include "QQCatsProjectile.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/InputSettings.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AQQCatsCharacter

AQQCatsCharacter::AQQCatsCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

	// Note: The ProjectileClass is set in the
	// derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void AQQCatsCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

//////////////////////////////////////////////////////////////////////////
// Input

void AQQCatsCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// set up gameplay key bindings
	check(InputComponent);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	InputComponent->BindAxis("MoveForward", this, &AQQCatsCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AQQCatsCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &AQQCatsCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &AQQCatsCharacter::LookUpAtRate);


	InputComponent->BindAction("DropCucumber", IE_Pressed, this, &AQQCatsCharacter::DropCucumber);
}

void AQQCatsCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AQQCatsCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AQQCatsCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AQQCatsCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AQQCatsCharacter::DropCucumber() {
	// try and fire a projectile
	UWorld* const World = GetWorld();
	if (ProjectileClass != NULL && World != NULL)
	{
		/* Raycast into the world */
		// compute ray start/end position
		const FRotator SpawnRotation = GetControlRotation();
		const FVector rayStart = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);
		const FVector rayEnd = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(CucumberRange);
		
		/*
		UE_LOG(LogTemp, Warning, TEXT("Raystart is %s"), *rayStart.ToString());
		UE_LOG(LogTemp, Warning, TEXT("RayEnd is %s"), *rayEnd.ToString());
		*/

		FHitResult firstHit;
		if (!World->LineTraceSingleByChannel(firstHit, rayStart, rayEnd, ECC_Visibility)) {
			//UE_LOG(LogTemp, Warning, TEXT("no hit!"));
			return;
		}

		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
		const FVector SpawnLocation = rayStart + (rayEnd - rayStart) * firstHit.Time + firstHit.Normal * CucumberNormalOffset;
		/*
		UE_LOG(LogTemp, Warning, TEXT("SpawnLocation is %s"), *SpawnLocation.ToString());
		UE_LOG(LogTemp, Warning, TEXT("t to collision is %f"), firstHit.Time);
		UE_LOG(LogTemp, Warning, TEXT("Character location is %s"), *GetActorLocation().ToString());
		UE_LOG(LogTemp, Warning, TEXT("Offset is %s"), *SpawnRotation.RotateVector(GunOffset).ToString()); */

		// spawn the projectile at the muzzle
		World->SpawnActor<AActor>(ProjectileClass, SpawnLocation, SpawnRotation);
	} 
}
