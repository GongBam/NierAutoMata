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
    SpringArm->TargetArmLength = 300.0f;
    SpringArm->bUsePawnControlRotation = true;

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
    Camera->bUsePawnControlRotation = true;
    
    weaponComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Mesh Component"));
    weaponComp -> SetupAttachment(GetMesh(), FName("DrawSocket"));
    weaponComp -> SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

    boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Box Component"));
    boxComp->SetupAttachment(weaponComp);
    boxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
   

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

    UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
 
    if(enhancedInputComponent != nullptr)
    {
        enhancedInputComponent->BindAction(ia_move, ETriggerEvent::Triggered, this , &APlayerCharacter::PlayerMove);
        enhancedInputComponent->BindAction(ia_move, ETriggerEvent::Completed, this, &APlayerCharacter::PlayerMove);
        enhancedInputComponent->BindAction(ia_jump, ETriggerEvent::Started, this, &APlayerCharacter::PlayerJump);
        enhancedInputComponent->BindAction(ia_jump, ETriggerEvent::Completed, this, &APlayerCharacter::PlayerJumpEnd);
        enhancedInputComponent->BindAction(ia_shot, ETriggerEvent::Triggered, this, &APlayerCharacter::Shot);
        enhancedInputComponent->BindAction(ia_look, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
        enhancedInputComponent->BindAction(ia_damaging, ETriggerEvent::Started, this, &APlayerCharacter::DAMAGING);
        enhancedInputComponent->BindAction(ia_LeftAttack, ETriggerEvent::Started, this, &APlayerCharacter::LeftAttack);
        enhancedInputComponent->BindAction(ia_RightAttack, ETriggerEvent::Started, this, &APlayerCharacter::RightAttack);
    }
    
}

void APlayerCharacter::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ABossCharacter* boss = Cast<ABossCharacter>(OtherActor);
    if (boss != nullptr)
    {
        boss->OnDamaged(damage);
        UE_LOG(LogTemp, Warning, TEXT("%d"), damage);
    }
}

void APlayerCharacter::PlayerMove(const FInputActionValue& Value)
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
    DrawSword=true;
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

void APlayerCharacter::PlayerJump(const FInputActionValue& Value)
{
    Jump();
}

void APlayerCharacter::PlayerJumpEnd(const FInputActionValue& Value)
{
    StopJumping();
}


void APlayerCharacter::Shot(const FInputActionValue& Value)
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

void APlayerCharacter::LeftAttack(const FInputActionValue& Value)
{
  
    if(DrawSword)
    {
        PlayAnimMontage(Draw_montage,1.5f);
        DrawSword = false;
    }
    else{
        int32 left = FMath::RandRange(1,3);
        FString sectionName = FString("Left")+FString::FromInt(left);
        PlayAnimMontage(Left_montages,1.5f,FName(sectionName));
   
        UE_LOG(LogTemp, Warning, TEXT("%d"), left);

        damage = 10;
    }
}

void APlayerCharacter::RightAttack(const FInputActionValue& Value)
{
    if (DrawSword)
    {
        PlayAnimMontage(Draw_montage, 1.5f);
        DrawSword = false;
    }
    else
    {
        int32 right = FMath::RandRange(1,3);
        FString sectionName = FString("Right") + FString::FromInt(right);
        PlayAnimMontage(Right_montages, 1.5f, FName(sectionName));
        damage = 20;
    }
}

void APlayerCharacter::EndAttack()
{

}

// �ӽ÷� ���� �������ִ� �Լ� 
void APlayerCharacter::DAMAGING(const FInputActionValue& Value)
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
}

//�÷��̾� �״� �Լ� 
void APlayerCharacter::PlayerDie()
{

    
}