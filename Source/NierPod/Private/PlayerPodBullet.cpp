// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPodBullet.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "BossCharacter.h"

// Sets default values
APlayerPodBullet::APlayerPodBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Comp"));
	SetRootComponent(SphereComp);
	SphereComp->SetSphereRadius(5);
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionObjectType(ECC_GameTraceChannel1);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);
	SphereComp->SetGenerateOverlapEvents(true);


	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	meshComp->SetupAttachment(SphereComp);
	meshComp->SetWorldScale3D(FVector(0.1f));
	meshComp->SetRelativeLocation(FVector(0, 0, -5));
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

// Called when the game starts or when spawned
void APlayerPodBullet::BeginPlay()
{
	Super::BeginPlay();
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &APlayerPodBullet::OnoverlapEnermy);
	SetLifeSpan(1.0f);
}

// Called every frame
void APlayerPodBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	FVector targetLoc = (GetActorLocation() + GetActorForwardVector() * Speed * DeltaTime);
	SetActorLocation(targetLoc);

}


void APlayerPodBullet::OnoverlapEnermy(UPrimitiveComponent* OverlappedComponent, AActor* otherActor, UPrimitiveComponent* OtherComp, int32 OtherbodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionFX, otherActor->GetActorLocation(), FRotator::ZeroRotator);

	if (explosionSound != nullptr)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), explosionSound);
	}

	ABossCharacter* boss = Cast<ABossCharacter>(otherActor);
	if (boss != nullptr)
	{
		boss->OnDamagedByPod(1);

	}
	Destroy();
}

