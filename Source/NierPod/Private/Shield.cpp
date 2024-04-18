

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
	
	//��Ʈ�̺�Ʈ ���ε� 
	sphereComp2->OnComponentBeginOverlap.AddDynamic(this, &AShield::ShieldAttacking);
	//2�� �� ������� LifeSpan ����
	SetLifeSpan(2.0f);
	
}

void AShield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
//������� �Լ� (���� Ȯ����Ѽ� �÷��̾� ���ĳ���)
void AShield::ShieldExtending()
{	
	sphereComp2->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//Lerp �̿��ؼ� ����ũ�� Ȯ���Ű�� 
	FVector originScale = sphereComp->GetComponentScale();
	FVector newScale = originScale * 5;
	FVector lerpScale = FMath::Lerp(originScale, newScale, GetWorld()->GetDeltaSeconds() * 3);

	sphereComp->SetRelativeScale3D(lerpScale);

	//0.7�� �� Destory 
	FTimerHandle destoryTimer;
	GetWorldTimerManager().SetTimer(destoryTimer, FTimerDelegate::CreateLambda([&]() {
		Destroy();
		}), 1.0f, false);
}
//���忡 �÷��̾ ������ �÷��̾� ������ + �˹� 
void AShield::ShieldAttacking(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SewwpResult)
{	
	player = Cast<APlayerCharacter>(OtherActor);
	if (player != nullptr)
	{	
		player->PlayerDamagedWithKnockBack(damage);
	}
}

