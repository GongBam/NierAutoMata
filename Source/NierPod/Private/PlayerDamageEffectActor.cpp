

#include "PlayerDamageEffectActor.h"
#include "Particles/ParticleSystemComponent.h"


APlayerDamageEffectActor::APlayerDamageEffectActor()
{
	PrimaryActorTick.bCanEverTick = true;


	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(rootComp);

	particleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle Component"));
	particleComp->SetupAttachment(rootComp);
	particleComp->SetAutoActivate(false);

}

void APlayerDamageEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void APlayerDamageEffectActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerDamageEffectActor::PlayFX()
{
	particleComp->Activate(true);
}

void APlayerDamageEffectActor::EndFX()
{
	particleComp->Activate(false);
}
