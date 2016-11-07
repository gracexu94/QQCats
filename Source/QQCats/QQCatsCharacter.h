// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "Cat.h"
#include "Cucumber.h"
#include "GameFramework/Character.h"
#include "QQCatsCharacter.generated.h"

class UInputComponent;

UCLASS(config=Game)
class AQQCatsCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	class USkeletalMeshComponent* Mesh1P;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* FP_MuzzleLocation;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;
public:
	AQQCatsCharacter();

	virtual void BeginPlay();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector GunOffset;

	/** Vector defining how far a cucumber can be spawned  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector CucumberRange;

	/** float defining how far "out" from a surface a cucumber should spawn  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float CucumberNormalOffset;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class AActor> ProjectileClass;

	/** Sound to play each time we fire */
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	//class USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	//class UAnimMontage* FireAnimation;



protected:

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);


	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Raycasts cucumber into scene
	void DropCucumber();

	// Find all cats and cucumbers in the scene
	TArray<AActor*> FindCats();
	TArray<AActor*> FindCucumbers();

	// Checks if cat can see cucumber
	ACucumber* isCatSeesCucumber();

	// Cats and cucumbers
	TArray<AActor*> cats;
	TArray<AActor*> cucumbers;

	// Visible Distance 
	float visDistance = 400.0;
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

