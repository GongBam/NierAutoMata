

#include "DamageEffectActor.h"
#include "Particles/ParticleSystemComponent.h"

ADamageEffectActor::ADamageEffectActor()
{
	PrimaryActorTick.bCanEverTick = true;

	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(rootComp);

	particleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle Component"));
	particleComp -> SetupAttachment(rootComp);
	particleComp -> SetAutoActivate(false);
}

void ADamageEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADamageEffectActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADamageEffectActor::PlayFX()
{
	particleComp->Activate(true);
}

void ADamageEffectActor::EndFX()
{
	particleComp->Activate(false);
}

