

#include "BossHealthWidget.h"
#include "BossCharacter.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/ProgressBar.h>

void UBossHealthWidget::SetHealthBar(float value)
{
	BossHealthBar->SetPercent(value);
}
