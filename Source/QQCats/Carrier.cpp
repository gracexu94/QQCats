// Fill out your copyright notice in the Description page of Project Settings.

#include "QQCats.h"
#include "Carrier.h"


// Sets default values
ACarrier::ACarrier()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	AsteroidBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
	RootComponent = AsteroidBoxComponent;
	AsteroidBoxComponent->InitBoxExtent(FVector(80.0f, 100.0f, 70.0f));
	AsteroidBoxComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	AsteroidBoxComponent->SetSimulatePhysics(true);
	AsteroidBoxComponent->SetEnableGravity(false);
	AsteroidBoxComponent->SetNotifyRigidBodyCollision(true);
	OnActorHit.AddDynamic(this, &ACarrier::onHit);
}

// Called when the game starts or when spawned
void ACarrier::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACarrier::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void ACarrier::onHit(AActor* SelfActor, class AActor* OtherActor, FVector NormalImpulse,
	const FHitResult& Hit) {


	UE_LOG(LogTemp, Warning, TEXT("something hit the carrier, is %s"), *OtherActor->GetName());


	if (OtherActor && (OtherActor != this) && OtherActor->IsA(ACat::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("cat"));
		Destroy();
		OtherActor->Destroy();
		UWorld* const World = GetWorld();
		if (World) {
			UE_LOG(LogTemp, Warning, TEXT("YOU WIN!! YOU ARE SMART!"));
			const FVector SpawnLocation = GetActorLocation();
			const FRotator SpawnRotation = GetActorRotation();

			World->SpawnActor<AActor>(ReplacementClass, SpawnLocation, SpawnRotation);
		}
	}
}
