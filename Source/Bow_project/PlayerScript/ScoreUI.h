// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine.h"
#include "HighScore.h"
#include "Blueprint/UserWidget.h"
#include "RunTime/UMG/Public/BluePrint/WidgetTree.h"
#include "RunTime/UMG/Public/Components/TextBlock.h"
#include "PlayerScript.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ScoreUI.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BOW_PROJECT_API UScoreUI : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UScoreUI();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlueprintEdit")
		TSubclassOf<class UUserWidget> playerWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlueprintEdit")
		UHighScore* mainWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlueprintEdit")
		class UPlayerScript* playerSc;

	UTextBlock* scoreText;
};
