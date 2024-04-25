

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
#include "BossTeleportLocationActor.h"
#include "ShootingStartLocationActor.h"
#include "BossReturnLocationActor.h"
#include "Camera/CameraComponent.h"
#include "DamageEffectActor.h"
#include "Kismet/GameplayStatics.h"
#include "NierGameModeBase.h"

ABossCharacter::ABossCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	bossCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Boss Camera Component"));
	bossCamera -> SetupAttachment(RootComponent);
	bossCamera -> SetRelativeLocation(FVector(135,0,109));

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

	//상태 모두 초기화
	bIsAttacked = false;
	bFirstPhase = false;
	bFirstShooting = false;
	bSecondShooting = false;

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
	for (TActorIterator<ADamageEffectActor> iter(GetWorld()); iter; ++iter)
	{
		damageFX = *iter;
	}
	playerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));


	//GameModeBase 캐싱 
	AGameModeBase* gm = GetWorld()->GetAuthGameMode();
	nierGM = Cast<ANierGameModeBase>(gm);
}

void ABossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (bossState)
	{
	case EBossState::IDLE:
		Idle(DeltaTime);
		break;
	case EBossState::PLAYERKILL:
		PlayerKilled();
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
		BlocKAttack(DeltaTime);
		break;
	case EBossState::SHOOTATTACK:
		ShootingAttack(DeltaTime);
		break;
	case EBossState::DAMAGED:
		DamageProcess(DeltaTime);
		break;
	case EBossState::PHASECHANGE:
		Phasing(DeltaTime);
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
	if (playerCharacter->currentHP <= 0)
	{
		bossState = EBossState::PLAYERKILL;
	}
	//플레이어 거리가 attackDistance 보다 멀리 있으면 다시 쫓아감
	if (FVector::Distance(GetActorLocation(), target->GetActorLocation()) > attackDistance &&
		FVector::Distance(GetActorLocation(), target->GetActorLocation()) < rollingDistance)
	{
		bossState = EBossState::MOVE;
	}
	//플레이어 거리가 많이 멀면 한번 구름 
	else if (FVector::Distance(GetActorLocation(), target->GetActorLocation()) > rollingDistance)
	{
		bossState = EBossState::ROLLING;
	}
}

void ABossCharacter::Idle(float DeltaSeconds)
{	
	//보스 피가 900 이하라면 MOVE 상태로 전환 - 첫 페이즈 시작
	if (currentHP <= 4800.0f)
	{
		bossState = EBossState::MOVE;
	}

}

void ABossCharacter::AttackReady()
{	// 특정 STATE 와 중복되지 않게 막기 

	if (playerCharacter->currentHP <= 0)
	{
		bossState = EBossState::PLAYERKILL;
	}
	if (bossState == EBossState::BLOCK || bossState == EBossState::BLOCKATTACK) {return;}
	if (bIsAttacked == true)
	{
		return;
	}
	else if (bIsAttacked == false)
	{ 
		//플레이어 거리가 attackDistance 보다 작으면
		if (FVector::Distance(GetActorLocation(), target->GetActorLocation()) < attackDistance)
			{	//랜덤 발차기 
				int32 num = FMath::RandRange(1,3);
				if(num == 1)
				{	
					bossState = EBossState::ATTACK;
				}
				else if(num == 2)
				{
					bossState = EBossState::ATTACK2;
				}
				//점프공격은 보스체력 500 이하에서부터 시작 
				else if(num == 3 && bPhaseChanged == true)
				{
				bossState = EBossState::JUMPATTACK;
				}
		}
		else 
		{
			CheckDistance();
		}
	}
}
//일반공격1
void ABossCharacter::Attack()
{
	if (playerCharacter->currentHP <= 0)
	{
		bossState = EBossState::PLAYERKILL;
	}
	if (bossState == EBossState::BLOCK) {return;}
	if (bIsAttacked == true)
	{
		return;
	}
	//플레이어 거리가 공격범위 안에 있으면 공격 후 딜레이 
	if (FVector::Distance(GetActorLocation(), target->GetActorLocation()) < attackDistance + 10.0f)
	{
		bossState = EBossState::ATTACKDELAY;
	}
	else
	{
		//플레이어와의 거리가 공격가능 범위보다 멀다면 거리계산해서 MOVE 혹은 ROLLING>MOVE 
		CheckDistance();
	}
}
//일반공격2
void ABossCharacter::Attack2()
{	
	if (playerCharacter->currentHP <= 0)
	{
		bossState = EBossState::PLAYERKILL;
	}
	if (bIsAttacked == true)
	{
		return;
	}
	if (bossState == EBossState::BLOCK) {return;}
	//플레이어 거리가 공격범위 안에 있으면 공격 후 딜레이 
	if (FVector::Distance(GetActorLocation(), target->GetActorLocation()) < attackDistance + 10.0f)
	{
		bossState = EBossState::ATTACKDELAY;
	}
	else
	{	//플레이어와의 거리가 공격가능 범위보다 멀다면 거리계산해서 MOVE 혹은 ROLLING>MOVE 
		CheckDistance();
	}
}
//보스체력 500이하부터 점프공격 시작 
void ABossCharacter::JumpAttack()
{
	if (playerCharacter->currentHP <= 0)
	{
		bossState = EBossState::PLAYERKILL;
	}
	if (bIsAttacked == true)
	{
		return;
	}
	if (bossState == EBossState::BLOCK) { return;}

	//플레이어 거리가 공격범위 안에 있으면 공격 후 딜레이 
	if (FVector::Distance(GetActorLocation(), target->GetActorLocation()) < attackDistance + 10.0f)
	{
		bossState = EBossState::ATTACKDELAY;
	}
	else
	{	//플레이어와의 거리가 공격가능 범위보다 멀다면 거리계산해서 MOVE 혹은 ROLLING>MOVE 
		CheckDistance();
	}
}
//플레이어 쫓아가는 함수
void ABossCharacter::MoveToTarget(float deltaSeconds)
{	
	if (playerCharacter->currentHP <= 0)
	{
		bossState = EBossState::PLAYERKILL;
	}
	//플레이어 방향백터
	FVector targetDir = target->GetActorLocation() - GetActorLocation();
	targetDir.Z = 0;

	// 플레이어가 공격범위 안에 있다면 + 공격당하지 않았다면 
	if (targetDir.Length() <= attackDistance && bIsAttacked == false)
	{	// 어택준비로 전환
		
		bossState = EBossState::ATTACKREADY;
	}
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
		
		//Move 중 일정 시간이 지나면 방어상태로 변경
		currentTime += deltaSeconds;
		if(currentTime > 4.0f)
		{	
			currentTime = 0;
			bossState = EBossState::BLOCK;
		}
	}
	else if (targetDir.Length() > rollingDistance)
	{	//플레이어가 더 먼 범위에 있다면 앞구르기 실행 
		bossState = EBossState::ROLLING;
	}
}

// 앞구르기 
void ABossCharacter::Rolling(float deltaSeconds)
{		
	if (playerCharacter->currentHP <= 0)
	{
		bossState = EBossState::PLAYERKILL;
	}
	currentTime += deltaSeconds;
	// 특정 STATE와 중복되지 않게 막기 
	if (bossState == EBossState::BLOCK || bossState == EBossState::DAMAGED || bossState == EBossState::BLOCKATTACK ) {return;}

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

	if(currentTime > 0.7f)
		{	
			// 플레이어가 공격범위 안에 있다면 + 공격당하지 않았다면 > 어택준비로 전환
			if (targetDir.Length() <= attackDistance && bIsAttacked == false)
			{
				currentTime = 0;
				bossState = EBossState::ATTACKREADY;
			}
			// 플레이어가 공격범위 안에 없다면 다시 쫓아감 
			else if (targetDir.Length() > attackDistance)
			{
				currentTime = 0;
				bossState = EBossState::MOVE;
			}
		}
}
 //ATTACK 후 잠시 Delay
void ABossCharacter::AttackDelay(float deltaSeconds)
{			
	if (playerCharacter->currentHP <= 0)
	{
		bossState = EBossState::PLAYERKILL;
	}
	currentTime += deltaSeconds;
	if (currentTime > attackDelayTime)
	{
		if (bIsAttacked == false)
		{
			//다음 공격 준비 
			currentTime = 0;
			bossState = EBossState::ATTACKREADY;
		}
		else if (bIsAttacked == true)
		{	//공격받았다면 다시 플레이어 찾음 
			currentTime = 0;
			CheckDistance();
		}
	}
}
//방어 
void ABossCharacter::Blocking(float deltaSeconds)
{	
	if (playerCharacter->currentHP <= 0)
	{
		bossState = EBossState::PLAYERKILL;
	}
	//쉴드가 생성된 상황이 아니라면 
	if (shieldSpawn == false)
	{
		//쉴드 연속으로 생성하지 않도록, 쉴드 생성 후 shieldSpawn = ture 체크 
		shieldSpawn = true;

		//보스 피가 800 이하로 깎였을 때 부터 방어 시작
		//제자리에서 쉴드 생성
		GetWorld()->SpawnActor<AShield>(shield, GetActorLocation(), GetActorRotation());
		SetActorLocation(GetActorLocation());
		if (shieldSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), shieldSound, GetActorLocation(), GetActorRotation());
		}
	}	
	//BLOCKATTACK 
	currentTime += deltaSeconds;
	if (currentTime > 1.0f)
		{	
			currentTime = 0;
			bossState = EBossState::BLOCKATTACK;
			//쉴드 생성된 상황 아님으로 다시 설정 
			shieldSpawn = false;
		}
}
//방어 후 바로 방어공격 
void ABossCharacter::BlocKAttack(float deltaSeconds)
{	
	if (playerCharacter->currentHP <= 0)
	{
		bossState = EBossState::PLAYERKILL;
	}
	//방어공격 애니메이션 실행시간 맞추기 위한 시간재기 
	currentTime += deltaSeconds;
	if (currentTime > 1.0f)
	{
		for (TActorIterator<AShield> it(GetWorld()); it; ++it)
		{
			sh = *it;
			if (sh != nullptr)
			{
				sh->ShieldExtending();
			}
		}
	}
}

void ABossCharacter::ShootingAttack(float deltaSeconds)
{	
	if (playerCharacter->currentHP <= 0)
	{
		bossState = EBossState::PLAYERKILL;
	}
	// teleport1 위치로 가서 슈팅 시작 
	for (TActorIterator<AShootingStartLocationActor> iter(GetWorld()); iter; ++iter)
	{
		shootingLoc = *iter;
	
		if (shootingLoc != nullptr)
		{
		//빛공격 활성화
		shootingLoc->bShoting=true;
		}
	
		// 6초 뒤 슈팅 끝 + 땅 아래로 내려와서(teleport2 위치) 다시 MOVE상태로 전환 
		currentTime+=deltaSeconds;
		if (currentTime > shootingTime)
		{	
			if(shootingLoc != nullptr)
			{ 
			//빛공격 비활성화
				shootingLoc->bShoting=false;
			}
			//땅 아래로 돌아갈 위치용 액터 찾기
			for (TActorIterator<ABossReturnLocationActor> iters(GetWorld()); iters; ++iters)
			{	
				returnLoc = *iters;
			}
			if (returnLoc != nullptr)
			{	//텔레포트 
				SetActorLocation(returnLoc->GetActorLocation());
				if (bFirstShooting == false)
				{	
					bFirstShooting = true;
					CheckDistance();
				}
				else if (bFirstShooting == true && bSecondShooting == false)
				{
					bSecondShooting = true;
					CheckDistance();
				}
			}
		}
	}
}

void ABossCharacter::DamageProcess(float deltaSeconds)
{	
	if (playerCharacter->currentHP <= 0)
	{
		bossState = EBossState::PLAYERKILL;
	}
	//페이즈 전환과정 중에는 데미지 입지 않게 설정 
	if(bPhaseChanging == true){return;}

		currentTime += deltaSeconds;
		if (currentTime > 1.0f)
		{
			currentTime = 0;
			if (currentHP > 4800)
			{
				//피격모션 후 IDLE 
				bossState = EBossState::IDLE;
				bIsAttacked = false;
			}
			else if (currentHP <= 4800 && bFirstPhase == false)
			{
				currentTime = 0;
				bossState = EBossState::PHASECHANGE;
				bIsAttacked = false;
			}
			else if (currentHP < 4800 && currentHP > 4000 && bFirstPhase == true)
			{
				//피격모션 후 플레이어 쫓아감
				currentTime = 0;
				bossState = EBossState::MOVE;
				bIsAttacked = false;
			}
			//슈팅페이즈 진입 
			else if (currentHP <= 4000 && bFirstShooting == false)
			{	
				currentTime = 0;
				bPhaseChanged = true;
				bIsAttacked = false;
				bossState = EBossState::PHASECHANGE;
			}
			else if (currentHP < 4000 && currentHP > 3000 && bFirstShooting == true)
			{
				//피격모션 후 플레이어 쫓아감
				bossState = EBossState::MOVE;
				bIsAttacked = false;
			}
			//슈팅페이즈 진입 
			else if (currentHP <= 3000 && bSecondShooting == false)
			{
				currentTime = 0;
				bPhaseChanged = true;
				bIsAttacked = false;
				bossState = EBossState::PHASECHANGE;
			}
			else if (currentHP < 3000 && bSecondShooting == true)
			{
				//피격모션 후 플레이어 쫓아감
				bossState = EBossState::MOVE;
				bIsAttacked = false;
			}
		}
}
//페이즈 모션 
void ABossCharacter::Phasing(float deltaSeconds)
{		
	bFirstPhase = true;
	if (playerCharacter->currentHP <= 0)
	{
		bossState = EBossState::PLAYERKILL;
	}
	PlayAnimMontage(phaseChaingingMontage);
	//슈팅페이즈 조건이 할당되지 않았다면 
	if (bPhaseChanged == false)
	{	//움직이기 + 공격 시작 
		APlayerCharacter* player = Cast<APlayerCharacter> (target);
		if (player != nullptr)
		{
			player->SwitchCameraToBoss();
		}

		currentTime += deltaSeconds;
		if(currentTime > 2.0f)
		{ 
		bossState = EBossState:: MOVE;
		}
	}
	//슈팅페이즈 조건이 할당 되었다면 
	else if(bPhaseChanged == true)
	{ 
		if (bFirstShooting == false)
		{
		//텔레포트할 위치용 액터를 찾음 
		for (TActorIterator<ABossTeleportLocationActor> iter(GetWorld()); iter; ++iter)
			{
				teleportLoc = *iter;

				if (teleportLoc != nullptr)
				{	// 텔레포트 
					SetActorLocation(teleportLoc->GetActorLocation());

					//카메라전환
					APlayerCharacter* player = Cast<APlayerCharacter>(target);
					if (player != nullptr)
					{
						player->SwitchCameraToBoss();
					}
					currentTime += deltaSeconds;
					if (currentTime > 2.5f)
					{	//2초 뒤 슈팅페이즈 시작 
						bossState = EBossState::SHOOTATTACK;
					}
				}
			}
		}
		else if (bSecondShooting == false)
		{	//텔레포트할 위치용 액터를 찾음 
			for (TActorIterator<ABossTeleportLocationActor> iter(GetWorld()); iter; ++iter)
			{
				teleportLoc = *iter;

				if (teleportLoc != nullptr)
				{	// 텔레포트 
					SetActorLocation(teleportLoc->GetActorLocation());

					//카메라전환
					APlayerCharacter* player = Cast<APlayerCharacter>(target);
					if (player != nullptr)
					{
						player->SwitchCameraToBoss();
					}
					currentTime += deltaSeconds;
					if (currentTime > 2.5f)
					{	//2초 뒤 슈팅페이즈 시작 
						bossState = EBossState::SHOOTATTACK;
					}
				}
			}
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
//페이즈 진입과정 > 데미지 적용되지 않도록 설정
void ABossCharacter::PhaseChangeStart()
{
	bPhaseChanging = true;
}
//페이즈 진입과정 끝나면 데미지 다시 적용 
void ABossCharacter::PhaseChangeEnd()
{
	bPhaseChanging = false;
}
//보스 데미지받는 함수
void ABossCharacter::OnDamaged(int32 dmg)
{	//보스 텔레포트+슈팅 하는 페이즈 '돌입과정' 에서는 데미지 적용 X
	if (bPhaseChanging == true) {return;}

	//보스가 공격중 / 방어중 / 페이즈 전환중 일 땐 데미지 적용 X 
	if (bossState == EBossState::ATTACK || bossState == EBossState::ATTACK2
		|| bossState == EBossState::JUMPATTACK || bossState == EBossState::BLOCK 
		|| bossState == EBossState::BLOCKATTACK || bossState == EBossState::PHASECHANGE){return;}

	//공격 받음 true 체크 
	bIsAttacked = true;
	//HP값이 0 ~ maxHP 값 사이에만 있을 수 있게 설정
	currentHP = FMath::Clamp(currentHP - dmg, 0, maxHP);

	if(bossState == EBossState::SHOOTATTACK) {return;}

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

void ABossCharacter::OnDamagedByPod(int32 dmg)
{
	if (bPhaseChanging == true) { return; }

	//보스가 공격중 / 방어중 / 페이즈 전환중 일 땐 데미지 적용 X 
	if (bossState == EBossState::ATTACK || bossState == EBossState::ATTACK2
		|| bossState == EBossState::JUMPATTACK || bossState == EBossState::BLOCK
		|| bossState == EBossState::BLOCKATTACK || bossState == EBossState::PHASECHANGE) {
		return;
	}

	//HP값이 0 ~ maxHP 값 사이에만 있을 수 있게 설정
	currentHP = FMath::Clamp(currentHP - dmg, 0, maxHP);

	if (bossUI != nullptr)
	{	//체력UI 깎임 
		bossUI->SetHealthBar((float)currentHP / (float)maxHP);
	}
}

//플레이어 데미지 주기
void ABossCharacter::OnDealDamageOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SeewpResult)
{
	// 왼발, 오른발 공격 콜리전에 플레이어가 닿았다면
	APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);
	if (player != nullptr)
	{	
		if (bPlayerAttacekd == false)
		{
			bPlayerAttacekd = true;
			player->PlayerDamaged(damage);
			FTimerHandle damageHandle;
			GetWorldTimerManager().SetTimer(damageHandle, FTimerDelegate::CreateLambda([&]() {
				bPlayerAttacekd = false;
				}), 0.5f, false);

			if (damageFX != nullptr)
			{
				damageFX->SetActorLocation(player->GetActorLocation());
				damageFX->PlayFX();
			}
			if (hitSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), hitSound, player->GetActorLocation(), player->GetActorRotation());
			}
		}
	}
}

void ABossCharacter::Die()
{	
	
	pc = GetController<APlayerController>();
	if (pc != nullptr)
	{
		pc->PlayerCameraManager->StartCameraFade(0, 1, 3.0f, FLinearColor::Black);
	}

	//죽는애님몽타주 실행 
	PlayAnimMontage(death_Montage);
	//콜리전 다 꺼버림
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	leftFootCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	rightFootCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetCharacterMovement()->DisableMovement();

	//1초 뒤 Destroy
	// 포드랑 9S(아군AI)에서 널포인트오류 낼 수 있을 것 같아서 일단 주석처리중
	
//	FTimerHandle deathTimer;
//	GetWorldTimerManager().SetTimer(deathTimer, FTimerDelegate::CreateLambda([&](){
//		Destroy();
//		}), 1.0f, false);
	

  //게임클리어 UI 띄우기
	FTimerHandle uiHandle;
	GetWorldTimerManager().SetTimer(uiHandle, FTimerDelegate::CreateLambda([&]() {

		if (nierGM != nullptr)
		{
			nierGM->ShowClearedUI();
		}
		}), 2.0f, false);

}

void ABossCharacter::PlayerKilled()
{
	GetCharacterMovement()->DisableMovement();

}
