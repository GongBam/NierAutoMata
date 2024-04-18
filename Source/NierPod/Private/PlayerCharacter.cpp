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

    //플레이어 시야 
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

    // 체력바 UI 위젯 캐싱 
    playerUI = Cast<UPlayerHealthWidget>(playerwidgetComp->GetWidget());

    //체력바 UI 위젯 붙임 
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

// 임시로 보스 데미지주는 함수 
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
}

//플레이어 죽는 함수 
void APlayerCharacter::PlayerDie()
{

    
}