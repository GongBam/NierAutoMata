// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerPOD.generated.h"

UCLASS()
class NIERPOD_API APlayerPOD : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerPOD();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:

public:

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<class APlayerPodBullet> bullet_bp;

	float ShotTime = 0.0f;
	bool bShootReady = true;

	void Shooting();


};

