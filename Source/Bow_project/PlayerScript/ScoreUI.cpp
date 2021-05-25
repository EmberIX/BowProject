// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreUI.h"
#include "PlayerScript.h"

// Sets default values for this component's properties
UScoreUI::UScoreUI()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	playerWidget = StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("/Game/UI/Score.Score_C"));
	// ...
}


// Called when the game starts
void UScoreUI::BeginPlay()
{
	Super::BeginPlay();

	playerSc = Cast<UPlayerScript>(GetOwner());

	if (playerWidget != NULL)
	{
		mainWidget = CreateWidget<UHighScore>(GetWorld(), playerWidget);
		if (mainWidget != NULL)
		{
			mainWidget->AddToViewport(2);
			UWidgetTree* widgetTree = mainWidget->WidgetTree;
			FString widgetString = "ScoreText";
			FName widgetName = FName(*widgetString);
			UWidget* targetWidget = widgetTree->FindWidget(widgetName);

			if (targetWidget != NULL)
			{
				scoreText = Cast<UTextBlock>(targetWidget);
			}
		}
	}
	// ...
	
}


// Called every frame
void UScoreUI::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//FString stringToShow = "Score : " + FString::FromInt(playerSc->playerScore);
	//scoreText->SetText(FText::FromString(stringToShow));
}

