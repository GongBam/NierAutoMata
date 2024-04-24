

#include "GameOver.h"
#include "NierGameModeBase.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Button.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>

void UGameOver::NativeConstruct()
{
	Super::NativeConstruct();

	// ����� ��ư�� Ŭ�� �̺�Ʈ�� RestartGame �Լ��� �����Ѵ�.
	btn_restart->OnClicked.AddDynamic(this, &UGameOver::RestartGame);

	// ���� ��ư�� Ŭ�� �̺�Ʈ�� QuitGame �Լ��� �����Ѵ�.
	btn_quit->OnClicked.AddDynamic(this, &UGameOver::QuitGame);

}

void UGameOver::RestartGame()
{
	
	GetWorld()->GetWorldSettings()->SetTimeDilation(1);
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	RemoveFromParent();
	// Level�� �ٽ� ����.
	UGameplayStatics::OpenLevel(GetWorld(), FName("ThirdPersonMap"));

}

void UGameOver::QuitGame()
{
	GetWorld()->GetFirstPlayerController()->ConsoleCommand("Quit");
}
