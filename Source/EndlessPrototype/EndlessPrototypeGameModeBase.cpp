// Copyright Epic Games, Inc. All Rights Reserved.


#include "EndlessPrototypeGameModeBase.h"
#include "FloorTiles.h"

void AEndlessPrototypeGameModeBase::CreateInitialTiles()
{
	AFloorTiles* Tile = AddTiles(false);

	if (Tile) {
		LaneSwitchValues.Add(Tile->LeftLane->GetComponentLocation().Y);
		LaneSwitchValues.Add(Tile->CenterLane->GetComponentLocation().Y);
		LaneSwitchValues.Add(Tile->RightLane->GetComponentLocation().Y);
	}
	
	AddTiles(false);
	AddTiles(false);

	for (int i = 0; i < InitialFloorTiles; i++) {
		AddTiles(true);
	}

}

AFloorTiles* AEndlessPrototypeGameModeBase::AddTiles(const bool bSpawnItem)
{
	UWorld* World = GetWorld();

	if (World) {
		AFloorTiles* Tile = World->SpawnActor<AFloorTiles>(FloorTileClass, NextSpawnPoint);

		if (Tile) {
			if (bSpawnItem) {
				Tile->SpawnItem();
			}

			NextSpawnPoint = Tile->GetAttachTransform();
		}
		return Tile;
	}
	
	return nullptr;
}

void AEndlessPrototypeGameModeBase::BeginPlay()
{
	CreateInitialTiles();
}

void AEndlessPrototypeGameModeBase::AddPickup()
{
	TotalPickedUp += 1;
}
