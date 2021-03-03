// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerScript.h"

// Sets default values for this component's properties
UPlayerScript::UPlayerScript()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerScript::BeginPlay()
{
	Super::BeginPlay();

	playerCharacter = Cast<ACharacter>(GetOwner());

	Setup_InputComponent();
	
	meshComponent = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();

	if (meshComponent != NULL)
	{
		animInstance = meshComponent->GetAnimInstance();

		if (animInstance != NULL)
		{
			animProperty_Direction = FindFProperty<FFloatProperty>(animInstance->GetClass(), "Direction");
			animProperty_Speed = FindFProperty<FFloatProperty>(animInstance->GetClass(), "Speed");
		}
	}
}

void UPlayerScript::Setup_InputComponent()
{
	GetOwner()->InputComponent->BindAxis("MoveForward",this, &UPlayerScript::Pressed_MoveForward);
	GetOwner()->InputComponent->BindAxis("MoveRight",this, &UPlayerScript::Pressed_MoveRight);
	GetOwner()->InputComponent->BindAxis("LookAround",this, &UPlayerScript::Mouse_LookAround);
	GetOwner()->InputComponent->BindAxis("LookUpDown",this, &UPlayerScript::Mouse_LookUpDown);
}

void UPlayerScript::Pressed_MoveForward(float axisValue)
{
	FRotator currentRotation = playerCharacter->GetControlRotation();
	currentRotation.Pitch = 0;

	FVector forwardDirection = FRotationMatrix(currentRotation).GetScaledAxis(EAxis::X);

	if (axisValue < 0)
	{
		axisValue = axisValue/2;
	}

	playerCharacter->AddMovementInput(forwardDirection,axisValue);

	if (axisValue < 0)
	{
		OnForward = false;
	}
	else
	{
		OnForward = true;
	}
}

void UPlayerScript::Pressed_MoveRight(float axisValue)
{
	FRotator currentRotation = playerCharacter->GetControlRotation();
	currentRotation.Pitch = 0;

	FVector rightDirection = FRotationMatrix(currentRotation).GetScaledAxis(EAxis::Y);

	playerCharacter->AddMovementInput(rightDirection, axisValue);
}

void UPlayerScript::Mouse_LookAround(float axisValue)
{
	//if (axisValue != 0)
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Cyan, TEXT("LookAround"));

	playerCharacter->AddControllerYawInput(axisValue);
}

void UPlayerScript::Mouse_LookUpDown(float axisValue)
{
	//if (axisValue != 0)
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Cyan, TEXT("LookUpDown"));

	playerCharacter->AddControllerPitchInput(axisValue);
}

// Called every frame
void UPlayerScript::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Updating_PlayerAnimationSpeed();
}


void UPlayerScript::Updating_PlayerAnimationSpeed()
{
	if (animProperty_Speed != NULL)
	{
		FVector groundVelocity = playerCharacter->GetCharacterMovement()->Velocity;
		groundVelocity.Z = 0.0f;

		float velocityLenght = groundVelocity.Size();

		if(OnForward == false)
		{
			velocityLenght = -velocityLenght;
		}

		animProperty_Speed->SetPropertyValue_InContainer(animInstance, velocityLenght);
		
		animProperty_Direction->SetPropertyValue_InContainer(animInstance, velocityLenght);
	}
}
