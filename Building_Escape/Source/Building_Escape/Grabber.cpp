// Copyright Michael Bridges 2019

#include "Grabber.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();
	SetupInputComponent();
}

void UGrabber::SetupInputComponent() {
	inputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (inputComponent) {
		inputComponent->BindAction("Grab", EInputEvent::IE_Pressed, this, &UGrabber::GrabPressed);
		inputComponent->BindAction("Grab", EInputEvent::IE_Released, this, &UGrabber::GrabReleased);
		UE_LOG(LogTemp, Warning, TEXT("Input component found on %s"), *GetOwner()->GetName());
	}
}

void UGrabber::FindPhysicsHandle() {
	// ...
	physicsHandleComponent = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (physicsHandleComponent) {
		//Physics handle is bound correctly.
	} else {
		UE_LOG(LogTemp, Error, TEXT("Couldn't find physics handle on %s"), *GetOwner()->GetName());
	}
}

void UGrabber::GrabPressed() {
	UE_LOG(LogTemp, Warning, TEXT("Grabber press"));

	// TODO only when action is triggered. Try and reach any actors with physics body collision channel set
		FVector location; 
	FRotator rotation;
	APlayerController *playerController = GetWorld()->GetFirstPlayerController();
	playerController->GetPlayerViewPoint(OUT location, OUT rotation);

	// Line trace
	FVector lineTraceBegin = location;
	FVector lineTraceEnd = location + (rotation.Vector() * reach);

	FHitResult hitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent *componentToGrab = hitResult.GetComponent();

	if (hitResult.GetActor()) {
		physicsHandleComponent->GrabComponentAtLocation
			(
				componentToGrab,
				NAME_None,
				lineTraceEnd
			);
	}

	// If we hit something attach physics handle
	// TODO attach physics handle
}

void UGrabber::GrabReleased() {
	UE_LOG(LogTemp, Warning, TEXT("Grabber released"));

	// TODO remove/release physics handle
	physicsHandleComponent->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector location; 
	FRotator rotation;
	APlayerController *playerController = GetWorld()->GetFirstPlayerController();
	playerController->GetPlayerViewPoint(OUT location, OUT rotation);

	// Line trace
	FVector lineTraceBegin = location;
	FVector lineTraceEnd = location + (rotation.Vector() * reach);

	if (physicsHandleComponent->GrabbedComponent) {
		physicsHandleComponent->SetTargetLocation(lineTraceEnd);
	}

	// If the physic handle is attahched.
	// Move the object we are holding.

	// ...
	// Get players viewpoint
	/*FVector location; 
	FRotator rotation;
	APlayerController *playerController = GetWorld()->GetFirstPlayerController();
	playerController->GetPlayerViewPoint(OUT location, OUT rotation);

	// Debug draw line trace
	FVector lineTraceBegin = location;
	FVector lineTraceEnd = location + (rotation.Vector() * reach);

	if (isDebugEnabled) {
		DrawDebugLine(GetWorld(), 
			lineTraceBegin, 
			lineTraceEnd, 
			FColor(0, 255, 0),
			false, 
			0.f,
			0,
			5.f);
	}*/

	// See what we hit
}


FHitResult UGrabber::GetFirstPhysicsBodyInReach() const {
	// Get players viewpoint
	FVector location; 
	FRotator rotation;
	APlayerController *playerController = GetWorld()->GetFirstPlayerController();
	playerController->GetPlayerViewPoint(OUT location, OUT rotation);

	// Line trace
	FVector lineTraceBegin = location;
	FVector lineTraceEnd = location + (rotation.Vector() * reach);

	// Ray-cast out to turn certain distance (Reach)
	FHitResult hitResult;
	FCollisionObjectQueryParams objectQueryParams(ECollisionChannel::ECC_PhysicsBody);
	FCollisionQueryParams queryParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT hitResult,
		lineTraceBegin,
		lineTraceEnd,
		objectQueryParams,
		queryParams);

	AActor *hitActor = hitResult.GetActor();
	if (hitActor) {
		UE_LOG(LogTemp, Warning, TEXT("You can grab %s"), *hitActor->GetName());
	}

	return hitResult;
}
