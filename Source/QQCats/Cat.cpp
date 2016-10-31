// Fill out your copyright notice in the Description page of Project Settings.

#include "QQCats.h"
#include "Cat.h"


// Sets default values
ACat::ACat()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CatRootComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));

	RootComponent = CatRootComponent;

}

// Called when the game starts or when spawned
void ACat::BeginPlay()
{
	Super::BeginPlay();
	CatRootComponent->InitSphereRadius(25.0f);
	CatRootComponent->SetCollisionProfileName(TEXT("Actor"));
	CatRootComponent->SetEnableGravity(true);
	CatRootComponent->SetSimulatePhysics(true);


}

// Called every frame
void ACat::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	// get cucumbers 
	// TODO: only do this when new cukes are spawned
	TSubclassOf<ACucumber> ClassToFind = ACucumber::StaticClass();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind, cukes);
	UE_LOG(LogTemp, Warning, TEXT("%d"), cukes.Num());

	CheckSurroundings();
}

void ACat::CheckSurroundings() {
	// get cat position
	FVector CatPos = GetActorLocation();

	for (AActor* cuke : cukes) {
		FVector CukePos = cuke->GetActorLocation();

		UE_LOG(LogTemp, Warning, TEXT("Cat Position: %s"), *CatPos.ToString());
		UE_LOG(LogTemp, Warning, TEXT("Cuke Position: %s"), *CukePos.ToString());
		UE_LOG(LogTemp, Warning, TEXT("%f"), FVector::Dist(CatPos, CukePos));

		if (FVector::Dist(CatPos, CukePos) < 400.0) {
			UE_LOG(LogTemp, Warning, TEXT("he jump"));

			CatRootComponent->AddForce(FVector(0.0f, 0.0f, 500000.0f));
		}
	}

}