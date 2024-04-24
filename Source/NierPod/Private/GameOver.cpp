

#include "GameOver.h"
#include "NierGameModeBase.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Button.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>

void UGameOver::NativeConstruct()
{
	Super::NativeConstruct();

	// 재시작 버튼의 클릭 이벤트에 RestartGame 함수를 연결한다.
	btn_restart->OnClicked.AddDynamic(this, &UGameOver::RestartGame);

	// 종료 버튼의 클릭 이벤트에 QuitGame 함수를 연결한다.
	btn_quit->OnClicked.AddDynamic(this, &UGameOver::QuitGame);

}

void UGameOver::RestartGame()
{
	
	GetWorld()->GetWorldSettings()->SetTimeDilation(1);
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	RemoveFromParent();
	// Level을 다시 연다.
	UGameplayStatics::OpenLevel(GetWorld(), FName("ThirdPersonMap"));

}

void UGameOver::QuitGame()
{
	GetWorld()->GetFirstPlayerController()->ConsoleCommand("Quit");
}
