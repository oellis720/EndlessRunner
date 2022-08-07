// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverWidget.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSPROTOTYPE_API UGameOverWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Replay;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* MainMenu;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnReplayClick();

	UFUNCTION()
	void OnMenuClick();
};
