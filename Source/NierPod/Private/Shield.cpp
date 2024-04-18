

#include "Shield.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PlayerCharacter.h"

AShield::AShield()
{
	PrimaryActorTick.bCanEverTick = true;

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	SetRootComponent(sphereComp);
	sphereComp->SetCollisionProfileName(FName("ShieldPreset"));
	sphereComp->SetNotifyRigidBodyCollision(true);

	sphereComp2 = CreateDefaultSubobject<USphereComponent>(TEXT("Attack Sphere Component"));
	sphereComp2->SetupAttachment(RootComponent);
	sphereComp2->SetRelativeScale3D(FVector(5));
	sphereComp2->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	meshComp->SetupAttachment(sphereComp);
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AShield::BeginPlay()
{
	Super::BeginPlay();
	
	//히트이벤트 바인딩 
	sphereComp2->OnComponentBeginOverlap.AddDynamic(this, &AShield::ShieldAttacking);
	//2초 뒤 사라지게 LifeSpan 설정
	SetLifeSpan(2.0f);
	
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
	FVector originScale = sphereComp->GetComponentScale();
	FVector newScale = originScale * 5;
	FVector lerpScale = FMath::Lerp(originScale, newScale, GetWorld()->GetDeltaSeconds() * 3);

	sphereComp->SetRelativeScale3D(lerpScale);

	//0.7초 뒤 Destory 
	FTimerHandle destoryTimer;
	GetWorldTimerManager().SetTimer(destoryTimer, FTimerDelegate::CreateLambda([&]() {
		Destroy();
		}), 1.0f, false);
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

