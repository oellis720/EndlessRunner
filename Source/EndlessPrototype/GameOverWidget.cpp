// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UGameOverWidget::NativeConstruct()
{
	if (Replay) {
		Replay->OnClicked.AddDynamic(this, &UGameOverWidget::OnReplayClick);
	}

	if (MainMenu) {
		MainMenu->OnClicked.AddDynamic(this, &UGameOverWidget::OnMenuClick);
	}

}

void UGameOverWidget::OnReplayClick()
{
	UWorld* World = GetWorld();
	if (World) {
		UKismetSystemLibrary::ExecuteConsoleCommand(World, TEXT("RestartLevel"));
	}
}

void UGameOverWidget::OnMenuClick()
{
	UWorld* World = GetWorld();
	if (World) {
		UGameplayStatics::OpenLevel(World, TEXT("MenuLevel"));
	}

}
