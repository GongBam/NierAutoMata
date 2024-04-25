// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "POD9s.generated.h"

UCLASS()
class NIERPOD_API APOD9s : public ACharacter
{
	GENERATED_BODY()

public:

	APOD9s();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;


	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<class APodBulletActor> bullet_bp;

	float ShotTime = 0.0f;
	bool bShootReady = true;
	bool bShootStart = false;
	FTimerHandle StartTime;

	void Shooting();
	void ShootingStart();


};
