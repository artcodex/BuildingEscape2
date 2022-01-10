// Copyright Michael Bridges 2019

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_ESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere)	
	float reach = 150.f;

	UPROPERTY(EditAnywhere)
	bool isDebugEnabled = true;	

	UPhysicsHandleComponent *physicsHandleComponent = nullptr;
	UInputComponent *inputComponent = nullptr;

	void GrabPressed();
	void GrabReleased();
	void FindPhysicsHandle();
	void SetupInputComponent();

	// Return the first Actor within reach with physics body
	FHitResult GetFirstPhysicsBodyInReach() const;
	TTuple<FVector, FRotator> GetLineTraceBegin() const;
	FVector GetLineTraceEnd() const;
};
