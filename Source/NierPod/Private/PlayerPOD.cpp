
#include "PlayerPOD.h"
#include "PlayerCharacter.h"
#include "Navigation/PathFollowingComponent.h"
#include "AITypes.h"
#include "PlayerPodBullet.h"

APlayerPOD::APlayerPOD()
{
	PrimaryActorTick.bCanEverTick = true;

}

void APlayerPOD::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerPOD::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bShootReady == false)
	{
		ShotTime += DeltaTime;

		if (ShotTime >= 0.1f)
		{
			bShootReady = true;
			ShotTime = 0.0f;

			UE_LOG(LogTemp, Warning, TEXT("Shoot Ready"));
		}
	}
}

void APlayerPOD::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
void APlayerPOD::Shooting()
{	
	if(bShootReady)
	{ 
		GetWorld()->SpawnActor<APlayerPodBullet>(bullet_bp,GetActorLocation(),GetActorRotation());

		UE_LOG(LogTemp, Warning, TEXT("Shoot"));

		bShootReady = false;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Wait ShootReady"));
	}
}
