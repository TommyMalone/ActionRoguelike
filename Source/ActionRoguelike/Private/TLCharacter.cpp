// Fill out your copyright notice in the Description page of Project Settings.


#include "TLCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include <Math/MathFwd.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "TLInteractionComponent.h"

// Sets default values
ATLCharacter::ATLCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<UTLInteractionComponent>("InteractionComp");

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void ATLCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerMovementContext, 0);
		}
	}
	
}

void ATLCharacter::MoveHorizontal(const FInputActionValue& Value)
{
	//TODO: Figure out how to map A and D into Y values of vector.
	const FVector2D Axis2DValue = Value.Get<FVector2D>();
	UE_LOG(LogTemp, Warning, TEXT("X Movement: %f"), Axis2DValue.X);
	UE_LOG(LogTemp, Warning, TEXT("Y Movement: %f"), Axis2DValue.Y);
	AddMovementInput(GetActorForwardVector(), Axis2DValue.X);
	AddMovementInput(GetActorRightVector(), Axis2DValue.Y);
}

void ATLCharacter::MoveForward(const FInputActionValue& Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;
	AddMovementInput(ControlRot.Vector(), Value.Get<float>());
}

void ATLCharacter::MoveRight(const FInputActionValue& Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;
	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	AddMovementInput(RightVector, Value.Get<float>());
}


void ATLCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D Axis2DValue = Value.Get<FVector2D>();
	if (GetController())
	{
		AddControllerYawInput(Axis2DValue.X);
		AddControllerPitchInput(Axis2DValue.Y);
	}

}

void ATLCharacter::PrimaryInteract()
{
	InteractionComp->PrimaryInteract();
}

void ATLCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackAnim);
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ATLCharacter::PrimaryAttack_TimeElapsed, 0.2f);
}

void ATLCharacter::PrimaryAttack_TimeElapsed()
{
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

	FTransform SpawnTM = FTransform(GetControlRotation(), HandLocation);
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;

	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
}

void ATLCharacter::Jump()
{
	ACharacter::Jump();
}

// Called every frame
void ATLCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATLCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);	
	

	if (UEnhancedInputComponent* EnhancedPlayerInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//EnhancedPlayerInputComponent->BindAction(MoveHorizontalAction, ETriggerEvent::Triggered, this, &ATLCharacter::MoveHorizontal);
		EnhancedPlayerInputComponent->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &ATLCharacter::MoveForward);
		EnhancedPlayerInputComponent->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &ATLCharacter::MoveRight);
		EnhancedPlayerInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATLCharacter::Look);
		EnhancedPlayerInputComponent->BindAction(PrimaryInteractAction, ETriggerEvent::Triggered, this, &ATLCharacter::PrimaryInteract);
		EnhancedPlayerInputComponent->BindAction(PrimaryAttackAction, ETriggerEvent::Triggered, this, &ATLCharacter::PrimaryAttack);
		EnhancedPlayerInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ATLCharacter::Jump);
	}
}

