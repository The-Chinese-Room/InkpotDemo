#include "UI/InkpotDemoWidget.h"


void UInkpotDemoWidget::ShowStory( UInkpotStory *InStory )
{
	InkStory = InStory;
	ReceiveOnShowStory( InkStory );
}


