

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

	//�޹� ���� �ݸ���
	leftFootCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftFoot Attack Collision"));
	leftFootCollision -> SetupAttachment(GetMesh(), TEXT("LeftToeBaseSocket"));
	leftFootCollision -> SetRelativeLocation(FVector(0, 10, 0));
	leftFootCollision -> SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));
	//������ ���� �ݸ���
	rightFootCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RightFoot Attack Collision"));
	rightFootCollision -> SetupAttachment(GetMesh(), TEXT("RightFootToeBaseSocket"));
	rightFootCollision -> SetRelativeLocation(FVector(-10, -20, 0));
	rightFootCollision -> SetRelativeScale3D(FVector(0.2f,0.2f,0.2f));

	//ü�¹�����
	bosswidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Boss Health Widget Component"));

	//���� �ݸ��� ������������ ���ε� 
	leftFootCollision -> OnComponentBeginOverlap.AddDynamic(this, &ABossCharacter::OnDealDamageOverlapBegin);
	rightFootCollision -> OnComponentBeginOverlap.AddDynamic(this, &ABossCharacter::OnDealDamageOverlapBegin);


	//�⺻ ĸ��������Ʈ preset ����
	GetCapsuleComponent()->SetCollisionProfileName(FName("BossPreset"));

	//�޹�, ������ ���� �ݸ��� �� �� ���Ͽ� ���� 
	leftFootCollision->SetCollisionProfileName(FName("BossAttackPreset"));
	rightFootCollision->SetCollisionProfileName(FName("BossAttackPreset"));

	

}

void ABossCharacter::BeginPlay()
{
	Super::BeginPlay();

	//���忡 �ִ� �÷��̾� ã�� target �� �־�� 
	for (TActorIterator<APlayerCharacter> player(GetWorld()); player; ++player)
	{
		target = *player;
	}

	//�⺻���� IDLE ���·� �ʱ�ȭ
	bossState = EBossState::IDLE;
	
	//���� ü�� max �� �ʱ�ȭ
	currentHP = maxHP;

	//���� ��� �ʱ�ȭ
	bIsAttacked = false;
	bFirstPhase = false;
	bFirstShooting = false;
	bSecondShooting = false;

	//ü�¹� UI ���� �޾ƿ��� ����Ʈ�� ����
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


	//GameModeBase ĳ�� 
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
	//�÷��̾� �Ÿ��� attackDistance ���� �ָ� ������ �ٽ� �Ѿư�
	if (FVector::Distance(GetActorLocation(), target->GetActorLocation()) > attackDistance &&
		FVector::Distance(GetActorLocation(), target->GetActorLocation()) < rollingDistance)
	{
		bossState = EBossState::MOVE;
	}
	//�÷��̾� �Ÿ��� ���� �ָ� �ѹ� ���� 
	else if (FVector::Distance(GetActorLocation(), target->GetActorLocation()) > rollingDistance)
	{
		bossState = EBossState::ROLLING;
	}
}

void ABossCharacter::Idle(float DeltaSeconds)
{	
	//���� �ǰ� 900 ���϶�� MOVE ���·� ��ȯ - ù ������ ����
	if (currentHP <= 4800.0f)
	{
		bossState = EBossState::MOVE;
	}

}

void ABossCharacter::AttackReady()
{	// Ư�� STATE �� �ߺ����� �ʰ� ���� 

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
		//�÷��̾� �Ÿ��� attackDistance ���� ������
		if (FVector::Distance(GetActorLocation(), target->GetActorLocation()) < attackDistance)
			{	//���� ������ 
				int32 num = FMath::RandRange(1,3);
				if(num == 1)
				{	
					bossState = EBossState::ATTACK;
				}
				else if(num == 2)
				{
					bossState = EBossState::ATTACK2;
				}
				//���������� ����ü�� 500 ���Ͽ������� ���� 
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
//�Ϲݰ���1
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
	//�÷��̾� �Ÿ��� ���ݹ��� �ȿ� ������ ���� �� ������ 
	if (FVector::Distance(GetActorLocation(), target->GetActorLocation()) < attackDistance + 10.0f)
	{
		bossState = EBossState::ATTACKDELAY;
	}
	else
	{
		//�÷��̾���� �Ÿ��� ���ݰ��� �������� �ִٸ� �Ÿ�����ؼ� MOVE Ȥ�� ROLLING>MOVE 
		CheckDistance();
	}
}
//�Ϲݰ���2
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
	//�÷��̾� �Ÿ��� ���ݹ��� �ȿ� ������ ���� �� ������ 
	if (FVector::Distance(GetActorLocation(), target->GetActorLocation()) < attackDistance + 10.0f)
	{
		bossState = EBossState::ATTACKDELAY;
	}
	else
	{	//�÷��̾���� �Ÿ��� ���ݰ��� �������� �ִٸ� �Ÿ�����ؼ� MOVE Ȥ�� ROLLING>MOVE 
		CheckDistance();
	}
}
//����ü�� 500���Ϻ��� �������� ���� 
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

	//�÷��̾� �Ÿ��� ���ݹ��� �ȿ� ������ ���� �� ������ 
	if (FVector::Distance(GetActorLocation(), target->GetActorLocation()) < attackDistance + 10.0f)
	{
		bossState = EBossState::ATTACKDELAY;
	}
	else
	{	//�÷��̾���� �Ÿ��� ���ݰ��� �������� �ִٸ� �Ÿ�����ؼ� MOVE Ȥ�� ROLLING>MOVE 
		CheckDistance();
	}
}
//�÷��̾� �Ѿư��� �Լ�
void ABossCharacter::MoveToTarget(float deltaSeconds)
{	
	if (playerCharacter->currentHP <= 0)
	{
		bossState = EBossState::PLAYERKILL;
	}
	//�÷��̾� �������
	FVector targetDir = target->GetActorLocation() - GetActorLocation();
	targetDir.Z = 0;

	// �÷��̾ ���ݹ��� �ȿ� �ִٸ� + ���ݴ����� �ʾҴٸ� 
	if (targetDir.Length() <= attackDistance && bIsAttacked == false)
	{	// �����غ�� ��ȯ
		
		bossState = EBossState::ATTACKREADY;
	}
	// �÷��̾ ���ݹ��� ���̶��
	if (targetDir.Length() > attackDistance && targetDir.Length() < rollingDistance)
	{
		//�÷��̾� ����
		GetCharacterMovement()->MaxWalkSpeed = traceSpeed;
		AddMovementInput(targetDir.GetSafeNormal());

		//�̵� �������� ȸ��
		FRotator currentRot = GetActorRotation();
		FRotator targetRot = targetDir.ToOrientationRotator();
		FRotator calcRot = FMath::Lerp(currentRot, targetRot, deltaSeconds * rotSpeed);
		SetActorRotation(calcRot);
		
		//Move �� ���� �ð��� ������ �����·� ����
		currentTime += deltaSeconds;
		if(currentTime > 4.0f)
		{	
			currentTime = 0;
			bossState = EBossState::BLOCK;
		}
	}
	else if (targetDir.Length() > rollingDistance)
	{	//�÷��̾ �� �� ������ �ִٸ� �ձ����� ���� 
		bossState = EBossState::ROLLING;
	}
}

// �ձ����� 
void ABossCharacter::Rolling(float deltaSeconds)
{		
	if (playerCharacter->currentHP <= 0)
	{
		bossState = EBossState::PLAYERKILL;
	}
	currentTime += deltaSeconds;
	// Ư�� STATE�� �ߺ����� �ʰ� ���� 
	if (bossState == EBossState::BLOCK || bossState == EBossState::DAMAGED || bossState == EBossState::BLOCKATTACK ) {return;}

	//�÷��̾� �������
	FVector targetDir = target->GetActorLocation() - GetActorLocation();
	targetDir.Z = 0;

	//�÷��̾� ������ ���� 
	GetCharacterMovement()->MaxWalkSpeed = 1000;
	AddMovementInput(targetDir.GetSafeNormal());

	//�̵� �������� ȸ��
	FRotator currentRot = GetActorRotation();
	FRotator targetRot = targetDir.ToOrientationRotator();
	FRotator calcRot = FMath::Lerp(currentRot, targetRot, deltaSeconds * rotSpeed);
	SetActorRotation(calcRot);

	if(currentTime > 0.7f)
		{	
			// �÷��̾ ���ݹ��� �ȿ� �ִٸ� + ���ݴ����� �ʾҴٸ� > �����غ�� ��ȯ
			if (targetDir.Length() <= attackDistance && bIsAttacked == false)
			{
				currentTime = 0;
				bossState = EBossState::ATTACKREADY;
			}
			// �÷��̾ ���ݹ��� �ȿ� ���ٸ� �ٽ� �Ѿư� 
			else if (targetDir.Length() > attackDistance)
			{
				currentTime = 0;
				bossState = EBossState::MOVE;
			}
		}
}
 //ATTACK �� ��� Delay
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
			//���� ���� �غ� 
			currentTime = 0;
			bossState = EBossState::ATTACKREADY;
		}
		else if (bIsAttacked == true)
		{	//���ݹ޾Ҵٸ� �ٽ� �÷��̾� ã�� 
			currentTime = 0;
			CheckDistance();
		}
	}
}
//��� 
void ABossCharacter::Blocking(float deltaSeconds)
{	
	if (playerCharacter->currentHP <= 0)
	{
		bossState = EBossState::PLAYERKILL;
	}
	//���尡 ������ ��Ȳ�� �ƴ϶�� 
	if (shieldSpawn == false)
	{
		//���� �������� �������� �ʵ���, ���� ���� �� shieldSpawn = ture üũ 
		shieldSpawn = true;

		//���� �ǰ� 800 ���Ϸ� ���� �� ���� ��� ����
		//���ڸ����� ���� ����
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
			//���� ������ ��Ȳ �ƴ����� �ٽ� ���� 
			shieldSpawn = false;
		}
}
//��� �� �ٷ� ������ 
void ABossCharacter::BlocKAttack(float deltaSeconds)
{	
	if (playerCharacter->currentHP <= 0)
	{
		bossState = EBossState::PLAYERKILL;
	}
	//������ �ִϸ��̼� ����ð� ���߱� ���� �ð���� 
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
	// teleport1 ��ġ�� ���� ���� ���� 
	for (TActorIterator<AShootingStartLocationActor> iter(GetWorld()); iter; ++iter)
	{
		shootingLoc = *iter;
	
		if (shootingLoc != nullptr)
		{
		//������ Ȱ��ȭ
		shootingLoc->bShoting=true;
		}
	
		// 6�� �� ���� �� + �� �Ʒ��� �����ͼ�(teleport2 ��ġ) �ٽ� MOVE���·� ��ȯ 
		currentTime+=deltaSeconds;
		if (currentTime > shootingTime)
		{	
			if(shootingLoc != nullptr)
			{ 
			//������ ��Ȱ��ȭ
				shootingLoc->bShoting=false;
			}
			//�� �Ʒ��� ���ư� ��ġ�� ���� ã��
			for (TActorIterator<ABossReturnLocationActor> iters(GetWorld()); iters; ++iters)
			{	
				returnLoc = *iters;
			}
			if (returnLoc != nullptr)
			{	//�ڷ���Ʈ 
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
	//������ ��ȯ���� �߿��� ������ ���� �ʰ� ���� 
	if(bPhaseChanging == true){return;}

		currentTime += deltaSeconds;
		if (currentTime > 1.0f)
		{
			currentTime = 0;
			if (currentHP > 4800)
			{
				//�ǰݸ�� �� IDLE 
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
				//�ǰݸ�� �� �÷��̾� �Ѿư�
				currentTime = 0;
				bossState = EBossState::MOVE;
				bIsAttacked = false;
			}
			//���������� ���� 
			else if (currentHP <= 4000 && bFirstShooting == false)
			{	
				currentTime = 0;
				bPhaseChanged = true;
				bIsAttacked = false;
				bossState = EBossState::PHASECHANGE;
			}
			else if (currentHP < 4000 && currentHP > 3000 && bFirstShooting == true)
			{
				//�ǰݸ�� �� �÷��̾� �Ѿư�
				bossState = EBossState::MOVE;
				bIsAttacked = false;
			}
			//���������� ���� 
			else if (currentHP <= 3000 && bSecondShooting == false)
			{
				currentTime = 0;
				bPhaseChanged = true;
				bIsAttacked = false;
				bossState = EBossState::PHASECHANGE;
			}
			else if (currentHP < 3000 && bSecondShooting == true)
			{
				//�ǰݸ�� �� �÷��̾� �Ѿư�
				bossState = EBossState::MOVE;
				bIsAttacked = false;
			}
		}
}
//������ ��� 
void ABossCharacter::Phasing(float deltaSeconds)
{		
	bFirstPhase = true;
	if (playerCharacter->currentHP <= 0)
	{
		bossState = EBossState::PLAYERKILL;
	}
	PlayAnimMontage(phaseChaingingMontage);
	//���������� ������ �Ҵ���� �ʾҴٸ� 
	if (bPhaseChanged == false)
	{	//�����̱� + ���� ���� 
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
	//���������� ������ �Ҵ� �Ǿ��ٸ� 
	else if(bPhaseChanged == true)
	{ 
		if (bFirstShooting == false)
		{
		//�ڷ���Ʈ�� ��ġ�� ���͸� ã�� 
		for (TActorIterator<ABossTeleportLocationActor> iter(GetWorld()); iter; ++iter)
			{
				teleportLoc = *iter;

				if (teleportLoc != nullptr)
				{	// �ڷ���Ʈ 
					SetActorLocation(teleportLoc->GetActorLocation());

					//ī�޶���ȯ
					APlayerCharacter* player = Cast<APlayerCharacter>(target);
					if (player != nullptr)
					{
						player->SwitchCameraToBoss();
					}
					currentTime += deltaSeconds;
					if (currentTime > 2.5f)
					{	//2�� �� ���������� ���� 
						bossState = EBossState::SHOOTATTACK;
					}
				}
			}
		}
		else if (bSecondShooting == false)
		{	//�ڷ���Ʈ�� ��ġ�� ���͸� ã�� 
			for (TActorIterator<ABossTeleportLocationActor> iter(GetWorld()); iter; ++iter)
			{
				teleportLoc = *iter;

				if (teleportLoc != nullptr)
				{	// �ڷ���Ʈ 
					SetActorLocation(teleportLoc->GetActorLocation());

					//ī�޶���ȯ
					APlayerCharacter* player = Cast<APlayerCharacter>(target);
					if (player != nullptr)
					{
						player->SwitchCameraToBoss();
					}
					currentTime += deltaSeconds;
					if (currentTime > 2.5f)
					{	//2�� �� ���������� ���� 
						bossState = EBossState::SHOOTATTACK;
					}
				}
			}
		}
	}
}

void ABossCharacter::BoxCollisionExtending()
{	//���� �ִϸ��̼ǽ� �� �ݸ��� Ŀ�� 
	rightFootCollision->SetRelativeScale3D(FVector(3.0f));
	leftFootCollision->SetRelativeScale3D(FVector(3.0f));
}

void ABossCharacter::BoxCollisionReset()
{	//���� �ִϸ��̼� ������ �� �ݸ��� �����·� ���ư� 
	rightFootCollision->SetRelativeScale3D(FVector(0.2f));
	leftFootCollision->SetRelativeScale3D(FVector(0.2f));
}
//������ ���԰��� > ������ ������� �ʵ��� ����
void ABossCharacter::PhaseChangeStart()
{
	bPhaseChanging = true;
}
//������ ���԰��� ������ ������ �ٽ� ���� 
void ABossCharacter::PhaseChangeEnd()
{
	bPhaseChanging = false;
}
//���� �������޴� �Լ�
void ABossCharacter::OnDamaged(int32 dmg)
{	//���� �ڷ���Ʈ+���� �ϴ� ������ '���԰���' ������ ������ ���� X
	if (bPhaseChanging == true) {return;}

	//������ ������ / ����� / ������ ��ȯ�� �� �� ������ ���� X 
	if (bossState == EBossState::ATTACK || bossState == EBossState::ATTACK2
		|| bossState == EBossState::JUMPATTACK || bossState == EBossState::BLOCK 
		|| bossState == EBossState::BLOCKATTACK || bossState == EBossState::PHASECHANGE){return;}

	//���� ���� true üũ 
	bIsAttacked = true;
	//HP���� 0 ~ maxHP �� ���̿��� ���� �� �ְ� ����
	currentHP = FMath::Clamp(currentHP - dmg, 0, maxHP);

	if(bossState == EBossState::SHOOTATTACK) {return;}

	if (bossUI != nullptr)
	{	//ü��UI ���� 
		bossUI->SetHealthBar((float)currentHP / (float)maxHP);
	}

	// ������ ��� ���, ���� ü���� 0 ���� ũ��
	if (currentHP > 0)
	{	
			//DamageProcess �� ��ȯ
			bossState = EBossState::DAMAGED;
			hitLocation = GetActorLocation();
	}
	// ������ ��� ���, ���� ü���� 0 ���϶�� 
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

	//������ ������ / ����� / ������ ��ȯ�� �� �� ������ ���� X 
	if (bossState == EBossState::ATTACK || bossState == EBossState::ATTACK2
		|| bossState == EBossState::JUMPATTACK || bossState == EBossState::BLOCK
		|| bossState == EBossState::BLOCKATTACK || bossState == EBossState::PHASECHANGE) {
		return;
	}

	//HP���� 0 ~ maxHP �� ���̿��� ���� �� �ְ� ����
	currentHP = FMath::Clamp(currentHP - dmg, 0, maxHP);

	if (bossUI != nullptr)
	{	//ü��UI ���� 
		bossUI->SetHealthBar((float)currentHP / (float)maxHP);
	}
}

//�÷��̾� ������ �ֱ�
void ABossCharacter::OnDealDamageOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SeewpResult)
{
	// �޹�, ������ ���� �ݸ����� �÷��̾ ��Ҵٸ�
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

	//�״¾ִԸ�Ÿ�� ���� 
	PlayAnimMontage(death_Montage);
	//�ݸ��� �� ������
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	leftFootCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	rightFootCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetCharacterMovement()->DisableMovement();

	//1�� �� Destroy
	// ����� 9S(�Ʊ�AI)���� ������Ʈ���� �� �� ���� �� ���Ƽ� �ϴ� �ּ�ó����
	
//	FTimerHandle deathTimer;
//	GetWorldTimerManager().SetTimer(deathTimer, FTimerDelegate::CreateLambda([&](){
//		Destroy();
//		}), 1.0f, false);
	

  //����Ŭ���� UI ����
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
