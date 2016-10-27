// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Cucumber.generated.h"

UCLASS()
class QQCATS_API ACucumber : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACucumber();

	// Called when the game starts or when spawned`
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UBoxComponent* CucumberBoxComponent;
	UMaterial* CucumberMaterial;
	
	void Drop();
};
