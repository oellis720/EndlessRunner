// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "Obstacle.h"
#include "FloorTiles.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class UBoxComponent;
class AObstacle;
class ASkillPickup;

UCLASS()
class ENDLESSPROTOTYPE_API AFloorTiles : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloorTiles();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* FloorMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UArrowComponent* AttachPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UArrowComponent* LeftLane;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UArrowComponent* RightLane;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UArrowComponent* CenterLane;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* FloorTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Configuration")
	TSubclassOf<AObstacle> SmallObstacleClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Configuration")
	TSubclassOf<AObstacle> LargeObstacleClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Configuration")
	TSubclassOf<ASkillPickup> SkillItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Configuration")
	float SpawnPercent1 = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Configuration")
	float SpawnPercent2 = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Configuration")
	float SpawnPercent3 = 0.5f;

	UFUNCTION(BlueprintCallable)
	void SpawnItem();


	FORCEINLINE const FTransform& GetAttachTransform() const {
		return AttachPoint->GetComponentTransform();
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleInstanceOnly)
	class AEndlessPrototypeGameModeBase* RunGameMode;

	UPROPERTY()
	FTimerHandle DestroyHandle;
	
	UFUNCTION()
	void OnTriggerBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void DestroyTiles();

	UFUNCTION()
	void SpawnLaneItem(UArrowComponent* Lane, int& NumLarge);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
