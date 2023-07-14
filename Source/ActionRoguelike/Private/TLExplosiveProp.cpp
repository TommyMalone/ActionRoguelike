// Fill out your copyright notice in the Description page of Project Settings.


#include "TLExplosiveProp.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ATLExplosiveProp::ATLExplosiveProp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//bSimulatePhysics = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
	StaticMeshComp->SetSimulatePhysics(true);
	StaticMeshComp->SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	StaticMeshComp->SetCollisionObjectType(ECC_PhysicsBody);
	RootComponent = StaticMeshComp;

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>("RadialForceComp");
	RadialForceComp->Radius = 750.0f;
	RadialForceComp->ImpulseStrength = 2500.0f;
	RadialForceComp->bImpulseVelChange = true;

	RadialForceComp->SetupAttachment(StaticMeshComp);
}

// Called when the game starts or when spawned
void ATLExplosiveProp::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATLExplosiveProp::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	StaticMeshComp->OnComponentHit.AddDynamic(this, &ATLExplosiveProp::OnActorHit);
}

void ATLExplosiveProp::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}

void ATLExplosiveProp::Explode()
{
	RadialForceComp->FireImpulse();
}

// Called every frame
void ATLExplosiveProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

