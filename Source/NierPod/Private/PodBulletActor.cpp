// Fill out your copyright notice in the Description page of Project Settings.


#include "PodBulletActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "BossCharacter.h"


APodBulletActor::APodBulletActor()
{

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


void APodBulletActor::BeginPlay()
{
	Super::BeginPlay();
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &APodBulletActor::OnoverlapEnermy);

	ABossCharacter* boss = FindEnemy_Iterator();
	if (boss != nullptr)
	{
		FVector lookDirection = boss->GetActorLocation() - GetActorLocation();
		moveDir = lookDirection.GetSafeNormal();
	}
	else
	{
		moveDir = GetActorForwardVector();
	}

}


void APodBulletActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector targetLoc = GetActorLocation() + moveDir * Speed * DeltaTime;
	SetActorLocation(targetLoc);
	//UE_LOG(LogTemp, Warning, TEXT("Shoot~"));


}

void APodBulletActor::OnoverlapEnermy(UPrimitiveComponent* OverlappedComponent, AActor* otherActor, UPrimitiveComponent* OtherComp, int32 OtherbodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionFX, otherActor->GetActorLocation(), FRotator::ZeroRotator);

	if (explosionSound != nullptr)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), explosionSound);
	}

	ABossCharacter* boss = Cast<ABossCharacter>(otherActor);
	if (boss != nullptr)
	{
		boss->OnDamaged(1);

	}



	Destroy();

}

ABossCharacter* APodBulletActor::FindEnemy_Iterator()
{
	TArray<ABossCharacter*> bosses;
	for (TActorIterator<ABossCharacter> boss(GetWorld()); boss; ++boss)
	{
		bosses.Add(*boss);
	}

	if (bosses.Num() > 0)
	{
		return bosses[0];
	}
	else
	{
		return nullptr;
	}

}
