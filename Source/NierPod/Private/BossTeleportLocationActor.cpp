

#include "BossTeleportLocationActor.h"
#include "Components/BoxComponent.h"

ABossTeleportLocationActor::ABossTeleportLocationActor()
{
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Teleport Location Component"));
}

void ABossTeleportLocationActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABossTeleportLocationActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

