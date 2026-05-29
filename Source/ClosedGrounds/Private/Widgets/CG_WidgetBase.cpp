// Pietras


#include "Widgets/CG_WidgetBase.h"

#include "ClosedGrounds/ClosedGrounds.h"

void UCG_WidgetBase::SetWidgetController(UCG_WidgetController* InWidgetController)
{
	if (!InWidgetController)
	{
		UE_LOG(LogGame, Error, TEXT("[%hs] - WidgetController is null!"), __FUNCTION__);
		return;
	}
	
	WidgetController = InWidgetController;
	OnWidgetControllerSet();
}
