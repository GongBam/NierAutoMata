#include "PlayerCharacter.h"


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Animation/AnimMontage.h>
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "BossCharacter.h"
#include "PlayerPOD.h"
#include "EngineUtils.h"
#include "PlayerHealthWidget.h"
#include "Components/WidgetComponent.h"






APlayerCharacter::APlayerCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    //플레이어 시야 
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->TargetArmLength = 600.0f;
    SpringArm->bUsePawnControlRotation = true;

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
    Camera->bUsePawnControlRotation = false;

    boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Box Component"));
    boxComp -> SetupAttachment(GetMesh(), TEXT("WeaponSocket"));
    boxComp -> SetCollisionEnabled(ECollisionEnabled::NoCollision);

    weaponComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Mesh Component"));
    weaponComp -> SetupAttachment(boxComp);
    weaponComp -> SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

    playerwidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Player Health Widget Component"));

    //플레이어 이동, 시야 설정 
    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;
    bUseControllerRotationYaw = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0, 540, 0);

    // 캐릭터의 최대 이동 속력과 가속력을 설정한다. (cm/s)
    // 2B 자동으로 뛰기에 넣으면 될듯
    GetCharacterMovement()->MaxWalkSpeed = 900.0f;
    GetCharacterMovement()->MaxAcceleration = 300.0f;

    // 점프력 설정
    GetCharacterMovement()->JumpZVelocity = 600.0f;
    GetCharacterMovement()->AirControl = 0.05f;

    // 연속 점프 가능 수
    JumpMaxCount = 2;
}


void APlayerCharacter::BeginPlay()
{
    Super::BeginPlay();


     //게임 시작시 currentHP > maxHP 로 초기화
    currentHP = maxHP;


    //플레이어 인풋맵핑 생성
    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(InputMapping, 0);
        }
    }

    AnimInstance = GetMesh()->GetAnimInstance();
    weaponComp->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnOverlap);


    playerUI = Cast<UPlayerHealthWidget>(playerwidgetComp->GetWidget());

    //체력바 UI 위젯 
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
    // 딜레이 2초 주고 원래 속도로 되돌리기
    if (canDash == false) {
        DashCooldown -= DeltaTime;
        if (DashCooldown <= 0) {
            //GetCharacterMovement()->Velocity = OldVelocity;
            GetCharacterMovement()->MaxWalkSpeed = 900.0f;
            GetCharacterMovement()->MaxAcceleration = 300.0f;
            DashCooldown = 0.2f;
            canDash = true;
        }
    }


}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* Input = CastChecked <UEnhancedInputComponent>(PlayerInputComponent))
    {   //인풋-함수 바인드
        Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
        Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
        Input->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerCharacter::Jump);
        Input->BindAction(DashAction, ETriggerEvent::Started, this, &APlayerCharacter::Dash);
        Input->BindAction(DodgeAction, ETriggerEvent::Started, this, &APlayerCharacter::DodgeFunction);
        Input->BindAction(LeftAttackAction, ETriggerEvent::Started, this, &APlayerCharacter::LeftAttack);
        Input->BindAction(RightAttackAction, ETriggerEvent::Started, this, &APlayerCharacter::RightAttack);
        Input->BindAction(shooting, ETriggerEvent::Triggered, this, &APlayerCharacter::Shot);
    }

}

void APlayerCharacter::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ABossCharacter* boss = Cast<ABossCharacter>(OtherActor);
    if (boss != nullptr)
    {
        boss->OnDamaged(damage);
    }
}



//이동
void APlayerCharacter::Move(const FInputActionValue& InputValue)
{
    FVector2D InputVector = InputValue.Get<FVector2D>();

    if (IsValid(Controller))
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

        AddMovementInput(ForwardDirection, InputVector.Y);
        AddMovementInput(RightDirection, InputVector.X);
    }
}
//시야
void APlayerCharacter::Look(const FInputActionValue& InputValue)
{
    FVector2D InputVector = InputValue.Get<FVector2D>();

    if (IsValid(Controller))
    {
        AddControllerYawInput(InputVector.X);
        AddControllerPitchInput(InputVector.Y);
    }
}
//점프    // 더블점프는 나중에 가능하면 번역 
void APlayerCharacter::Jump()
{
    ACharacter::Jump();
    /*
    if (GetCharacterMovement()->IsFalling() != true)
    {
        ACharacter::Jump();
    }
    */
}

// 대쉬
void APlayerCharacter::Dash(const FInputActionValue& InputValue)
{
    if (canDash == true) {
        canDash = false;
        //OldVelocity = GetCharacterMovement()->Velocity;
        //GetCharacterMovement()->Velocity = GetActorForwardVector() * 5000.0f;
        // 최대속도 5000 주기
        GetCharacterMovement()->MaxWalkSpeed = 5000.0f;
        // 가속도 100000 주기 - 더 빠르게
        GetCharacterMovement()->MaxAcceleration = 100000.0f;
        AnimInstance->Montage_Play(DashMontage);
    }
}

void APlayerCharacter::DodgeFunction(const FInputActionValue& InputValue)
{
    bool bCondition = (eActionState == EStateType::Dodge);

    bCondition ^= true;

    if (bCondition)
    {
        PerformDodge();
    }
    else
    {
        bSaveDodge = true;
    }
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

void APlayerCharacter::PlayerDamaged()
{
    currentHP = FMath::Clamp(currentHP - 10, 0, maxHP);
    //맞았을 때 (체력이 아직 0보다 크면)
        if(playerUI != nullptr)
        {
            playerUI->SetHealthBar((float)currentHP / (float)maxHP);
        }
    if (currentHP > 0)
    {
    }
    // 맞았을 때 체력이 0이거나 0보다 작아지면
    if (currentHP <= 0)
    {
        PlayerDie();
    }

 //   UE_LOG(LogTemp, Warning, TEXT("Player HP : %d"), currentHP);



}

float APlayerCharacter::GetCurrentHealth()
{
    return float(currentHP);
}

float APlayerCharacter::GetMaxHealth()
{
    return float(maxHP);
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


//플레이어 죽는 함수 
void APlayerCharacter::PlayerDie()
{

    
}