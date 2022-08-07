// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EndlessPrototypeGameModeBase.generated.h"

/**
 * 
 */
class AFloorTiles;
class UUserWidget;

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkillCountChanged, int, SkillCount);

UCLASS()
class ENDLESSPROTOTYPE_API AEndlessPrototypeGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Configuration")
	int InitialFloorTiles = 10;

	UPROPERTY(EditAnywhere, Category = "Configuration")
	TSubclassOf<AFloorTiles> FloorTileClass;

	UPROPERTY(EditAnywhere, Category = "Configuration")
	TSubclassOf<UUserWidget> GameHUDClass;

	UPROPERTY(VisibleInstanceOnly, Category = "Runtime")
	FTransform NextSpawnPoint;

	UPROPERTY(VisibleInstanceOnly, Category = "Runtime")
	TArray<float> LaneSwitchValues;

	UPROPERTY(VisibleInstanceOnly, Category = "Runtime")
	class UGameHUDWidget* GameHud;

	UPROPERTY(VisibleAnywhere)
	int TotalPickedUp = 0;

	UFUNCTION(BlueprintCallable)
	void CreateInitialTiles();

	UFUNCTION(BlueprintCallable)
	AFloorTiles* AddTiles(const bool bSpawnItems);

	UFUNCTION(BlueprintCallable)
	void AddPickup();


protected:
	virtual void BeginPlay() override;
};
