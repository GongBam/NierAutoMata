

#include "BossPhasingActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"


ABossPhasingActor::ABossPhasingActor()
{
	PrimaryActorTick.bCanEverTick = true;
	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	SetRootComponent(sphereComp);

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	meshComp->SetupAttachment(sphereComp);
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABossPhasingActor::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(2.0f);
}

void ABossPhasingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

