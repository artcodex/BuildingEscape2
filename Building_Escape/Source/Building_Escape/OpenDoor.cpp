// Copyright Michael Bridges 2019

#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "OpenDoor.h"
#include "GameFramework/Actor.h"


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
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	auto playerController = this->GetPlayerController();
	
	if (PressurePlate)
	{
		if (PressurePlate->IsOverlappingActor(ActorThatOpens) || playerController->WasInputKeyJustPressed(EKeys::O)) {
			targetYaw = initialYaw + openYaw;
			currentSpeed = openSpeed;
			doorLastOpened = GetWorld()->GetTimeSeconds();
		} else {
			float currentTime = GetWorld()->GetTimeSeconds();
			if (currentTime-doorLastOpened >= doorCloseDelay) {
				targetYaw = initialYaw;
				currentSpeed = closeSpeed;
			}
		}
			
		SwingDoor(DeltaTime);
	}
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