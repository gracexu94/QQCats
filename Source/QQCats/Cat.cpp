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

	isLanded = true;
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

	SelfRight();

	if (isLanded) {
		CheckSurroundings();  // Look for cucumbers
	}
	else {
		// Check if landed
		CheckAirborne();
	}
}

void ACat::SelfRight() {
	FVector characterUp = this->GetActorUpVector();
	FVector worldUp = FVector::UpVector;

	float angleBetween = acosf(FVector::DotProduct(characterUp, worldUp));
	if (angleBetween > this->catMaxTipAngle) {
		angleBetween *= 57.29577951f; // radians to degrees...?
		// rotate the cat so that it's angle to "up" is within catMaxTipAngle 
		FVector rotationAxis = FVector::CrossProduct(characterUp, worldUp);
		FQuat correctionRotation = FQuat(rotationAxis, this->catMaxTipAngle);

		FRotator oldRotation = this->GetActorRotation();
		FQuat correctedRotation = correctionRotation * oldRotation.Quaternion();

		this->SetActorRotation(correctedRotation);
	}
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
			//UE_LOG(LogTemp, Warning, TEXT("first hit name is %s"), *firstHit.GetActor()->GetName());
			if (cuke->GetName().Equals(firstHit.GetActor()->GetName())) {
				//UE_LOG(LogTemp, Warning, TEXT("cucumber unobstructed"));
				//UE_LOG(LogTemp, Warning, TEXT("Cat Position: %s"), *CatPos.ToString());
				//UE_LOG(LogTemp, Warning, TEXT("Cuke Position: %s"), *CukePos.ToString());
				//UE_LOG(LogTemp, Warning, TEXT("%f"), FVector::Dist(CatPos, CukePos));

				float distance = FVector::Dist(CatPos, CukePos);
				if (distance < threshold) {
					FVector diff = CatPos - CukePos;
					// UE_LOG(LogTemp, Warning, TEXT("he jump"));

					//float xforce = diff.X * 1000.0f;
					//float yforce = diff.Y * 1000.0f;
					//CatRootComponent->AddForce(FVector(xforce, yforce, 15000000.0f));
					
					float xdir = diff.X * 100.0f;
					float ydir = diff.X * 100.0f;
					float zdir = 100000;
					FVector dir = FVector(xdir, ydir, zdir);
					// dir.Normalize();

					CatRootComponent->AddImpulse(dir);
					UE_LOG(LogTemp, Warning, TEXT("added impulse"));

					isLanded = false;

					// rotate cat along UP axis to look at cucumber
					FVector toCucumberDirection = CukePos - CatPos;
					toCucumberDirection.Z = 0.0;
					toCucumberDirection.Normalize();

					FVector catForward = this->GetActorForwardVector();

					FVector rotationAxis = FVector::CrossProduct(catForward, toCucumberDirection);

					float angleBetween = acosf(FVector::DotProduct(catForward, toCucumberDirection)) * 57.29577951f; // radians to degrees

					FRotator oldRotation = this->GetActorRotation();
					oldRotation.Yaw += angleBetween * rotationAxis.Z;

					this->SetActorRotation(oldRotation);

				}
			}
		}
		
	}

}

// Raycasts in the cat down position to see if the cat has landed
void ACat::CheckAirborne() {
	// get cat position
	FVector CatPos = GetActorLocation();

	// get position below the cat's feet
	FVector FeetPos = CatPos + -1.0 * GetActorUpVector();

	UWorld* const World = GetWorld();

	FHitResult firstHit;
	if (World->LineTraceSingleByChannel(firstHit, CatPos, FeetPos, ECC_Visibility)) {
		UE_LOG(LogTemp, Warning, TEXT("first hit name is %s"), *firstHit.GetActor()->GetName());
		// Hit Something
		// TODO: Delete else and negate this condition, but for now just check
		isLanded = true;

		UE_LOG(LogTemp, Warning, TEXT("landed"));
	}
	else {
		isLanded = false;
	}

}