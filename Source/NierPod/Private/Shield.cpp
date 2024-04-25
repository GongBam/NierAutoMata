

#include "Shield.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "BossCharacter.h"
#include "EngineUtils.h"

AShield::AShield()
{
	PrimaryActorTick.bCanEverTick = true;

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	SetRootComponent(sphereComp);
	sphereComp->SetCollisionProfileName(FName("ShieldPreset"));
	sphereComp->SetNotifyRigidBodyCollision(true);

	sphereComp2 = CreateDefaultSubobject<USphereComponent>(TEXT("Attack Sphere Component"));
	sphereComp2->SetupAttachment(RootComponent);
	sphereComp2->SetRelativeScale3D(FVector(4));
	sphereComp2->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	meshComp->SetupAttachment(sphereComp);
	meshComp->SetRelativeScale3D(FVector(0.6));
	meshComp->SetRelativeLocation(FVector(0, 0, -30));
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	meshComp->SetSimulatePhysics(false);

}

void AShield::BeginPlay()
{
	Super::BeginPlay();

	//보스정보 캐싱
	for (TActorIterator<ABossCharacter> iter(GetWorld()); iter; ++iter)
	{
		boss = *iter;
	}
	
	//히트이벤트 바인딩 
	sphereComp2->OnComponentBeginOverlap.AddDynamic(this, &AShield::ShieldAttacking);
}

void AShield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
//쉴드공격 함수 (쉴드 확장시켜서 플레이어 밀쳐내기)
void AShield::ShieldExtending()
{	
	sphereComp2->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//Lerp 이용해서 쉴드크기 확장시키기 
	FVector originScale = meshComp->GetComponentScale();
	FVector newScale = originScale * 2;
	FVector lerpScale = FMath::Lerp(originScale, newScale, GetWorld()->GetDeltaSeconds() * 2);

	meshComp->SetRelativeScale3D(lerpScale);

	FTimerHandle destoryTimer;
	GetWorldTimerManager().SetTimer(destoryTimer, FTimerDelegate::CreateLambda([&]() {
		if (shieldAttackSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), shieldAttackSound, GetActorLocation(), GetActorRotation());
			boss->currentTime = 0;
			boss->bossState = EBossState::ATTACKDELAY;
			Destroy();
		}
	}), 0.5f, false);
}
//쉴드에 플레이어가 맞으면 플레이어 데미지 + 넉백 
void AShield::ShieldAttacking(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SewwpResult)
{	
	player = Cast<APlayerCharacter>(OtherActor);
	if (player != nullptr)
	{

		player->PlayerDamagedWithKnockBack(damage);
	}
}

