// Fill out your copyright notice in the Description page of Project Settings.

#include "AICharacter.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "BossCharacter.h"
#include "EngineUtils.h"
#include "PlayerCharacter.h"
#include "GameFrameWork/CharacterMovementComponent.h"
#include "WeaponActor.h"


AAICharacter::AAICharacter()
{
 
	PrimaryActorTick.bCanEverTick = true;
	
	

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	meshComp->SetupAttachment(GetMesh(),FName("WeaponSocket"));
	meshComp->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.7f));
	//meshComp->SetRelativeRotation(FRotator(0,0,-170));
	meshComp->SetRelativeLocation(FVector(0, -40, 100));
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Attack Collision"));
	boxComp->SetupAttachment(meshComp);
	boxComp->SetBoxExtent(FVector(0, 0, 75.0f));
	boxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	boxComp->SetCollisionObjectType(ECC_GameTraceChannel1);
	boxComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);
	boxComp->SetGenerateOverlapEvents(true);



}

void AAICharacter::BeginPlay()
{
	Super::BeginPlay();

	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AAICharacter::OnAIAttackOverlapBegin);

	/*
	AIstate = EAIState::IDLE;

	for (TActorIterator<APlayerCharacter> player(GetWorld()); player; ++player)
	{
		target1 = *player;
	}

	for (TActorIterator<ABossCharacter> boss(GetWorld()); boss; ++boss)
	{
		target2 = *boss;
	}
	*/


}

void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*
	switch (AIstate)
	{
	case EAIState::IDLE:
		Idle(DeltaTime);
		break;
	case EAIState::MOVE:
		MoveToTarget(DeltaTime);
		break;
	case EAIState::ATTACK:
		Attack();
		break;
	case EAIState::ATTACKDELAY:
		AttackDelay(DeltaTime);
		break;
	default:
		break;
	}
	*/

}

void AAICharacter::OnAIAttackOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABossCharacter* boss = Cast<ABossCharacter>(OtherActor);
	if (boss != nullptr)
	{
		boss->OnDamaged(10);
	}
	UE_LOG(LogTemp, Warning, TEXT("slash"));
	
}

ABossCharacter* AAICharacter::FindEnemy_Iterate()
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


/*
void AAICharacter::Idle(float DeltaSeconds)
{
	AIstate = EAIState::MOVE;
}
*/

/*
void AAICharacter::Attack()
{
	if (FVector::Distance(target2->GetActorLocation(), GetActorLocation()) > ThrowingDistance)
	{
		PlayAnimMontage(AIThrowing);
		GetWorld()->SpawnActor<AWeaponActor>(weapon_bp, GetActorLocation(), GetActorRotation());

		AIstate = EAIState::ATTACKDELAY;
	}
	else
	{
		PlayAnimMontage(AIAttack);
		AIstate = EAIState::ATTACKDELAY;
	}

}
*/

/*
void AAICharacter::MoveToTarget(float DeltaSeconds)
{
	FVector targetDir = target2->GetActorLocation() - GetActorLocation();
	targetDir.Z = 0;
	if (targetDir.Length()<BossDistance)
	{
		GetCharacterMovement()->MaxWalkSpeed = traceSpeed;
		AddMovementInput(targetDir.GetSafeNormal());

		PlayAnimMontage(AIwalk);

		FRotator currentRot = GetActorRotation();
		FRotator targetRot = targetDir.ToOrientationRotator();

		FRotator calcRot = FMath::Lerp(currentRot, targetRot, DeltaSeconds * rotSpeed);

		SetActorRotation(calcRot);

		AIstate = EAIState::ATTACK;
	}
	else
	{
		FVector PlayerDir = target1->GetActorLocation() - GetActorLocation();
		PlayerDir.Z = 0;
		if (PlayerDir.Length() < PlayerDistance)
		{
			AIstate = EAIState::IDLE;
		}
	}
}
*/

/*
void AAICharacter::AttackDelay(float DeltaSeconds)
{
	currentTime += DeltaSeconds;
	if (currentTime > attackDelayTime)
	{
		currentTime = 0;
		AIstate = EAIState::ATTACK;
	}
	else
	{
		AIstate = EAIState::MOVE;
	}
}
*/


