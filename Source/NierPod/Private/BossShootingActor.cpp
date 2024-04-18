

#include "BossShootingActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PlayerCharacter.h"
#include "EngineUtils.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>

ABossShootingActor::ABossShootingActor()
{
	PrimaryActorTick.bCanEverTick = true;

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	SetRootComponent(sphereComp);
	sphereComp -> SetCollisionProfileName(FName("BossShooting"));
	sphereComp -> SetGenerateOverlapEvents(true);

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	meshComp -> SetupAttachment(sphereComp);
	meshComp -> SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void ABossShootingActor::BeginPlay()
{
	Super::BeginPlay();
	
	sphereComp -> OnComponentBeginOverlap.AddDynamic(this, &ABossShootingActor::OnOverlapPlayer);

	//�÷��̾� ã�Ƽ� ��ġ���� �´� ȸ���� ���� 
	APlayerCharacter* player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (player != nullptr)
	{
		FVector lookDir = player->GetActorLocation() - GetActorLocation();
		moveDir = lookDir.GetSafeNormal();
		UE_LOG(LogTemp, Warning, TEXT("moveDir.X : %f, moveDir.Y : %f, moveDir.Z : %F"), moveDir.X, moveDir.Y, moveDir.Z);
	}
}

void ABossShootingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetLifeSpan(3.0f);
	//�÷��̾� ��ġ�� �ִ� ������ ��
	SetActorLocation(GetActorLocation() + moveDir * moveSpeed * DeltaTime);

}


void ABossShootingActor::OnOverlapPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);
	if(player != nullptr)
	{
		player->PlayerDamaged();
		Destroy();
	}
}

