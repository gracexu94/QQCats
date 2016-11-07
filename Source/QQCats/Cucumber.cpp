// Fill out your copyright notice in the Description page of Project Settings.
#include "QQCats.h"
#include "Cucumber.h"


// Sets default values
ACucumber::ACucumber()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CucumberRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = CucumberRootComponent;
	// Our root component will be a box that reacts to physics  
	/*CucumberBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));  
	RootComponent = CucumberBoxComponent;  
	CucumberBoxComponent->InitBoxExtent(FVector(12.0f, 15.0f, 12.0f)); 
	CucumberBoxComponent->SetCollisionProfileName(TEXT("NoCollision"));  
	CucumberBoxComponent->SetSimulatePhysics(false); 
	CucumberBoxComponent->SetEnableGravity(false); 
	CucumberBoxComponent->SetNotifyRigidBodyCollision(false);*/
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