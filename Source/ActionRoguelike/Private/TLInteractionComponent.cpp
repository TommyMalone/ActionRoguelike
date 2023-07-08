// Fill out your copyright notice in the Description page of Project Settings.


#include "TLInteractionComponent.h"
#include "TLGameplayInterface.h"
#include <DrawDebugHelpers.h>

// Sets default values for this component's properties
UTLInteractionComponent::UTLInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UTLInteractionComponent::PrimaryInteract()
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	AActor* Owner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;
	Owner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = EyeLocation + (EyeRotation.Vector() * 1000);

	TArray<FHitResult> Hits;
	FCollisionShape Shape;
	float Radius = 30.0f;
	Shape.SetSphere(Radius);
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);

	FColor DebugTraceColor = bBlockingHit ? FColor::Green : FColor::Red;

	for (FHitResult Hit : Hits)
	{
		if (AActor* HitActor = Hit.GetActor())
		{
			if (HitActor->Implements<UTLGameplayInterface>())
			{
				APawn* OwnerPawn = Cast<APawn>(Owner);
				ITLGameplayInterface::Execute_Interact(HitActor, OwnerPawn);
				DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, DebugTraceColor, false, 2.0f);
				break;
			}
		}
	}

	DrawDebugLine(GetWorld(), EyeLocation, End, DebugTraceColor, false, 2.0f, 0, 2.0f);
}


// Called when the game starts
void UTLInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTLInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

