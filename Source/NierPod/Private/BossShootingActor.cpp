

#include "BossShootingActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PlayerCharacter.h"
#include "EngineUtils.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>
#include "Kismet/GameplayStatics.h"

ABossShootingActor::ABossShootingActor()
{
	PrimaryActorTick.bCanEverTick = true;

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	SetRootComponent(sphereComp);
	sphereComp -> SetCollisionProfileName(FName("BossShooting"));
	sphereComp -> SetGenerateOverlapEvents(true);
	sphereComp->SetWorldScale3D(FVector(2.5f));

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	meshComp -> SetupAttachment(sphereComp);
	meshComp -> SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void ABossShootingActor::BeginPlay()
{
	Super::BeginPlay();
	
	sphereComp -> OnComponentBeginOverlap.AddDynamic(this, &ABossShootingActor::OnOverlapPlayer);

	//플레이어 찾아서 위치값에 맞는 회전값 받음 
	APlayerCharacter* player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (player != nullptr)
	{
		FVector lookDir = player->GetActorLocation() - GetActorLocation();
		moveDir = lookDir.GetSafeNormal();
	}
}

void ABossShootingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetLifeSpan(3.0f);
	//플레이어 위치가 있던 쪽으로 감
	SetActorLocation(GetActorLocation() + moveDir * moveSpeed * DeltaTime);

}


void ABossShootingActor::OnOverlapPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);
	if(player != nullptr)
	{
		player->PlayerDamaged(damage);
		if (damagedSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), damagedSound, player->GetActorLocation(), player->GetActorRotation());
		}
		Destroy();
	}
}

