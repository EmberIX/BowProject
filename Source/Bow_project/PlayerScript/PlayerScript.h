// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerScript.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BOW_PROJECT_API UPlayerScript : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerScript();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	ACharacter* playerCharacter;

	USkeletalMeshComponent* meshComponent;
	UAnimInstance* animInstance;

	UFloatProperty* animProperty_Direction;
	UFloatProperty* animProperty_Speed;

	void Setup_InputComponent();
	void Pressed_MoveForward(float axisValue);
	void Pressed_MoveRight(float axisValue);
	void Mouse_LookAround(float axisValue);
	void Mouse_LookUpDown(float axisValue);

	void Updating_PlayerAnimationSpeed();

	bool OnForward = true;
	bool OnRight = false;
};
