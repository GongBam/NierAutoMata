

#include "ClearedUIWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UClearedUIWidget::NativeConstruct()
{
	btn_restart->OnClicked.AddDynamic(this, &UClearedUIWidget::RestartGame);
	btn_quit->OnClicked.AddDynamic(this, &UClearedUIWidget::QuitGame);
}

void UClearedUIWidget::RestartGame()
{
	GetWorld()->GetWorldSettings()->SetTimeDilation(1);
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	RemoveFromParent();
	UGameplayStatics::OpenLevel(GetWorld(), FName("ThirdPersonMap"));
}

void UClearedUIWidget::QuitGame()
{
	GetWorld()->GetFirstPlayerController()->ConsoleCommand("Quit");
}
