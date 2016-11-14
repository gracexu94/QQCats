// Fill out your copyright notice in the Description page of Project Settings.

#include "QQCats.h"
#include "Cat.h"


// Sets default values
ACat::ACat()
{
	timers.resize(6);

	timers[catTimers::AIR] = -1.0f;
	timers[catTimers::IDLE] = -1.0f;
	timers[catTimers::WALK] = -1.0f;
	timers[catTimers::TURN] = -1.0f;

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

	resetBoredom();
}

void ACat::updateTimers(float DeltaTime) {
	timers[catTimers::AIR] -= DeltaTime;
	timers[catTimers::IDLE] -= DeltaTime;
	timers[catTimers::WALK] -= DeltaTime;
	timers[catTimers::TURN] -= DeltaTime;
}

void ACat::resetBoredom() {
	timers[catTimers::IDLE] = this->durationIdleMax; // TODO randomize?
}

void ACat::updateCatBehaviors(float DeltaTime) {
	// update position/orientation based on if there's still a walk/turn behavior going
	if (timers[catTimers::WALK] > 0.0f) {
		step(DeltaTime);
	}
	else if (timers[catTimers::TURN] > 0.0f) {
		turn(DeltaTime);
	}

	// launch a new behavior if the cat's idle
	else if (timers[catTimers::IDLE] < 0.0f) {
		// decide on a new behavior for the cat by setting the appropriate timer.
		UE_LOG(LogTemp, Warning, TEXT("cat got bored"));

		float random = (float )rand() / (float)RAND_MAX;
		if (random < 0.5f) {
			timers[catTimers::WALK] = this->walkMax;
			PlayMontage(walkMontage);
		}
		if (0.5f <= random && random < 0.75f) {
			timers[catTimers::TURN] = this->turnMax;
			turnLeft = true;
			PlayMontage(leftTurnMontage);
		}
		if (0.75f <= random) {
			timers[catTimers::TURN] = this->turnMax;
			turnLeft = false;
			PlayMontage(rightTurnMontage);
		}
	}
}

void ACat::step(float DeltaTime) {
	resetBoredom(); // don't allow return to idle

	// check if getting ready to stop
	if (timers[catTimers::WALK] < DeltaTime) {
		PlayMontage(tailWagMontage);
		timers[catTimers::IDLE] = 0.0f;
	}
	else {
		// walk forward aimlessly
		FVector forward = GetActorForwardVector() * walkStepSize * DeltaTime;
		SetActorLocation(forward + GetActorLocation());
	}
}

void ACat::turn(float DeltaTime) {
	resetBoredom();

	// check if getting ready to stop
	if (timers[catTimers::TURN] < DeltaTime) {
		PlayMontage(tailWagMontage);
		timers[catTimers::IDLE] = 0.0f;
	}
	else {
		// turn aimlessly
		FRotator oldRotation = this->GetActorRotation();

		FRotator correctedRotation;
		float turnStep = turnStepSize * DeltaTime;
		correctedRotation.Yaw = turnLeft ? oldRotation.Yaw - turnStep : oldRotation.Yaw + turnStep;
		this->SetActorRotation(correctedRotation); // this works fine with friction, gravity
	}
}

// Called every frame
void ACat::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	updateTimers(DeltaTime);
	updateCatBehaviors(DeltaTime);

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
		FVector rotationAxis = FVector::CrossProduct(characterUp, worldUp);
		rotationAxis.Normalize();
		
		FRotator oldRotation = this->GetActorRotation();
		oldRotation.Pitch = 0.0f;
		oldRotation.Roll = 0.0f;

		FQuat rotationQuaternion = oldRotation.Quaternion();
		FQuat rotateToAngle = FQuat(rotationAxis, -this->catMaxTipAngle);
		rotationQuaternion = rotateToAngle * rotationQuaternion;

		this->SetActorRotation(rotationQuaternion);
	}
}

void ACat::CheckSurroundings() {
	// get cat position

	FVector CatPos = this->GetActorLocation();
	FVector CatEyePos = CatPos + this->GetActorRotation().RotateVector(cucumberTargetOffset);

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

				// jump!

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

					// set AIR timer so we can detect timeout
					// immediately deplete behavior timers
					resetBoredom();
					timers[catTimers::AIR] = this->durationAirMax;
					timers[catTimers::WALK] = -1.0f;
					timers[catTimers::TURN] = -1.0f;
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
			resetBoredom();

			UE_LOG(LogTemp, Warning, TEXT("landed"));
			PlayMontage(tailWagMontage);
		}
	}
	else if (timers[catTimers::AIR] < 0.0f) {
		isLanded = true;
		resetBoredom();
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