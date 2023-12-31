// Fill out your copyright notice in the Description page of Project Settings.


#include "TLItemChest.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ATLItemChest::ATLItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	LidMesh->SetupAttachment(BaseMesh);
}

// Called when the game starts or when spawned
void ATLItemChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATLItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATLItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	bIsOpen = !bIsOpen;
}

