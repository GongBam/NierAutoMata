

#include "AdamHair.h"
#include "Components/StaticMeshComponent.h"
#include "BossCharacter.h"

AAdamHair::AAdamHair()
{
	PrimaryActorTick.bCanEverTick = true;
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	meshComp->SetupAttachment(RootComponent);
}

void AAdamHair::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAdamHair::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

