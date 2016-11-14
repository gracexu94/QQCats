// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "vector"

#include "Cucumber.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Animation/AnimMontage.h"

#include "Cat.generated.h"

UCLASS()
class QQCATS_API ACat : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACat();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	void SelfRight();
	void CheckSurroundings(); // Checks for cucumbers
	void CheckAirborne(); // Raycasts down to see if the cat has landed

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float catMaxTipAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USphereComponent* CatRootComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float checkGroundDistance;

	UMaterial* CatMaterial;

	bool isLanded;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector cucumberTargetOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float threshold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float horizontalImpulseScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float verticalImpulse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		UAnimMontage *tailWagMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		UAnimMontage *flyingWiggleMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		UAnimMontage *walkMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		UAnimMontage *rightTurnMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		UAnimMontage *leftTurnMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Behavior)
		float durationAirMax; // maximum in-air time, for getting around ground detect bugs

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Behavior)
		float durationIdleMax; // maximum time that a cat can be idle

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Behavior)
		float walkMax; // maximum time that a cat can be walking

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Behavior)
		float turnMax; // maximum time that a cat can be turning

private:

	USkeletalMeshComponent* SkeletalMeshComponent;

	void PlayMontage(UAnimMontage *montage);

	void updateTimers(float DeltaTime);

	void updateCatBehaviors(float DeltaTime);

	void step(float DeltaTime);

	void turn(float DeltaTime);
	bool turnLeft;

	void resetBoredom();

	TArray<class AActor*> cukes;

	enum catTimers {
		AIR = 0,
		IDLE = 1,
		WALK = 2,
		TURN = 3
	};

	std::vector<float> timers;
};