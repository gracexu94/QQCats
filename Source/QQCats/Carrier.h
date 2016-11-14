// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Cat.h"
#include "GameFramework/Actor.h"
#include "Carrier.generated.h"

UCLASS()
class QQCATS_API ACarrier : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACarrier();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UBoxComponent* AsteroidBoxComponent;

	/** Replacement blueprint class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
		TSubclassOf<class AActor> ReplacementClass;
	
	UFUNCTION() void onHit(AActor* SelfActor, class AActor* OtherActor,
		FVector NormalImpulse, const FHitResult& Hit);
};
