

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

	//���ݹ޾Ҵ��� ���� false �ʱ�ȭ
	bIsAttacked = false;

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

void ABossCharacter::Idle(float DeltaSeconds)
{	
	//���� �ǰ� 900 ���϶�� MOVE ���·� ��ȯ - ������ ����
	if (currentHP <= 900.0f)
	{
		bossState = EBossState::MOVE;
	}
}

void ABossCharacter::AttackReady()
{
	if (bossState == EBossState::BLOCK) { return; }

	UE_LOG(LogTemp, Warning, TEXT("READY TO ATTACK PLAYER"));
	//�÷��̾� �Ÿ��� attackDistance ���� ������
	if (FVector::Distance(GetActorLocation(), target->GetActorLocation()) < attackDistance)
		{	//���� ������ - �⺻���� 
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
	//�÷��̾� �Ÿ��� attackDistance ���� �ָ� ������ �ٽ� �Ѿư�
	else
	{
		bossState = EBossState::MOVE;
	}
}

void ABossCharacter::Attack()
{
	if (bossState == EBossState::BLOCK) {return;}

	//�÷��̾� �Ÿ��� attackDistance ���� ������
	if (FVector::Distance(GetActorLocation(), target->GetActorLocation()) < attackDistance + 10.0f)
	{	

		UE_LOG(LogTemp, Warning, TEXT("Boss Kick!"));
		bossState = EBossState::ATTACKDELAY;
	}
	//�÷��̾� �Ÿ��� attackDistance ���� ũ��
	else
	{	//�÷��̾� �ٽ� ����
		bossState = EBossState::MOVE;
	}
}

void ABossCharacter::Attack2()
{
	if (bossState == EBossState::BLOCK) {return;}

	//�÷��̾� �Ÿ��� attackDistance ���� ������
	if (FVector::Distance(GetActorLocation(), target->GetActorLocation()) < attackDistance + 10.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Boss Kick 2 !"));
		bossState = EBossState::ATTACKDELAY;
	}
	//�÷��̾� �Ÿ��� attackDistance ���� ũ��
	else
	{	//�÷��̾� �ٽ� ����
		bossState = EBossState::MOVE;
	}
}

void ABossCharacter::JumpAttack()
{
	if (bossState == EBossState::BLOCK) { return; }

	//�÷��̾� �Ÿ��� attackDistance ���� ������
	if (FVector::Distance(GetActorLocation(), target->GetActorLocation()) < attackDistance + 10.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Boss Jump Kick !"));
		bossState = EBossState::ATTACKDELAY;
	}
	//�÷��̾� �Ÿ��� attackDistance ���� ũ��
	else
	{	//�÷��̾� �ٽ� ����
		bossState = EBossState::MOVE;
	}
}
//�÷��̾� �Ѿư��� �Լ�
void ABossCharacter::MoveToTarget(float deltaSeconds)
{	
	//�÷��̾� �������
	FVector targetDir = target->GetActorLocation() - GetActorLocation();
	targetDir.Z = 0;

	// �÷��̾ ���ݹ��� ���̶��
	if (targetDir.Length() > attackDistance)
	{
		//�÷��̾� ����
		GetCharacterMovement()->MaxWalkSpeed = traceSpeed;
		AddMovementInput(targetDir.GetSafeNormal());

		//�̵� �������� ȸ��
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
	// �÷��̾ ���ݹ��� �ȿ� �ִٸ� + ���ݴ����� �ʾҴٸ� 
	if (targetDir.Length() <= attackDistance && bIsAttacked == false)
	{	// �����غ�� ��ȯ
		bossState = EBossState::ATTACKREADY;
	}
}

// �ձ����� 
void ABossCharacter::Rolling(float deltaSeconds)
{
		
}
//ATTACK �� ��� Delay
void ABossCharacter::AttackDelay(float deltaSeconds)
{	
	currentTime += deltaSeconds;
	if (currentTime > attackDelayTime && bIsAttacked == false)
	{	
		//���� ���� �غ� 
		currentTime = 0;
		bossState = EBossState::ATTACKREADY;
		//�÷��̾ �ݰ� ����� �ٽ� �Ѿư� 
		if(FVector::Distance(GetActorLocation(), target->GetActorLocation()) > attackDistance + 10.f)
		{	
			if(currentTime > attackDelayTime)
			{	
			currentTime = 0;
			bossState = EBossState::MOVE;
			}
		}
	}
}

void ABossCharacter::Blocking(float deltaSeconds)
{	//���� �ǰ� 800 ���Ϸ� ���� �� ���� ��� ����
		//���ڸ����� ���� ����
		GetWorld()->SpawnActor<AShield>(shield, GetActorLocation(), GetActorRotation());
		SetActorLocation(GetActorLocation());
		//2 �� ������ �� (Shield : 1.5�� �� ������ �����)
		FTimerHandle blockingTimer;
		GetWorldTimerManager().SetTimer(blockingTimer, FTimerDelegate::CreateLambda([&]()
			{
				bossState = EBossState::BLOCKATTACK;
			}), 1.0f, false);
		
}

void ABossCharacter::BlocKAttack()
{
	//������� �� MOVE ��ȯ
	UE_LOG(LogTemp, Warning, TEXT("SHIELD ATTACK"));
	bossState = EBossState::ATTACKDELAY;
}

void ABossCharacter::DamageProcess(float deltaSeconds)
{
	bIsAttacked = false;

	//���� ü���� 900 ���ϰ� �ƴ϶�� 
	if(currentHP > 900)
	{	
		//�ǰݸ�� �� IDLE 
		FTimerHandle hitTimer;
		GetWorldTimerManager().SetTimer(hitTimer, FTimerDelegate::CreateLambda([&]()
			{
				bossState = EBossState::IDLE;
			}), 1.0f, false);

	}
	//���� ü���� 900 ���϶�� 
	if(currentHP <= 900)
	{ 	
		//�ǰݸ�� �� �÷��̾� �Ѿư� 
		FTimerHandle hitTimer;
		GetWorldTimerManager().SetTimer(hitTimer, FTimerDelegate::CreateLambda([&]() 
		{
		bossState = EBossState::MOVE;
		}), 1.0f, false);
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

//���� �������޴� �Լ�
void ABossCharacter::OnDamaged(int32 dmg)
{	
	//������ ������ / �ǰݹ��� ���� / ����� �� �� ������ ���� X 
	if (bossState == EBossState::ATTACK){return;}
	if (bossState == EBossState::ATTACK2){return;}
	if (bossState == EBossState::JUMPATTACK){return;}
	if (bossState == EBossState::BLOCK){return;}
	if (bossState == EBossState::BLOCKATTACK){return;}
	if (bossState == EBossState::DAMAGED){return;}

	//���� ���� true üũ 
	bIsAttacked = true;

	//HP���� 0 ~ maxHP �� ���̿��� ���� �� �ְ� ����
	currentHP = FMath::Clamp(currentHP - dmg, 0, maxHP);
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

//�÷��̾� ������ �ֱ�
void ABossCharacter::OnDealDamageOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SewwpResult)
{
	// �޹�, ������ ���� �ݸ����� �÷��̾ ��Ҵٸ�
	APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);
	if (player != nullptr)
	{	
		player->PlayerDamaged();
	}
}

void ABossCharacter::Die()
{	
	//�״¾ִԸ�Ÿ�� ���� 
	PlayAnimMontage(death_Montage);
	//�ݸ��� �� ������
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	leftFootCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	rightFootCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetCharacterMovement()->DisableMovement();
}
