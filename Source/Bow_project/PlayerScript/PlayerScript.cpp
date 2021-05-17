// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerScript.h"

// Sets default values for this component's properties
UPlayerScript::UPlayerScript()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	Arrow_BP = StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("/Game/BluePrint/Arrow_BP"));
	Aim = StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("/Game/UI/Aim.Aim_C"));
	// ...
}


// Called when the game starts
void UPlayerScript::BeginPlay()
{
	Super::BeginPlay();

	playerCharacter = Cast<ACharacter>(GetOwner());

	Setup_InputComponent();
	
	meshComponent = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();

	Aiming = CreateWidget<UPlayer_Aim>(GetWorld(), Aim);

	if (meshComponent != NULL)
	{
		animInstance = meshComponent->GetAnimInstance();

		if (animInstance != NULL)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, TEXT("AA"));
			animProperty_Direction = FindFProperty<FFloatProperty>(animInstance->GetClass(), "Direction");
			animProperty_Speed = FindFProperty<FFloatProperty>(animInstance->GetClass(), "Speed");
			animProperty_isAiming = FindFProperty<FBoolProperty>(animInstance->GetClass(), "isAiming");
		}
	}

	TArray<UCameraComponent*> Player_Camera;
	this->GetOwner()->GetComponents<UCameraComponent>(Player_Camera);

	for (int32 index = 0; index < Player_Camera.Num(); index++)
	{
		if (Player_Camera[index]->GetFName().ToString() == "Camera")
		{
			camera = Player_Camera[index];
		}
	}
}

void UPlayerScript::Setup_InputComponent()
{
	GetOwner()->InputComponent->BindAxis("MoveForward",this, &UPlayerScript::Pressed_MoveForward);
	GetOwner()->InputComponent->BindAxis("MoveRight",this, &UPlayerScript::Pressed_MoveRight);
	GetOwner()->InputComponent->BindAxis("LookAround",this, &UPlayerScript::Mouse_LookAround);
	GetOwner()->InputComponent->BindAxis("LookUpDown",this, &UPlayerScript::Mouse_LookUpDown);
	
	GetOwner()->InputComponent->BindAction("Aim",IE_Pressed,this, &UPlayerScript::Aimming);
	GetOwner()->InputComponent->BindAction("Shoot",IE_Pressed,this, &UPlayerScript::Shoot);
	GetOwner()->InputComponent->BindAction("Aim",IE_Released,this, &UPlayerScript::ReleasedAimming);
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

	playerCharacter->AddControllerYawInput(axisValue);
}

void UPlayerScript::Mouse_LookUpDown(float axisValue)
{

	playerCharacter->AddControllerPitchInput(axisValue);
}

// Called every frame
void UPlayerScript::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Updating_PlayerAnimationSpeed();

}

void UPlayerScript::Shoot()
{
	printf("Shoot");
	if (isAiming)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, TEXT("Shoot"));

		if (ShootArrow)
		{
			animInstance->Montage_Play(ShootArrow);

			playerScore += 1;

		}
		FVector Location = GetOwner()->GetActorLocation();
		FRotator Rotation;

		//GetController()->GetPlayerViewPoint(Location, Rotation);

		FActorSpawnParameters SpawnArrow;
		//AActor* SpawnActorRef = GetWorld()->SpawnActor<AActor>(ArrowToSpawn, Location, Rotation,SpawnArrow);
		Arrow = (AArrow*) GetWorld()->SpawnActor<AActor>(Arrow_BP, Location, Rotation, SpawnArrow);

	}

}

void UPlayerScript::Aimming()
{
	if (Aiming != NULL)
	{
		Aiming->AddToViewport(10);
		camera->FieldOfView = 50;
		isAiming = true;	
		if (animInstance != NULL)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, TEXT("dd"));
			animProperty_isAiming->SetPropertyValue_InContainer(animInstance, isAiming);
		}
	}



}

void UPlayerScript::ReleasedAimming()
{
	if (Aiming != NULL)
	{
		Aiming->RemoveFromParent();
		camera->FieldOfView = 70;


		isAiming = false;
		if (animInstance != NULL)
		{
			animProperty_isAiming->SetPropertyValue_InContainer(animInstance, isAiming);
		}
	}
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
