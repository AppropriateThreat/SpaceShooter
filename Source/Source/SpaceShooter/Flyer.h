// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Flyer.generated.h"

UCLASS()
class SPACESHOOTER_API AFlyer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFlyer();

	UPROPERTY(EditAnywhere, Category = "Flight")
		float Acceleration{ 30.f };

	UPROPERTY(EditAnywhere, Category = "Flight")
		float MaxSpeed{ 5000.f };

	UPROPERTY(EditAnywhere, Category = "Flight")
		float MinSpeed{ 500.f };

	UPROPERTY(VisibleAnywhere, Category = "Flight")
		float CurrentForwardSpeed{ 300.f }; //when the player will be instantiated, this will be the initial speed so there won't be a problem for the player to start from 0 speed

	UPROPERTY(EditAnywhere, Category = "Flight")
		float RollRateMultiplier{ 200.f };
	UPROPERTY(EditAnywhere, Category = "Flight")
		float PitchRateMultiplier{ 200.f };

	UPROPERTY(EditAnywhere, Category = "Flight")
		float BoostForwardSpeed{ CurrentForwardSpeed * 2 };

	float CurrentYawSpeed;
	float CurrentPitchSpeed;
	float CurrentRollSpeed;

	bool bIntentionalPitch{ false };
	bool bIntentionalRoll{ false };

protected:

	void ProcessedKeyPitch(float Rate);
	void ProcessedKeyRoll(float Rate);

	void ProcessedMouseYInput(float Value);
	void ProcessedMouseXInput(float Value);

	//for calculating the actual rotation
	void ProcessRoll(float Value);
	void ProcessPitch(float Value);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
