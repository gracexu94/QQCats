// Fill out your copyright notice in the Description page of Project Settings.

#include "QQCats.h"
#include "Cucumber.h"


// Sets default values
ACucumber::ACucumber()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Our root component will be a box that reacts to physics  
	CucumberBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));  
	RootComponent = CucumberBoxComponent;  CucumberBoxComponent->InitBoxExtent(FVector(12.0f, 15.0f, 12.0f)); 
	CucumberBoxComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));  
	CucumberBoxComponent->SetSimulatePhysics(true); 
	CucumberBoxComponent->SetEnableGravity(true); 
	CucumberBoxComponent->SetNotifyRigidBodyCollision(true);

}

// Called when the game starts or when spawned
void ACucumber::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACucumber::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

