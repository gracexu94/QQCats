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
	virtual void Tick( float DeltaSeconds ) override;

	void CheckSurroundings();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* CatRootComponent;
	
	UMaterial* CatMaterial;
	
private:
	TArray<class AActor*> cukes; 
	float threshold = 400.0;
};
