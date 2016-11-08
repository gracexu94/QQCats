// Fill out your copyright notice in the Description page of Project Settings.

#include "QQCats.h"
#include "Cat.h"


// Sets default values
ACat::ACat()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CatRootComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	CatRootComponent->SetCollisionProfileName(TEXT("OverlapAll"));
	CatRootComponent->SetNotifyRigidBodyCollision(true);

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
	//UE_LOG(LogTemp, Warning, TEXT("%d"), cukes.Num());

	CheckSurroundings();
}

void ACat::CheckSurroundings() {
	// get cat position
	FVector CatPos = GetActorLocation();
	//CatPos.Z += 1.0f;

	UWorld* const World = GetWorld();

	for (AActor* cuke : cukes) {
		FVector CukePos = cuke->GetActorLocation();
		// raycast towards every cucumber to see if it's "visible"
		FHitResult firstHit;
		if (World->LineTraceSingleByChannel(firstHit, CatPos, CukePos, ECC_Visibility)) {
			UE_LOG(LogTemp, Warning, TEXT("first hit name is %s"), *firstHit.GetActor()->GetName());
			if (cuke->GetName().Equals(firstHit.GetActor()->GetName())) {
				UE_LOG(LogTemp, Warning, TEXT("cucumber unobstructed"));
			}

		}


		//UE_LOG(LogTemp, Warning, TEXT("Cat Position: %s"), *CatPos.ToString());
		//UE_LOG(LogTemp, Warning, TEXT("Cuke Position: %s"), *CukePos.ToString());
		//UE_LOG(LogTemp, Warning, TEXT("%f"), FVector::Dist(CatPos, CukePos));
		/*
		float distance = FVector::Dist(CatPos, CukePos);
		if (distance < threshold) {
			FVector diff = CatPos - CukePos;
			//UE_LOG(LogTemp, Warning, TEXT("he jump"));
			
			float xforce = diff.X * 100.0f;
			float yforce = diff.Y * 100.0f;
			CatRootComponent->AddForce(FVector(xforce, yforce, 150000.0f));
		} */
	}

}