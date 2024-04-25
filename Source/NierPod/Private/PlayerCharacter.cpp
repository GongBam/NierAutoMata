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
#include "Shield.h"
#include "NierGameModeBase.h"
#include "PlayerDamageEffectActor.h"
#include "Kismet/GameplayStatics.h"

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
    bIsDead = false;

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
    //데미지액터 미리 받아둠 
    for (TActorIterator<APlayerDamageEffectActor> iter(GetWorld()); iter; ++iter)
    {
        damageFX = *iter;
    }

    //GameModeBase 캐싱 
    AGameModeBase* gm = GetWorld()->GetAuthGameMode();
    nierGM = Cast<ANierGameModeBase>(gm);

}

void APlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(Loca==true)
    {
        //SetActorLocation(PlayerLocation);
    }
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
        enhancedInputComponent->BindAction(ia_SelfDamage, ETriggerEvent::Started, this, &APlayerCharacter::SelfDamaging);
        enhancedInputComponent->BindAction(ia_LeftAttack, ETriggerEvent::Started, this, &APlayerCharacter::LeftAttack);
        enhancedInputComponent->BindAction(ia_RightAttack, ETriggerEvent::Started, this, &APlayerCharacter::RightAttack);
        enhancedInputComponent->BindAction(ia_dodge, ETriggerEvent::Started, this, &APlayerCharacter::Dodge);
    }
    
}

void APlayerCharacter::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ABossCharacter* boss = Cast<ABossCharacter>(OtherActor);
    if (boss != nullptr)
    {
        boss->OnDamaged(damage);
        UE_LOG(LogTemp, Warning, TEXT("%d"), damage);
        if (damageFX != nullptr)
        {
            damageFX->SetActorLocation(boss->GetActorLocation());
            damageFX->PlayFX();
        }
        //칼맞는 소리

        if (swordHitSound != nullptr)
        {
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), swordHitSound, boss->GetActorLocation(), boss->GetActorRotation());
        }
    }
}

void APlayerCharacter::PlayerMove(const FInputActionValue& Value)
{
    	FVector2D inputValue = Value.Get<FVector2D>();
        moveDirection = FVector(inputValue.Y, inputValue.X, 0);
        if (IsValid(Controller))
        {
            const FRotator Rotation = Controller->GetControlRotation();
            const FRotator YawRotation(0, Rotation.Yaw, 0);

            const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
            const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

            AddMovementInput(ForwardDirection, inputValue.Y);
            AddMovementInput(RightDirection, inputValue.X);

            if (playerAnim != nullptr)
            {
                playerAnim->moveDirection = moveDirection;
            }
        }
        playerAnim->bIsAttack = false;
        
}

void APlayerCharacter::SelfDamaging(const FInputActionValue& Value)
{
    PlayerDamaged(100);
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
 
    if (GetWorldTimerManager().IsTimerActive(AttackTimer))
    {
        return;
    }
    if(DrawSword)
    {
        FString sectionName = FString("Left") + FString::FromInt(left);
        PlayAnimMontage(Left_montages, 1.5f, FName(sectionName));
        //칼 휘두르는 소리 
        if (swordSound != nullptr)
        {
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), swordSound, GetActorLocation(), GetActorRotation());
        }

        UE_LOG(LogTemp, Warning, TEXT("%d"), left);

        damage = 10;
        left = left + 1;
        if (left > 3)
        {
            left = 1;
        }
        PlayerLocation = GetActorLocation();
        Loca=true;
    }
    else
    {
        PlayAnimMontage(Draw_montage, 1.5f);
        //칼 휘두르는 소리 
        if (swordSound != nullptr)
        {
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), swordSound, GetActorLocation(), GetActorRotation());
        }
        DrawSword = true;
    }
    
    if(playerAnim!=nullptr)
    {
        playerAnim->bIsAttack=true;

        GetWorldTimerManager().SetTimer(AttackTimer, this, &APlayerCharacter::EndAttack, 0.5f, false);
    }
    GetWorldTimerManager().SetTimer(SteasheTimer, this, &APlayerCharacter::Steashe, 5.0f, false);
    
 }

void APlayerCharacter::RightAttack(const FInputActionValue& Value)
{
    if (GetWorldTimerManager().IsTimerActive(AttackTimer))
    {
        return;
    }

    if (DrawSword)
    {
        FString sectionName = FString("Right") + FString::FromInt(right);
        PlayAnimMontage(Right_montages, 1.5f, FName(sectionName));
        //칼 휘두르는 소리 
        if (swordSound != nullptr)
        {
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), swordSound, GetActorLocation(), GetActorRotation());
        }
        damage = 20;
        UE_LOG(LogTemp, Warning, TEXT("%d"), right);
        right = right + 1;

        if (right >= 3)
        {
            right = 1;
        }
        PlayerLocation = GetActorLocation();
        Loca = true;
    }
    else
    {
        PlayAnimMontage(Draw_montage, 1.5f);
        //칼 휘두르는 소리
        if (swordSound != nullptr)
        {
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), swordSound, GetActorLocation(), GetActorRotation());
        }
        DrawSword = true;
    }
    if (playerAnim != nullptr)
    {
        playerAnim->bIsAttack = true;

        GetWorldTimerManager().SetTimer(AttackTimer, this, &APlayerCharacter::EndAttack, 0.5f, false);
    }
    GetWorldTimerManager().SetTimer(SteasheTimer, this, &APlayerCharacter::Steashe, 5.0f, false);
}

void APlayerCharacter::Dodge(const FInputActionValue& Value)
{
    
    PlayAnimMontage(Dodge_montage, 1.5f);
    // DodgeAnim 이 실행 되는 동안 캐릭터에게 들어오는 데미지가 0이 되어야 한다.
  
   
    isDodge=true;
   
    UE_LOG(LogTemp, Warning , TEXT("Dodge Mode"));
		
 }

void APlayerCharacter::DodgeFinished()
{
     isDodge = false;

     UE_LOG(LogTemp, Warning, TEXT("Dodge Mode off"));
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
          //칼맞는 소리
          if (swordHitSound != nullptr)
          {
              UGameplayStatics::PlaySoundAtLocation(GetWorld(), swordHitSound, bossCharacter->GetActorLocation(), bossCharacter->GetActorRotation());
          }
      }
    }
   
}

//플레이어 데미지 입는 함수 
void APlayerCharacter::PlayerDamaged(int32 dmg)
{   
    if(isDodge==true)
    {
        UE_LOG(LogTemp, Warning, TEXT("Dodge"));
        return;
    }
    else
    {
        //체력깎기 (체력 0~maxHP 범위로 설정)
        currentHP = FMath::Clamp(currentHP - dmg, 0, maxHP);
        if(playerUI != nullptr)
        {   //깎인 체력으로 체력바 UI 갱신 
        playerUI->SetHealthBar((float)currentHP / (float)maxHP);
        }
        // 맞았을 때 체력이 0이거나 0보다 작아지면
        if (currentHP <= 0)
        {   
            //죽는함수 호출 
            PlayerDie();
        }
        PlayAnimMontage(OnDamaged_montage, 1.5f);
        UE_LOG(LogTemp, Warning , TEXT("UnDodge"));
    }
}
//플레이어 데미지 입음 + 뒤로 튕겨지는 함수 
void APlayerCharacter::PlayerDamagedWithKnockBack(int32 dmg, AActor* attacker)
{   
    //체력깎기 (체력 0~maxHP 범위로 설정)
    currentHP = FMath::Clamp(currentHP - dmg, 0, maxHP);

    FVector backVac = GetActorForwardVector() * -1.0f;
    FVector targetDir = (GetActorLocation() - attacker->GetActorLocation()) * 100000;
    FVector knockBackLocation = FMath::InterpEaseIn(GetActorLocation(), targetDir, GetWorld()->GetDeltaSeconds() * 2, 2.0f);
    UE_LOG(LogTemp, Warning, TEXT("before:%f,%f,%f"), GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z);
    SetActorLocation(knockBackLocation, true);
    UE_LOG(LogTemp, Warning, TEXT("after:%f,%f,%f"),knockBackLocation.X,knockBackLocation.Y,knockBackLocation.Z);
    UE_LOG(LogTemp, Warning, TEXT("before:%f,%f,%f"), GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z);
    PlayAnimMontage(OnDamaged_montage, 1.5f);

    if (playerUI != nullptr)
    {  //깎인 체력으로 체력바 UI 갱신 
        playerUI->SetHealthBar((float)currentHP / (float)maxHP);
    }

    //맞았을 때 체력이 0이거나 0보다 작아지면 
    if (currentHP <= 0)
    {   
        //죽는함수 호출
        PlayerDie();
    }
}

void APlayerCharacter::EndAttack()
{
	if (playerAnim != nullptr)
	{
		playerAnim->bIsAttack = false;
	}
    Loca = false;
}

void APlayerCharacter::Steashe()
{
    PlayAnimMontage(steashe_montage);
    DrawSword=false;
}

//보스 페이즈전환시 카메라 전환(시네마틱시퀀스) 
void APlayerCharacter::SwitchCameraToBoss()
{   
   //보스 찍고있는 카메라로 이동 
   UCameraComponent* bc = bossCharacter->bossCamera;
   pc -> SetViewTargetWithBlend(bossCharacter, 0.5f);
   //0.5초 뒤 다시 본래 플레이어 카메라로 이동 
   FTimerHandle cameraHandle;
   GetWorldTimerManager().SetTimer(cameraHandle, FTimerDelegate::CreateLambda([&]() {
       pc->SetViewTargetWithBlend(this, 1.0f);

       }), 0.5f, false);
}


//플레이어 죽는 함수 
void APlayerCharacter::PlayerDie()
{   
    bIsDead = true;
    if(pc!=nullptr)
    {
        pc->PlayerCameraManager->StartCameraFade(0,1,3.0f,FLinearColor::Black);

        FTimerHandle restartHandle;
    }
    
    PlayAnimMontage(Die_montage);
    GetCharacterMovement()->DisableMovement(); 
  
  //게임오버 UI 띄우기
    FTimerHandle uiHandle; 
    GetWorldTimerManager().SetTimer(uiHandle, FTimerDelegate::CreateLambda([&]() {

    if (nierGM != nullptr)
    {
        nierGM->ShowGameOverUI();
    }
        }), 3.0f, false);
    
}