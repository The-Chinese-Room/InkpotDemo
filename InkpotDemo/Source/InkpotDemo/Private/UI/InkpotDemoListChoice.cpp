#include "UI/InkpotDemoListChoice.h"
#include "Inkpot/InkpotChoice.h"

void UInkpotDemoListChoice::NativeOnListItemObjectSet(UObject* InListItemObject)
{
	UInkpotChoice *choice = Cast<UInkpotChoice>(InListItemObject);
	if(!choice)
		return;

	FText text = FText::FromString( choice->GetString() );
	ReceiveOnTextUpdate( text );	
}
