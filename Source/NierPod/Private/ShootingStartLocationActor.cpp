

#include "ShootingStartLocationActor.h"
#include "Components/SceneComponent.h"
#include "Components/ArrowComponent.h"
#include "BossShootingActor.h"

AShootingStartLocationActor::AShootingStartLocationActor()
{
	PrimaryActorTick.bCanEverTick = true;

	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(rootComp);

	arrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow Component"));
	arrowComp->SetupAttachment(rootComp);
}

void AShootingStartLocationActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AShootingStartLocationActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bShoting == true)
	{ 
		currentTime += DeltaTime;
	
		if (currentTime > delayTime)
	
		{
			FActorSpawnParameters params;
			params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			GetWorld()->SpawnActor<ABossShootingActor>(shooting_bp, arrowComp->GetComponentLocation(), arrowComp->GetComponentRotation(), params);

			currentTime = 0;
		}
	}
}

