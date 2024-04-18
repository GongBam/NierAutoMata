

#include "BossReturnLocationActor.h"
#include "Components/BoxComponent.h"

ABossReturnLocationActor::ABossReturnLocationActor()
{
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Teleport Location Component"));
	SetRootComponent(boxComp);
}

void ABossReturnLocationActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABossReturnLocationActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

