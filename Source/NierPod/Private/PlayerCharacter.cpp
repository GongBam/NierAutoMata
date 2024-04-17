#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "BossCharacter.h"
#include "PlayerPOD.h"
#include "PlayerHealthWidget.h"
#include "Components/WidgetComponent.h"
#include "EngineUtils.h"
#include "TEST2BAnimInstance.h"

APlayerCharacter::APlayerCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    //�÷��̾� �þ� 
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->TargetArmLength = 600.0f;
    SpringArm->bUsePawnControlRotation = true;

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
    Camera->bUsePawnControlRotation = true;

    boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Box Component"));
    boxComp -> SetupAttachment(GetMesh(), TEXT("WeaponSocket"));
    boxComp -> SetCollisionEnabled(ECollisionEnabled::NoCollision);

    weaponComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Mesh Component"));
    weaponComp -> SetupAttachment(boxComp);
    weaponComp -> SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

    playerwidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Player Health Widget Component"));

    //�÷��̾� �̵�, �þ� ���� 
    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;
    bUseControllerRotationYaw = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0, 540, 0);

    // ĳ������ �ִ� �̵� �ӷ°� ���ӷ��� �����Ѵ�. (cm/s)
    // 2B �ڵ����� �ٱ⿡ ������ �ɵ�
    GetCharacterMovement()->MaxWalkSpeed = 900.0f;
    GetCharacterMovement()->MaxAcceleration = 300.0f;

    // ������ ����
    GetCharacterMovement()->JumpZVelocity = 600.0f;
    GetCharacterMovement()->AirControl = 0.05f;

    // ���� ���� ���� ��
    JumpMaxCount = 2;
}


void APlayerCharacter::BeginPlay()
{
    Super::BeginPlay();


     //���� ���۽� currentHP > maxHP �� �ʱ�ȭ
    currentHP = maxHP;


    //�÷��̾� ��ǲ���� ����
    pc = GetController<APlayerController>();
    if(pc!=nullptr)
    {
        UEnhancedInputLocalPlayerSubsystem* subsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
        
        if(subsys != nullptr)
        {
            subsys->AddMappingContext(imc_KeyMap, 0);
        }
    }

    playerAnim = Cast<UTEST2BAnimInstance>(GetMesh()->GetAnimInstance());
    weaponComp->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnOverlap);

    // ü�¹� UI ���� ĳ�� 
    playerUI = Cast<UPlayerHealthWidget>(playerwidgetComp->GetWidget());

    //ü�¹� UI ���� ���� 
    if (playerHealthWidget_bp != nullptr)
    {
        playerUI = CreateWidget<UPlayerHealthWidget>(GetWorld(), playerHealthWidget_bp);
        if (playerUI != nullptr)
        {
            playerUI->AddToViewport();
        }
    }
}

void APlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

<<<<<<< Updated upstream
    UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
=======
    if (UEnhancedInputComponent* Input = CastChecked <UEnhancedInputComponent>(PlayerInputComponent))
    {   //��ǲ-�Լ� ���ε�
        Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
        Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
        Input->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerCharacter::Jump);
        Input->BindAction(DashAction, ETriggerEvent::Started, this, &APlayerCharacter::Dash);
        Input->BindAction(DodgeAction, ETriggerEvent::Started, this, &APlayerCharacter::DodgeFunction);
        Input->BindAction(LeftAttackAction, ETriggerEvent::Started, this, &APlayerCharacter::LeftAttack);
        Input->BindAction(RightAttackAction, ETriggerEvent::Started, this, &APlayerCharacter::RightAttack);
        Input->BindAction(shooting, ETriggerEvent::Triggered, this, &APlayerCharacter::Shot);
        Input->BindAction(damaging, ETriggerEvent::Started, this, &APlayerCharacter::DAMAGING);
    }
>>>>>>> Stashed changes

    if(enhancedInputComponent != nullptr)
    {
        enhancedInputComponent->BindAction(ia_move, ETriggerEvent::Triggered, this , &APlayerCharacter::PlayerMove);
        enhancedInputComponent->BindAction(ia_move, ETriggerEvent::Completed, this, &APlayerCharacter::PlayerMove);
        enhancedInputComponent->BindAction(ia_jump, ETriggerEvent::Started, this, &APlayerCharacter::PlayerJump);
        enhancedInputComponent->BindAction(ia_jump, ETriggerEvent::Completed, this, &APlayerCharacter::PlayerJumpEnd);
        enhancedInputComponent->BindAction(ia_shot, ETriggerEvent::Triggered, this, &APlayerCharacter::Shot);
        enhancedInputComponent->BindAction(ia_look, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
    }
    
}

void APlayerCharacter::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ABossCharacter* boss = Cast<ABossCharacter>(OtherActor);
    if (boss != nullptr)
    {

    }
}

<<<<<<< Updated upstream
void APlayerCharacter::PlayerMove(const FInputActionValue& Value)
=======
//�̵�
void APlayerCharacter::Move(const FInputActionValue& InputValue)
>>>>>>> Stashed changes
{
    FVector2D inputValue = Value.Get<FVector2D>();
    moveDirection = FVector(inputValue.Y, inputValue.X, 0);
    if(IsValid(Controller))
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw,0);

        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

        AddMovementInput(ForwardDirection, inputValue.Y);
        AddMovementInput(RightDirection, inputValue.X);

        if (playerAnim != nullptr)
        {
            playerAnim->moveDirection = moveDirection;
        }
    }
  
}


void APlayerCharacter::Look(const FInputActionValue& Value)
{
    FVector2D InputVector = Value.Get<FVector2D>();

    if (IsValid(Controller))
    {
        AddControllerYawInput(InputVector.X);
        AddControllerPitchInput(InputVector.Y);
    }
}

void APlayerCharacter::PlayerJump(const FInputActionValue& InputValue)
{
    Jump();
}

void APlayerCharacter::PlayerJumpEnd(const FInputActionValue& InputValue)
{
    StopJumping();
}


void APlayerCharacter::Shot(const FInputActionValue& InputValue)
{
    for(TActorIterator<APlayerPOD> it(GetWorld());it;++it)
      {
        APlayerPOD* podshot = *it;
        if(podshot !=nullptr)
        {
            podshot->Shooting();
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("null"));
        }
    }
}


<<<<<<< Updated upstream
=======
void APlayerCharacter::SetActionState(EStateType eState)
{
    if (eActionState != eState)
    {
        eActionState = eState;
    }
}

void APlayerCharacter::PerformDodge()
{
    SetActionState(EStateType::Nothing);

    if (DodgeMontage != nullptr)
    {
        PlayAnimMontage(DodgeMontage);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("DodgeMontage Is Null"));
    }
}

// �ӽ÷� ���� �������ִ� �Լ� 
void APlayerCharacter::DAMAGING(const FInputActionValue& InputValue)
{
   
    for(TActorIterator<ABossCharacter> iter(GetWorld()); iter; ++iter)
    {
        bossCharacter = *iter;
      if (bossCharacter != nullptr)
      {   
          bossCharacter->OnDamaged(damage + 50);
      }
    }
}

// PerformLightAttack
bool APlayerCharacter::PerformLightAttack(int32 attackIndex)
{
    UAnimMontage* selectedLightAttackMontage = lightAttackMontages[attackIndex];

    if (IsValid(selectedLightAttackMontage))
    {   
        SetActionState(EStateType::Attack);

        PlayAnimMontage(selectedLightAttackMontage);

        LightAttackIndex++;


        if (LightAttackIndex >= lightAttackMontages.Num())
        {
            LightAttackIndex = 0;
        }

        return true;
    }
    else
    {
        FString strMessage = FString(TEXT("Attack Montage Not Valid"));
        UKismetSystemLibrary::PrintString(GetWorld(), *strMessage);

        return false;
    }

    return false;
}

// Reset Light Attack Variables
void APlayerCharacter::ResetLightAttackVariables()
{
    LightAttackIndex = 0;
    LightAttackSaved = false;


}

bool APlayerCharacter::PerformHeavyAttack(int32 attackIndex)
{
    UAnimMontage* selectedHeavyAttackMontage = heavyAttackMontages[attackIndex];

    if (IsValid(selectedHeavyAttackMontage)) {
     
        SetActionState(EStateType::Attack);

        PlayAnimMontage(selectedHeavyAttackMontage);

        HeavyAttackIndex++;


        if (HeavyAttackIndex >= heavyAttackMontages.Num()) {
            HeavyAttackIndex = 0;
        }
        return true;
    }
    else {
        FString strMessage = FString(TEXT("Attack Montage Not Valid"));
        UKismetSystemLibrary::PrintString(GetWorld(), *strMessage);

        return false;
    }
    return false;
}

void APlayerCharacter::ResetHeavyAttackVariables()
{
    HeavyAttackIndex = 0;
    HeavyAttackSaved = false;

}

>>>>>>> Stashed changes
void APlayerCharacter::PlayerDamaged()
{
    currentHP = FMath::Clamp(currentHP - 10, 0, maxHP);
    //�¾��� �� (ü���� ���� 0���� ũ��)
        if(playerUI != nullptr)
        {
            playerUI->SetHealthBar((float)currentHP / (float)maxHP);
        }
    if (currentHP > 0)
    {
    }
    // �¾��� �� ü���� 0�̰ų� 0���� �۾�����
    if (currentHP <= 0)
    {
        PlayerDie();
    }
<<<<<<< Updated upstream

 //   UE_LOG(LogTemp, Warning, TEXT("Player HP : %d"), currentHP);
 }
=======
}

void APlayerCharacter::LeftAttack(const FInputActionValue& InputValue)
{
    UE_LOG(LogTemp, Warning, TEXT("LeftAttack"));

    PerformLightAttack(LightAttackIndex);

}

void APlayerCharacter::RightAttack(const FInputActionValue& InputValue)
{
    UE_LOG(LogTemp, Warning, TEXT("RightAttack"));

    PerformHeavyAttack(HeavyAttackIndex);

}

>>>>>>> Stashed changes

//�÷��̾� �״� �Լ� 
void APlayerCharacter::PlayerDie()
{

    
}