

#include "BossCharacter.h"
#include "EngineUtils.h"
#include "PlayerCharacter.h"
#include "GameFrameWork/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimMontage.h"
#include "Components/BoxComponent.h"
#include "Shield.h"
#include "BossHealthWidget.h"

ABossCharacter::ABossCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	leftFootCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftFoot Attack Collision"));
	leftFootCollision -> SetupAttachment(GetMesh(), TEXT("LeftToeBaseSocket"));
	leftFootCollision -> SetRelativeLocation(FVector(0, 10, 0));
	leftFootCollision -> SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));

	rightFootCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RightFoot Attack Collision"));
	rightFootCollision -> SetupAttachment(GetMesh(), TEXT("RightFootToeBaseSocket"));
	rightFootCollision -> SetRelativeLocation(FVector(-10, -20, 0));
	rightFootCollision -> SetRelativeScale3D(FVector(0.2f,0.2f,0.2f));

	leftFootCollision -> OnComponentBeginOverlap.AddDynamic(this, &ABossCharacter::OnDealDamageOverlapBegin);
	rightFootCollision -> OnComponentBeginOverlap.AddDynamic(this, &ABossCharacter::OnDealDamageOverlapBegin);


	//기본 캡슐컴포넌트 preset 설정
	GetCapsuleComponent()->SetCollisionProfileName(FName("BossPreset"));

	leftFootCollision->SetCollisionProfileName(FName("BossAttackPreset"));
	rightFootCollision->SetCollisionProfileName(FName("BossAttackPreset"));

	

}

void ABossCharacter::BeginPlay()
{
	Super::BeginPlay();

	//기본상태 IDLE 상태로 초기화
	bossState = EBossState::IDLE;

	//월드에 있는 플레이어 찾고 target 에 넣어둠 
	for (TActorIterator<APlayerCharacter> player(GetWorld()); player; ++player)
	{
		target = *player;
	}
	
	//현재 체력 max 로 초기화
	currentHP = maxHP;

	//체력바 UI 위젯 
	if (bossHealthWidget_bp != nullptr)
	{
		bossUI = CreateWidget<UBossHealthWidget>(GetWorld(), bossHealthWidget_bp);
		if (bossUI != nullptr)
		{
			bossUI->AddToViewport();
		}
	}
}

void ABossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (bossState)
	{
	case EBossState::IDLE:
		Idle(DeltaTime);
		break;
	case EBossState::MOVE:
		MoveToTarget(DeltaTime);
		break;
	case EBossState::ATTACKREADY:
		AttackReady();
		break;
	case EBossState::ATTACK:
		Attack();
		break;
	case EBossState::ATTACK2:
		Attack2();
		break;
	case EBossState::JUMPATTACK:
		JumpAttack();
		break;
	case EBossState::ATTACKDELAY:
		AttackDelay(DeltaTime);
		break;
	case EBossState::BLOCK:
		Blocking(DeltaTime);
		break;
	case EBossState::BLOCKATTACK:
		BlocKAttack();
		break;
	case EBossState::DAMAGED:
		DamageProcess(DeltaTime);
		break;
	case EBossState::DIE:
		break;
	default:

		break;
	}
}

void ABossCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

double ABossCharacter::GetMaxHealth()
{
	return double(maxHP);
}

double ABossCharacter::GetHealth()
{
	return double(currentHP);
}

void ABossCharacter::Idle(float DeltaSeconds)
{	
	//보스 피가 900 이하라면 MOVE 상태로 전환
	if (currentHP <= 900.0f)
	{
		bossState = EBossState::MOVE;
	}
}

void ABossCharacter::AttackReady()
{
	UE_LOG(LogTemp, Warning, TEXT("READY TO ATTACK PLAYER"));
	//플레이어 거리가 attackDistance 보다 작으면
	if (FVector::Distance(GetActorLocation(), target->GetActorLocation()) < attackDistance)
		{	int32 num = FMath::RandRange(1,3);
			if(num == 1)
			{
				bossState = EBossState::ATTACK;
			}
			if(num == 2)
			{
				bossState = EBossState::ATTACK2;
			}
			if(num == 3)
			{
				bossState = EBossState::JUMPATTACK;
			}
	}
	else
	{
		bossState = EBossState::MOVE;
	}
}

void ABossCharacter::Attack()
{	
	//플레이어 거리가 attackDistance 보다 작으면
	if (FVector::Distance(GetActorLocation(), target->GetActorLocation()) < attackDistance + 10.0f)
	{	

		UE_LOG(LogTemp, Warning, TEXT("Boss Kick!"));
		bossState = EBossState::ATTACKDELAY;
	}
	//플레이어 거리가 attackDistance 보다 크면
	else
	{	//플레이어 다시 따라감
		bossState = EBossState::MOVE;
	}
}

void ABossCharacter::Attack2()
{
	//플레이어 거리가 attackDistance 보다 작으면
	if (FVector::Distance(GetActorLocation(), target->GetActorLocation()) < attackDistance + 10.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Boss Kick 2 !"));
		bossState = EBossState::ATTACKDELAY;
	}
	//플레이어 거리가 attackDistance 보다 크면
	else
	{	//플레이어 다시 따라감
		bossState = EBossState::MOVE;
	}
}

void ABossCharacter::JumpAttack()
{
	//플레이어 거리가 attackDistance 보다 작으면
	if (FVector::Distance(GetActorLocation(), target->GetActorLocation()) < attackDistance + 10.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Boss Jump Kick !"));
		bossState = EBossState::ATTACKDELAY;
	}
	//플레이어 거리가 attackDistance 보다 크면
	else
	{	//플레이어 다시 따라감
		bossState = EBossState::MOVE;
	}
}

void ABossCharacter::MoveToTarget(float deltaSeconds)
{	
	//플레이어 방향백터
	FVector targetDir = target->GetActorLocation() - GetActorLocation();
	targetDir.Z = 0;

	// 플레이어가 공격범위 밖이라면
	if (targetDir.Length() > attackDistance)
	{
		//플레이어 따라감
		GetCharacterMovement()->MaxWalkSpeed = traceSpeed;
		AddMovementInput(targetDir.GetSafeNormal());

		//이동 방향으로 회전

		FRotator currentRot = GetActorRotation();
		FRotator targetRot = targetDir.ToOrientationRotator();

		FRotator calcRot = FMath::Lerp(currentRot, targetRot, deltaSeconds * rotSpeed);

		SetActorRotation(calcRot);
	}
	// 플레이어가 공격범위 안에 있다면 
	else
	{	// 어택준비로 전환
		bossState = EBossState::ATTACKREADY;
	}
}

void ABossCharacter::AttackDelay(float deltaSeconds)
{

	currentTime += deltaSeconds;
	if (currentTime > attackDelayTime)
	{
		currentTime = 0;
		bossState = EBossState::ATTACKREADY;
	}
	
	if(FVector::Distance(GetActorLocation(), target->GetActorLocation()) > attackDistance + 10.f)
	{	
		if(currentTime > attackDelayTime)
			{ 
				bossState = EBossState::MOVE;
			}
	}
}

void ABossCharacter::Blocking(float deltaSeconds)
{	//보스 피가 800 이하로 깎였을 때 부터 방어 시작

		//제자리에서 쉴드 생성
		GetWorld()->SpawnActor<AShield>(shield, GetActorLocation(), GetActorRotation());
		SetActorLocation(GetActorLocation());
		//2 초 지났을 때 확률적으로(Shield : 1.5초 뒤 저절로 사라짐)
		currentTime += deltaSeconds;
		if(currentTime > 2.0f)
		{	
			currentTime = 0;
			if(shield != nullptr)
			{ 
				int32 num = FMath::RandRange(1,2);
				if (num == 1)
				{		//MOVE 상태로 전환
					bossState = EBossState::MOVE;
				}
				if (num == 2)
				{ 
					bossState = EBossState::BLOCKATTACK;
				}
			}
		}
}

void ABossCharacter::BlocKAttack()
{	
	//쉴드공격 후 MOVE 전환
	UE_LOG(LogTemp, Warning, TEXT("SHIELD ATTACK"));
	bossState = EBossState::MOVE;
}

void ABossCharacter::DamageProcess(float deltaSeconds)
{
	FVector targetLoc = hitLocation + GetActorLocation() * -1.0f;
	FVector knockBackLocation = FMath::Lerp(GetActorLocation(), targetLoc, deltaSeconds * 1.5f);
	//보스 체력이 800 이하가 아니라면 
	if(currentHP > 900)
	{	
		//보스 뒤로 아주쪼금 밀려남 
		SetActorLocation(knockBackLocation, true);
		UE_LOG(LogTemp, Warning, TEXT("%d"), currentHP);
		bossState = EBossState::IDLE;
	}
	//보스 체력이 800 이하라면 
	else
	{ 
			//확률적으로
			int32 num = FMath::RandRange(0,5);
			if(num > 0)
			{ 
				//맞거나
				if (FVector::Distance(GetActorLocation(), targetLoc) > 10)
				{	
					//보스 뒤로 아주쪼금 밀려남 
					SetActorLocation(knockBackLocation, true);
					UE_LOG(LogTemp, Warning, TEXT("%d"), currentHP);
				}
				else
				{
					bossState = EBossState::MOVE;
				}
			}
			//방어
			else
			{
				bossState = EBossState::BLOCK;
			}
	}	
}

void ABossCharacter::BoxCollisionExtending()
{	//공격 애니메이션시 발 콜리전 커짐 
	rightFootCollision->SetRelativeScale3D(FVector(3.0f));
	leftFootCollision->SetRelativeScale3D(FVector(3.0f));
}

void ABossCharacter::BoxCollisionReset()
{	//공격 애니메이션 끝나면 발 콜리전 원상태로 돌아감 
	rightFootCollision->SetRelativeScale3D(FVector(0.2f));
	leftFootCollision->SetRelativeScale3D(FVector(0.2f));
}

void ABossCharacter::OnDamaged(int32 dmg)
{
	if (bossState == EBossState::BLOCK)
	{
		return;
	}
	if (bossState == EBossState::DAMAGED)
	{
		return;
	}
	//HP값이 0 ~ maxHP 값 사이에만 있을 수 있게 설정
	currentHP = FMath::Clamp(currentHP - dmg, 0, maxHP);

	// 데미지 계산 결과, 현재 체력이 0 보다 크면
	if (currentHP > 0)
	{
		//DamageProcess 로 전환
		bossState = EBossState::DAMAGED;
		hitLocation = GetActorLocation();
		UE_LOG(LogTemp, Warning, TEXT("%d"), currentHP);
	}
	// 데미지 계산 결과, 현재 체력이 0 이하라면 
	if (currentHP <= 0)
	{
		//DIE 
		bossState = EBossState::DIE;
		Die();
	}
}

//플레이어 데미지 주기
void ABossCharacter::OnDealDamageOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SewwpResult)
{
	// 왼발, 오른발 공격 콜리전에 플레이어가 닿았다면
	APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);
	if (player != nullptr)
	{	
		player->PlayerDamaged();
		UE_LOG(LogTemp, Warning, TEXT("PlayerDamaged"));
	}
}

void ABossCharacter::Die()
{	
	//콜리전 다 꺼버림
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	leftFootCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	rightFootCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetCharacterMovement()->DisableMovement();

	PlayAnimMontage(death_Montage);
}
