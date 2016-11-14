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

	// Get the SkeletalMesh so we can play animations
	TArray<USkeletalMeshComponent*> Components;
	this->GetComponents<USkeletalMeshComponent>(Components);
	for (int32 i = 0; i<Components.Num(); i++)
	{
		SkeletalMeshComponent = Components[i];
		break;
	}
}

// Called every frame
void ACat::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	// get cucumbers 
	// TODO: only do this when new cukes are spawned
	TSubclassOf<ACucumber> ClassToFind = ACucumber::StaticClass();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind, cukes);

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
		// rotate the cat so that it's angle to "up" is within catMaxTipAngle 
		//FVector rotationAxis = FVector::CrossProduct(characterUp, worldUp);
		//FQuat correctionRotation = FQuat(rotationAxis, this->catMaxTipAngle);

		FRotator oldRotation = this->GetActorRotation();

		FRotator correctedRotation;
		correctedRotation.Yaw = oldRotation.Yaw;
		correctedRotation.Pitch = 0.0f;
		correctedRotation.Roll = 0.0f;

		//FQuat correctedRotation = correctionRotation * oldRotation.Quaternion();

		this->SetActorRotation(correctedRotation);
	}
}

void ACat::CheckSurroundings() {
	// get cat position

	FVector CatPos = this->GetActorLocation();
	FVector CatEyePos = CatPos + this->GetActorRotation().RotateVector(cucumberTargetOffset);

	//CatPos.Z += 1.0f;

	UWorld* const World = GetWorld();

	for (AActor* cuke : cukes) {
		FVector CukePos = cuke->GetActorLocation();
		// raycast towards every cucumber to see if it's "visible"
		FHitResult firstHit;
		if (World->LineTraceSingleByChannel(firstHit, CatEyePos, CukePos, ECC_Visibility)) {
			//UE_LOG(LogTemp, Warning, TEXT("first hit name is %s"), *firstHit.GetActor()->GetName());
			if (cuke->GetName().Equals(firstHit.GetActor()->GetName())) {
				//UE_LOG(LogTemp, Warning, TEXT("cucumber unobstructed"));
				//UE_LOG(LogTemp, Warning, TEXT("Cat Position: %s"), *CatPos.ToString());
				//UE_LOG(LogTemp, Warning, TEXT("Cuke Position: %s"), *CukePos.ToString());
				//UE_LOG(LogTemp, Warning, TEXT("%f"), FVector::Dist(CatPos, CukePos));

				float distance = FVector::Dist(CatPos, CukePos);
				if (distance < threshold) {
					FVector diff = CatPos - CukePos;
					
					float xdir = diff.X * horizontalImpulseScale;
					float ydir = diff.Y * horizontalImpulseScale;
					float zdir = verticalImpulse;
					FVector dir = FVector(xdir, ydir, zdir);

					CatRootComponent->AddImpulse(dir);
					isLanded = false;
					PlayMontage(flyingWiggleMontage);

					UE_LOG(LogTemp, Warning, TEXT("added impulse"));

					// rotate cat along UP axis to look at cucumber
					FVector toCucumberDirection = CukePos - CatPos;
					toCucumberDirection.Z = 0.0;
					toCucumberDirection.Normalize();

					FVector catForward = this->GetActorForwardVector();

					FVector rotationAxis = FVector::CrossProduct(catForward, toCucumberDirection);

					float angleBetween = acosf(FVector::DotProduct(catForward, toCucumberDirection)) * 57.29577951f; // radians to degrees

					FRotator oldRotation = this->GetActorRotation();
					if (rotationAxis.Z > 0.0f)
						oldRotation.Yaw += angleBetween;
					else
						oldRotation.Yaw -= angleBetween;

					this->SetActorRotation(oldRotation);

				}
			}
		}
		
	}

}

// Raycasts in the cat down position to see if the cat has landed
void ACat::CheckAirborne() {
	// get cat position
	FVector FeetPos = GetActorLocation();

	// get position below the cat's feet
	FVector CatPos = FeetPos + checkGroundDistance * GetActorUpVector();

	UWorld* const World = GetWorld();

	FHitResult firstHit;
	if (World->LineTraceSingleByChannel(firstHit, CatPos, FeetPos, ECC_WorldStatic)) {
		UE_LOG(LogTemp, Warning, TEXT("first hit name is %s"), *firstHit.GetActor()->GetName());
		// Hit Something that isn't this
		// TODO: Delete else and negate this condition, but for now just check
		if (firstHit.GetActor()->GetName() != this->GetName()) {
			isLanded = true;

			UE_LOG(LogTemp, Warning, TEXT("landed"));
			PlayMontage(tailWagMontage);
		}
	}
	else {
		isLanded = false;
	}

}

void ACat::PlayMontage(UAnimMontage *montage) {
	if (montage->IsValidLowLevel())
	{
		if (this->SkeletalMeshComponent)
		{
			UAnimInstance *AnimInst = this->SkeletalMeshComponent->GetAnimInstance();
			if (AnimInst)
			{
				AnimInst->Montage_Play(montage);
			}
		}
	}
}