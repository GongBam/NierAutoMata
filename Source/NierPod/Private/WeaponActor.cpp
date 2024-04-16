#include "WeaponActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GamePlayStatics.h"
#include "EngineUtils.h"
#include "BossCharacter.h"


AWeaponActor::AWeaponActor()
{

	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Comp"));
	SetRootComponent(BoxComp);
	BoxComp->SetRelativeScale3D(FVector(1, 2, 0.3));
	BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComp->SetCollisionObjectType(ECC_GameTraceChannel1);
	BoxComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);
	BoxComp->SetGenerateOverlapEvents(true);
	BoxComp->SetRelativeRotation(FRotator(0, 0, -90));


	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	meshComp->SetupAttachment(BoxComp);
	meshComp->SetRelativeScale3D(FVector(0.6, 0.6, 0.35));
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeaponActor::BeginPlay()
{
	Super::BeginPlay();


	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AWeaponActor::OnoverlapEnemy);

	ABossCharacter* enemy = FindEnemy_Iterate();
	if (enemy != nullptr)
	{
		FVector lookDirection = enemy->GetActorLocation() - GetActorLocation();
		moveDir = lookDirection.GetSafeNormal();
	}
	else
	{

		Destroy();
	}

	SetLifeSpan(1.3f);

}


void AWeaponActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



	if (!bShouldStopMoving)
	{
		FVector targetLoc = GetActorLocation() + moveDir * Speed * DeltaTime;
		SetActorLocation(targetLoc);
	}

}


ABossCharacter* AWeaponActor::FindEnemy_Iterate()
{
	TArray<ABossCharacter*> enemys;
	for (TActorIterator<ABossCharacter> enemy(GetWorld()); enemy; ++enemy)
	{
		enemys.Add(*enemy);
	}
	if (enemys.Num() > 0)
	{
		return enemys[0];
	}
	else
	{
		return nullptr;
	}

}

void AWeaponActor::OnoverlapEnemy(UPrimitiveComponent* OverlappedComponent, AActor* otherActor, UPrimitiveComponent* OtherComp, int32 OtherbodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionFX, otherActor->GetActorLocation(), FRotator::ZeroRotator);


	if (explosionSound != nullptr)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), explosionSound);
	}
	ABossCharacter* boss = Cast<ABossCharacter>(otherActor);
	if (boss != nullptr)
	{
		boss->OnDamaged(5);
	}

	bShouldStopMoving = true;
}

