// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Cucumber.h"
#include "GameFramework/Actor.h"
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

private:
	TArray<class AActor*> cukes;
};