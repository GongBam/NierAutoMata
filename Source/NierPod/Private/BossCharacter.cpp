

#include "BossCharacter.h"
#include "EngineUtils.h"
#include "PlayerCharacter.h"
#include "GameFrameWork/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimMontage.h"
#include "Components/BoxComponent.h"
#include "Shield.h"
#include "BossHealthWidget.h"
#include "Components/WidgetComponent.h"

ABossCharacter::ABossCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	//왼발 공격 콜리전
	leftFootCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftFoot Attack Collision"));
	leftFootCollision -> SetupAttachment(GetMesh(), TEXT("LeftToeBaseSocket"));
	leftFootCollision -> SetRelativeLocation(FVector(0, 10, 0));
	leftFootCollision -> SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));
	//오른발 공격 콜리전
	rightFootCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RightFoot Attack Collision"));
	rightFootCollision -> SetupAttachment(GetMesh(), TEXT("RightFootToeBaseSocket"));
	rightFootCollision -> SetRelativeLocation(FVector(-10, -20, 0));
	rightFootCollision -> SetRelativeScale3D(FVector(0.2f,0.2f,0.2f));

	//체력바위젯
	bosswidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Boss Health Widget Component"));

	//공격 콜리전 데미지오버랩 바인딩 
	leftFootCollision -> OnComponentBeginOverlap.AddDynamic(this, &ABossCharacter::OnDealDamageOverlapBegin);
	rightFootCollision -> OnComponentBeginOverlap.AddDynamic(this, &ABossCharacter::OnDealDamageOverlapBegin);


	//기본 캡슐컴포넌트 preset 설정
	GetCapsuleComponent()->SetCollisionProfileName(FName("BossPreset"));

	//왼발, 오른발 공격 콜리전 양 발 소켓에 넣음 
	leftFootCollision->SetCollisionProfileName(FName("BossAttackPreset"));
	rightFootCollision->SetCollisionProfileName(FName("BossAttackPreset"));

	

}

void ABossCharacter::BeginPlay()
{
	Super::BeginPlay();

	//월드에 있는 플레이어 찾고 target 에 넣어둠 
	for (TActorIterator<APlayerCharacter> player(GetWorld()); player; ++player)
	{
		target = *player;
	}

	//기본상태 IDLE 상태로 초기화
	bossState = EBossState::IDLE;
	
	//현재 체력 max 로 초기화
	currentHP = maxHP;

	//공격받았는지 상태 false 초기화
	bIsAttacked = false;

	//체력바 UI 위젯 받아오고 뷰포트에 붙임
	bossUI = Cast<UBossHealthWidget>(bosswidgetComp->GetWidget());

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
	case EBossState::ROLLING:
		Rolling(DeltaTime);
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

void ABossCharacter::CheckDistance()
{
	//플레이어 거리가 attackDistance 보다 멀리 있으면 다시 쫓아감
	if (FVector::Distance(GetActorLocation(), target->GetActorLocation()) > attackDistance &&
		FVector::Distance(GetActorLocation(), target->GetActorLocation()) < rollingDistance)
	{
		bossState = EBossState::MOVE;
	}
	//플레이어 거리가 많이 멀면 한번 구름 
	if (FVector::Distance(GetActorLocation(), target->GetActorLocation()) > rollingDistance)
	{
		bossState = EBossState::ROLLING;
	}
}

void ABossCharacter::Idle(float DeltaSeconds)
{	
	//보스 피가 900 이하라면 MOVE 상태로 전환 - 페이즈 시작
	if (currentHP <= 900.0f)
	{
		bossState = EBossState::MOVE;
	}
}

void ABossCharacter::AttackReady()
{	// 특정 STATE 와 중복되지 않게 막기 
	if (bossState == EBossState::BLOCK) { return; }
	if (bossState == EBossState::BLOCKATTACK) { return; }
	if (bossState == EBossState::ROLLING) { return; }

	UE_LOG(LogTemp, Warning, TEXT("READY TO ATTACK PLAYER"));
	//플레이어 거리가 attackDistance 보다 작으면
	if (FVector::Distance(GetActorLocation(), target->GetActorLocation()) < attackDistance)
		{	//랜덤 발차기 - 기본공격 
			int32 num = FMath::RandRange(1,2);
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
		CheckDistance();
	}
}

void ABossCharacter::Attack()
{
	if (bossState == EBossState::BLOCK) {return;}

	//플레이어 거리가 공격범위 안에 있으면 공격 후 딜레이 
	if (FVector::Distance(GetActorLocation(), target->GetActorLocation()) < attackDistance + 10.0f)
	{	
		bossState = EBossState::ATTACKDELAY;
	}
	else
	{
		CheckDistance();
	}
}

void ABossCharacter::Attack2()
{
	if (bossState == EBossState::BLOCK) {return;}

	//플레이어 거리가 공격범위 안에 있으면 공격 후 딜레이 
	if (FVector::Distance(GetActorLocation(), target->GetActorLocation()) < attackDistance + 10.0f)
	{	
		bossState = EBossState::ATTACKDELAY;
	}
	else
	{
		CheckDistance();
	}
}

void ABossCharacter::JumpAttack()
{
	if (bossState == EBossState::BLOCK) { return; }

	//플레이어 거리가 공격범위 안에 있으면 공격 후 딜레이 
	if (FVector::Distance(GetActorLocation(), target->GetActorLocation()) < attackDistance + 10.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Boss Jump Kick !"));
		bossState = EBossState::ATTACKDELAY;
	}
	else
	{
		CheckDistance();
	}
}
//플레이어 쫓아가는 함수
void ABossCharacter::MoveToTarget(float deltaSeconds)
{	
	//플레이어 방향백터
	FVector targetDir = target->GetActorLocation() - GetActorLocation();
	targetDir.Z = 0;

	// 플레이어가 공격범위 밖이라면
	if (targetDir.Length() > attackDistance && targetDir.Length() < rollingDistance)
	{
		//플레이어 따라감
		GetCharacterMovement()->MaxWalkSpeed = traceSpeed;
		AddMovementInput(targetDir.GetSafeNormal());

		//이동 방향으로 회전
		FRotator currentRot = GetActorRotation();
		FRotator targetRot = targetDir.ToOrientationRotator();
		FRotator calcRot = FMath::Lerp(currentRot, targetRot, deltaSeconds * rotSpeed);
		SetActorRotation(calcRot);


		FTimerHandle blockTimer;
		GetWorldTimerManager().SetTimer(blockTimer, FTimerDelegate::CreateLambda([&]()
			{
				bossState = EBossState::BLOCK;
			}), 5.0f, false);
	}
	if (targetDir.Length() > rollingDistance)
	{
		bossState = EBossState::ROLLING;
	}
	// 플레이어가 공격범위 안에 있다면 + 공격당하지 않았다면 
	if (targetDir.Length() <= attackDistance && bIsAttacked == false)
	{	// 어택준비로 전환
		bossState = EBossState::ATTACKREADY;
	}
}

// 앞구르기 
void ABossCharacter::Rolling(float deltaSeconds)
{	
	UE_LOG(LogTemp, Warning, TEXT("ROLLING"));
	// 특정 STATE와 중복되지 않게 막기 
	if (bossState == EBossState::BLOCK) {return;}
	if (bossState == EBossState::DAMAGED) {return;}
	if (bossState == EBossState::BLOCKATTACK) {return;}

	//플레이어 방향백터
	FVector targetDir = target->GetActorLocation() - GetActorLocation();
	targetDir.Z = 0;

	//플레이어 쪽으로 구름 
	GetCharacterMovement()->MaxWalkSpeed = 1000;
	AddMovementInput(targetDir.GetSafeNormal());

	//이동 방향으로 회전
	FRotator currentRot = GetActorRotation();
	FRotator targetRot = targetDir.ToOrientationRotator();
	FRotator calcRot = FMath::Lerp(currentRot, targetRot, deltaSeconds * rotSpeed);
	SetActorRotation(calcRot);

	FTimerHandle rollingTimer;
	GetWorldTimerManager().SetTimer(rollingTimer, FTimerDelegate::CreateLambda([&]()
		{	
			// 플레이어가 공격범위 안에 있다면 + 공격당하지 않았다면 
			if (targetDir.Length() <= attackDistance && bIsAttacked == false)
			{	// 어택준비로 전환
				bossState = EBossState::ATTACKREADY;
			}
			// 플레이어가 공격범위 안에 없다면 다시 쫓아감 
			if (targetDir.Length() > attackDistance && bIsAttacked == false)
			{
				bossState = EBossState::MOVE;
			}
		}), 1.0f, false);
	if (bossState == EBossState::ROLLING) { return; }
}
 //ATTACK 후 잠시 Delay
void ABossCharacter::AttackDelay(float deltaSeconds)
{	
	currentTime += deltaSeconds;
	if (currentTime > attackDelayTime + 10 && bIsAttacked == false)
	{	//다음 공격 준비 
		currentTime = 0;
		bossState = EBossState::ATTACKREADY;
	}
	else
	{
		CheckDistance();
	}
	if (bIsAttacked == true)
	{
		return;
	}
}

void ABossCharacter::Blocking(float deltaSeconds)
{
	if (shieldSpawn == false)
	{
		//보스 피가 800 이하로 깎였을 때 부터 방어 시작
			//제자리에서 쉴드 생성
		GetWorld()->SpawnActor<AShield>(shield, GetActorLocation(), GetActorRotation());
		SetActorLocation(GetActorLocation());
		shieldSpawn = true;

		//2 초 지났을 때 (Shield : 1.5초 뒤 저절로 사라짐)
		FTimerHandle blockingTimer;
		GetWorldTimerManager().SetTimer(blockingTimer, FTimerDelegate::CreateLambda([&]()
			{
				bossState = EBossState::BLOCKATTACK;
				shieldSpawn = false;
			}), 1.0f, false);
	}
}

void ABossCharacter::BlocKAttack()
{	

	UE_LOG(LogTemp, Warning, TEXT("SHIELD ATTACK"));
	//쉴드공격 후 어택딜레이로 전환 : 방어 중이었으므로 거리조건 일단 추가 안함 
	FTimerHandle blockattackingTimer;
	GetWorldTimerManager().SetTimer(blockattackingTimer, FTimerDelegate::CreateLambda([&]()
		{
			bossState = EBossState::ATTACKDELAY;
		}), 1.0f, false);
}

void ABossCharacter::DamageProcess(float deltaSeconds)
{
	bIsAttacked = false;

	//보스 체력이 900 이하가 아니라면 
	if(currentHP > 900)
	{	
		//피격모션 후 IDLE 
		FTimerHandle hitTimer;
		GetWorldTimerManager().SetTimer(hitTimer, FTimerDelegate::CreateLambda([&]()
			{
			bossState = EBossState::IDLE;
			}), 1.0f, false);
	}
	//보스 체력이 900 이하라면 
	if(currentHP <= 900)
	{ 	
		//피격모션 후 플레이어 쫓아감 
		FTimerHandle hitTimer;
		GetWorldTimerManager().SetTimer(hitTimer, FTimerDelegate::CreateLambda([&]() 
		{
		bossState = EBossState::MOVE;
		}), 1.0f, false);
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

//보스 데미지받는 함수
void ABossCharacter::OnDamaged(int32 dmg)
{	
	//보스가 공격중 / 피격받은 직후 / 방어중 일 땐 데미지 적용 X 
	if (bossState == EBossState::ATTACK){return;}
	if (bossState == EBossState::ATTACK2){return;}
	if (bossState == EBossState::JUMPATTACK){return;}
	if (bossState == EBossState::BLOCK){return;}
	if (bossState == EBossState::BLOCKATTACK){return;}
	if (bossState == EBossState::DAMAGED){return;}

	//공격 받음 true 체크 
	bIsAttacked = true;

	//HP값이 0 ~ maxHP 값 사이에만 있을 수 있게 설정
	currentHP = FMath::Clamp(currentHP - dmg, 0, maxHP);
	if (bossUI != nullptr)
	{	//체력UI 깎임 
		bossUI->SetHealthBar((float)currentHP / (float)maxHP);
	}

	// 데미지 계산 결과, 현재 체력이 0 보다 크면
	if (currentHP > 0)
	{	
		//DamageProcess 로 전환
		bossState = EBossState::DAMAGED;
		hitLocation = GetActorLocation();
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
	}
}

void ABossCharacter::Die()
{	
	//죽는애님몽타주 실행 
	PlayAnimMontage(death_Montage);
	//콜리전 다 꺼버림
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	leftFootCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	rightFootCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetCharacterMovement()->DisableMovement();
}
