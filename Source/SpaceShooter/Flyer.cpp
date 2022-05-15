// Fill out your copyright notice in the Description page of Project Settings.

#include "Flyer.h"

// Sets default values
AFlyer::AFlyer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AFlyer::ProcessedKeyPitch(float Rate)
{
	if (FMath::Abs(Rate) > .2f)
		ProcessPitch(Rate * 2.f);
}

void AFlyer::ProcessedKeyRoll(float Rate)
{
	if (FMath::Abs(Rate) > .2f)
		ProcessRoll(Rate * 2.f);
}

void AFlyer::ProcessedMouseYInput(float Value)
{
	ProcessPitch(Value);
}

void AFlyer::ProcessedMouseXInput(float Value)
{
	ProcessRoll(Value);
}

void AFlyer::ProcessRoll(float Value)
{
	bIntentionalRoll = FMath::Abs(Value) > 0.f;

	if (bIntentionalPitch && !bIntentionalRoll) return;

	const float TargetRollSpeed = bIntentionalRoll ? (Value * RollRateMultiplier) : (GetActorRotation().Roll * -2.f);

	CurrentRollSpeed = FMath::FInterpTo(CurrentRollSpeed, TargetRollSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
}

void AFlyer::ProcessPitch(float Value)
{
	bIntentionalPitch = FMath::Abs(Value) > 0.f;
	const float TargetPitchSpeed = Value * PitchRateMultiplier;
	CurrentPitchSpeed = FMath::FInterpTo(CurrentPitchSpeed, TargetPitchSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
}

// Called when the game starts or when spawned
void AFlyer::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFlyer::Tick(float DeltaTime)
{
	//calculateThrust
	const float CurrentAcc = GetActorRotation().Pitch * DeltaTime * Acceleration;
	const float NewForwardSpeed = -CurrentForwardSpeed + CurrentAcc;
	CurrentForwardSpeed = FMath::Clamp(NewForwardSpeed, MinSpeed, MaxSpeed);

	const FVector LocalMove = FVector(CurrentForwardSpeed * DeltaTime, 0.f, 0.f);
	AddActorLocalOffset(LocalMove, true);  //sweep will stop the model from going trough walls

	FRotator DeltaRotation(0, 0, 0);
	DeltaRotation.Roll = CurrentRollSpeed * DeltaTime;
	DeltaRotation.Yaw = CurrentYawSpeed * DeltaTime;
	DeltaRotation.Pitch = CurrentPitchSpeed * DeltaTime;

	AddActorLocalRotation(DeltaRotation);

	GEngine->AddOnScreenDebugMessage(0, 0.f, FColor::Green, FString::Printf(TEXT("ForwardSpeed: %f"), CurrentForwardSpeed)); //need to fix the speed not updating when you play the games

	Super::Tick(DeltaTime);
}

//detects collision between the player model and the environment and tries to deflect it
void AFlyer::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	//deflect along surface
	const FRotator CurrentRotation = GetActorRotation();
	SetActorRotation(FQuat::Slerp(CurrentRotation.Quaternion(), HitNormal.ToOrientationQuat(), 0.025f));

	//slow down
	CurrentForwardSpeed = FMath::FInterpTo(CurrentForwardSpeed, MinSpeed, GetWorld()->GetDeltaSeconds(), 5.f);
}

// Called to bind functionality to input
void AFlyer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//key binding setup
	PlayerInputComponent->BindAxis("Turn", this, &AFlyer::ProcessedMouseXInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AFlyer::ProcessedKeyRoll);
	PlayerInputComponent->BindAxis("LookUp", this, &AFlyer::ProcessedMouseYInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AFlyer::ProcessedKeyPitch);
}