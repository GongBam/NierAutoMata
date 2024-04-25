// Fill out your copyright notice in the Description page of Project Settings.


#include "POD9s.h"
#include "PodBulletActor.h"


// Sets default values
APOD9s::APOD9s()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APOD9s::BeginPlay()
{
	Super::BeginPlay();


	GetWorldTimerManager().SetTimer(StartTime, this, &APOD9s::ShootingStart, 5.0f, false);
	
}


void APOD9s::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(bShootReady == false)
	{
		ShotTime += DeltaTime;

		if(ShotTime >= 0.1f)
		{
			bShootReady = true;
			ShotTime = 0.0f;
		}
	}
	Shooting();

}

// Called to bind functionality to input
void APOD9s::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APOD9s::Shooting()
{
	if(bShootStart == true)
	{
		if(bShootReady)
		{
			GetWorld()->SpawnActor<APodBulletActor>(bullet_bp, GetActorLocation(), GetActorRotation());
			bShootReady = false;
		}
	}
	
}

void APOD9s::ShootingStart()
{
	bShootStart = true;
}

