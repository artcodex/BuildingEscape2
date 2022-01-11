// Copyright Michael Bridges 2019

#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"



#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	initialYaw = GetOwner()->GetActorRotation().Yaw;
	targetYaw += initialYaw; //TargetYaw = TargetYaw + InitialYaw;

	if(!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has the open door component on it, but no pressureplate set!"), *GetOwner()->GetName());	
	}

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	FindAudioComponent();
}

void UOpenDoor::FindAudioComponent() {
	audioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();

	if (!audioComponent) {
		UE_LOG(LogTemp, Error, TEXT("Cannot find audio component on %s"), *GetOwner()->GetName());
	}
}

 void UOpenDoor::PlayDoorSound(bool isOpening) {
	 if (audioComponent && isOpening != isDoorOpen) {
		 if (audioComponent->IsPlaying()) {
			 audioComponent->Stop();
		 }

		 audioComponent->Play();
	 }
 }


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	auto playerController = this->GetPlayerController();
	
	if (PressurePlate)
	{
		if (TotalMassOfActors() >= doorOpenMass) { 
			targetYaw = initialYaw + openYaw;
			currentSpeed = openSpeed;
			doorLastOpened = GetWorld()->GetTimeSeconds();

			PlayDoorSound(true);
			isDoorOpen = true;
		} else {
			float currentTime = GetWorld()->GetTimeSeconds();
			if (currentTime-doorLastOpened >= doorCloseDelay) {
				targetYaw = initialYaw;
				currentSpeed = closeSpeed;

				PlayDoorSound(false);
				isDoorOpen = false;
			}
		}
			
		SwingDoor(DeltaTime);
	}
}

float UOpenDoor::TotalMassOfActors() {
	float totalMass = 0.f;

	TArray<AActor*> overlappingActors;
	PressurePlate->GetOverlappingActors(OUT overlappingActors);

	for(AActor *actor : overlappingActors) {
		UPrimitiveComponent *primitiveComp = actor->FindComponentByClass<UPrimitiveComponent>();
		totalMass += primitiveComp->GetMass();
	}

	//UE_LOG(LogTemp, Warning, TEXT("Total mass is %.2f"), totalMass);

	return totalMass;
}

void UOpenDoor::SwingDoor(float DeltaTime)
{
	float currentYaw = GetOwner()->GetActorRotation().Yaw;

	if (FMath::Abs(currentYaw - targetYaw) > 0.001f) {
		float nextYaw = FMath::FInterpTo(currentYaw, targetYaw, DeltaTime, currentSpeed);

		FRotator newRotator = FRotator(0.f, nextYaw, 0.f);
		GetOwner()->SetActorRotation(newRotator);
	}
}

APlayerController* UOpenDoor::GetPlayerController() {
	UWorld* world = GetWorld();
	APlayerController* playerController = NULL;
	for (FConstPlayerControllerIterator Iterator = world->GetPlayerControllerIterator(); Iterator; ++Iterator) {
		playerController = Iterator->Get();
		break;
	}

	return playerController;
}