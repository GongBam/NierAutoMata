
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
}

void APlayerPOD::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
void APlayerPOD::Shooting()
{
	GetWorld()->SpawnActor<APlayerPodBullet>(bullet_bp,GetActorLocation(),GetActorRotation());
}
