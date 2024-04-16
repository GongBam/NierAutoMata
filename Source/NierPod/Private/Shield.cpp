

#include "Shield.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

AShield::AShield()
{
	PrimaryActorTick.bCanEverTick = true;

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	SetRootComponent(sphereComp);

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	meshComp->SetupAttachment(sphereComp);
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);


}

void AShield::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(1.5f);
	
}

void AShield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

