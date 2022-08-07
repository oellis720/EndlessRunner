// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RunCharacter.generated.h"

UCLASS()
class ENDLESSPROTOTYPE_API ARunCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARunCharacter();

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	int CurrentLane = 1;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	int NextLane = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveDownImpulse = -1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	bool isUsingAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	float AbilityDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	int PickupCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	FTimerHandle AbilityTimerHandle; 

	UPROPERTY(EditAnywhere, Category = "Configuration")
	TSubclassOf<UUserWidget> GameOverScreenClass;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Lanes")
	void ChangeLane();

	UFUNCTION(BlueprintCallable, Category = "Lanes")
	void ChangeLaneUpdate(const float Value);

	UFUNCTION(BlueprintCallable, Category = "Lanes")
	void ChangeLaneFinish();

	UFUNCTION(BlueprintCallable)
	void Death();

	UFUNCTION(BlueprintCallable)
	void GameOver();

	UFUNCTION()
	void AddPickup();

	UFUNCTION()
		void OnBoxHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(VisibleInstanceOnly)
	class AEndlessPrototypeGameModeBase* RunGameMode;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	FTimerHandle RestartTimerHandle;

	UPROPERTY()
	bool bIsDead = false;
	
	UFUNCTION()
	void MoveLeft();

	UFUNCTION()
	void MoveRight();

	UFUNCTION()
	void MoveDown();

	UFUNCTION()
	void OnDeath();

	UFUNCTION()
	void UseAbility();

	UFUNCTION()
	void ResetAbility();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
