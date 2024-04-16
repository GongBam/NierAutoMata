

#include "PlayerHealthWidget.h"
#include "PlayerCharacter.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/ProgressBar.h>

void UPlayerHealthWidget::SetHealthBar(float value)
{
	playerHealthBar->SetPercent(value);
}
