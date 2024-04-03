#include "Inkpot/InkpotStory.h"
#include "Inkpot/InkpotChoice.h"
#include "Inkpot/Inkpot.h"
#include "Ink/Choice.h"
#include "Ink/StoryState.h"
#include "Ink/SearchResult.h"
#include "Ink/Path.h"
#include "Utility/InkpotLog.h"


void UInkpotStory::Initialise( TSharedPtr<FInkpotStoryInternal>  InInkpotStory )
{
	StoryInternal = InInkpotStory;
	StoryInternal->OnDidContinue().AddUObject(this, &UInkpotStory::OnContinueInternal );
	StoryInternal->OnMakeChoice().AddUObject(this, &UInkpotStory::OnMakeChoiceInternal );
	StoryInternal->OnEvaluateFunction().AddUObject(this, &UInkpotStory::OnEvaluateFunctionInternal );
	StoryInternal->OnCompleteEvaluateFunction().AddUObject(this, &UInkpotStory::OnCompleteEvaluateFunctionInternal );
	StoryInternal->OnChoosePathString().AddUObject(this, &UInkpotStory::OnChoosePathStringInternal );
}

TSharedPtr<FInkpotStoryInternal> UInkpotStory::GetStoryInternal()
{
	return StoryInternal;
}

int32 UInkpotStory::GetID() const
{
	if(!StoryInternal.IsValid())
		return FInkpotStoryInternal::BadStoryHandle;
	return StoryInternal->GetID();
}

bool UInkpotStory::IsValid() const
{
	if(!StoryInternal.IsValid())
		return false;
	return StoryInternal->IsValidStory();
}

FString UInkpotStory::GetCurrentText()
{
	FString currentText = StoryInternal->GetCurrentText();
	currentText.TrimEndInline();
	return currentText;
}

const TArray<FString>& UInkpotStory::GetCurrentTags()
{
	return StoryInternal->GetCurrentTags();
}

TArray<FString> UInkpotStory::GlobalTags()
{
	return StoryInternal->GlobalTags();
}

TArray<FString> UInkpotStory::TagsForContentAtPath( const FString &InPath)
{
	return StoryInternal->TagsForContentAtPath( InPath );
}

void UInkpotStory::ChoosePath( const FString &InPath )
{
	StoryInternal->ChoosePathString( InPath, true, TArray<TSharedPtr<Ink::FValueType>>() );
}

void UInkpotStory::ChoosePathString( const FString &InPath, const TArray<FInkpotValue> &InValues )
{
	TArray<TSharedPtr<Ink::FValueType>> values;
	values.Reserve( InValues.Num() );
	for( const FInkpotValue &inValue : InValues )
		values.Emplace( *inValue );
	StoryInternal->ChoosePathString( InPath, true, values );
}

FString UInkpotStory::Continue()
{
	return StoryInternal->Continue();
}

FString UInkpotStory::ContinueMaximally()
{
	return StoryInternal->ContinueMaximally();
}

FString UInkpotStory::ContinueMaximallyAtPath(const FString& InPath)
{
	FString ret; 
	ChoosePath(InPath);
	if (CanContinue())
		ret = ContinueMaximally();
	return ret;
}

bool UInkpotStory::CanContinue()
{
	return StoryInternal->CanContinue();
}

bool UInkpotStory::HasChoices()
{
	return StoryInternal->HasChoices();
}

const TArray<UInkpotChoice*>& UInkpotStory::GetCurrentChoices()
{
	return Choices;
}

void UInkpotStory::UpdateChoices()
{
	Choices.Empty();

	if(!StoryInternal->HasChoices())
		return;

	TArray<TSharedPtr<Ink::FChoice>> inkChoices =  StoryInternal->GetCurrentChoices();
	for( auto inkChoice : inkChoices  )
	{
		UInkpotChoice* choice = NewObject<UInkpotChoice>( this );
		choice->Initialise( inkChoice );
		Choices.Emplace( choice );
	}
}

void UInkpotStory::ChooseChoice( UInkpotChoice* InChoice )
{
	if(!InChoice)
		return;
	ChooseChoiceIndex( InChoice->GetIndex() );
}

void UInkpotStory::ChooseChoiceIndex( int32 InChoiceIndex )
{
	StoryInternal->ChooseChoiceIndex( InChoiceIndex );
}

bool UInkpotStory::SwitchFlow( const FString &InFlowName )
{
	bool isFlowAlive = IsFlowAlive( InFlowName );
	StoryInternal->SwitchFlow( InFlowName );
	UpdateChoices();
	BroadcastFlowChange();
	return !isFlowAlive; 
}

void UInkpotStory::RemoveFlow(const FString& InFlowName)
{
	FString currentFlow = GetCurrentFlowName();
	StoryInternal->RemoveFlow( InFlowName );
	if( !InFlowName.Equals( currentFlow ) )
		return;
	UpdateChoices();
	BroadcastFlowChange();
}

FString UInkpotStory::GetCurrentFlowName()
{
	return StoryInternal->GetCurrentFlowName();
}

void UInkpotStory::SwitchToDefaultFlow()
{
	StoryInternal->SwitchToDefaultFlow();
	UpdateChoices();
	BroadcastFlowChange();
}

void UInkpotStory::BroadcastFlowChange()
{
	if(!EventOnSwitchFlow.IsBound())
		return;
	FString currentFlow = GetCurrentFlowName();
	EventOnSwitchFlow.Broadcast( this, currentFlow );
}

bool UInkpotStory::IsFlowAlive( const FString &InFlowName )
{
	TSharedPtr<Ink::FStoryState> state = StoryInternal->GetStoryState();
	TSharedPtr<TMap<FString, TSharedPtr<Ink::FFlow>>> namedFlows = state->GetNamedFlows();
	if( namedFlows.IsValid() )
		return namedFlows->Contains( InFlowName );
	return false;
}

const TArray<FString> &UInkpotStory::GetAliveFlowNames()
{
	TSharedPtr<Ink::FStoryState> state = StoryInternal->GetStoryState();
	return state->GetAliveFlowNames();
}

int32 UInkpotStory::GetAliveFlowCount()
{
	TSharedPtr<Ink::FStoryState> state = StoryInternal->GetStoryState();
	TSharedPtr<TMap<FString, TSharedPtr<Ink::FFlow>>> namedFlows = state->GetNamedFlows();

	int32 count = 0;
	if( namedFlows.IsValid() )
		count = namedFlows->Num();
	return count; 
}

void UInkpotStory::SetValue(const FString &InVariable, FInkpotValue InValue)
{
	TSharedPtr<Ink::FVariableState> variableState = StoryInternal->GetVariablesState();
	variableState->SetVariable( InVariable, Ink::FValue::Create(**InValue) );
}

FInkpotValue UInkpotStory::GetValue(const FString &InVariable)
{
	TSharedPtr<Ink::FVariableState> variableState = StoryInternal->GetVariablesState();
	TSharedPtr<Ink::FObject> variableObj = variableState->GetVariable( InVariable );
	TSharedPtr<Ink::FValue> variable = StaticCastSharedPtr<Ink::FValue>( variableObj );
	Ink::FValueType value;
	if(variable.IsValid())
		value = variable->GetValueObject();
	else
		INKPOT_ERROR("Could not get value of variable '%s'", *InVariable );
	return FInkpotValue(value);
}

void UInkpotStory::SetBool(const FString &InVariable, bool bInValue)
{
	TSharedPtr<Ink::FVariableState> variableState = StoryInternal->GetVariablesState();
	variableState->SetVariable( InVariable, MakeShared<Ink::FValueBool>( bInValue ) );
}

bool UInkpotStory::GetBool(const FString &InVariable)
{
	bool rval;
	bool success = GetVariable<bool, Ink::FValueBool>(InVariable, Ink::EValueType::Bool, rval );
	return rval;
}

void UInkpotStory::SetInt(const FString &InVariable, int32 InValue)
{
	TSharedPtr<Ink::FVariableState> variableState = StoryInternal->GetVariablesState();
	variableState->SetVariable( InVariable, MakeShared<Ink::FValueInt>( InValue ) );
}

int32 UInkpotStory::GetInt(const FString &InVariable)
{
	int32 rval;
	bool success = GetVariable<int32, Ink::FValueInt>(InVariable, Ink::EValueType::Int, rval );
	return rval;
}

void UInkpotStory::SetFloat(const FString &InVariable, float InValue )
{
	TSharedPtr<Ink::FVariableState> variableState = StoryInternal->GetVariablesState();
	variableState->SetVariable( InVariable, MakeShared<Ink::FValueFloat>(InValue));
}

float UInkpotStory::GetFloat(const FString &InVariable)
{
	float rval;
	bool success = GetVariable<float, Ink::FValueFloat>(InVariable, Ink::EValueType::Float, rval );
	return rval;
}

void UInkpotStory::SetString(const FString &InVariable, const FString &InValue )
{
	TSharedPtr<Ink::FVariableState> variableState = StoryInternal->GetVariablesState();
	variableState->SetVariable( InVariable, MakeShared<Ink::FValueString>(InValue) );
}

FString UInkpotStory::GetString( const FString& InVariable )
{
	FString rval;
	bool success = GetVariable<FString, Ink::FValueString>(InVariable, Ink::EValueType::String, rval );
	return rval;
}

void UInkpotStory::SetEmpty(const FString &InVariable)
{
	TSharedPtr<Ink::FVariableState> variableState = StoryInternal->GetVariablesState();
	variableState->SetVariable( InVariable, MakeShared<Ink::FValue>() );
}

int32 UInkpotStory::GetVariableKeys( TArray<FString> &OutKeys )
{
	TSharedPtr<Ink::FVariableState> variableState = StoryInternal->GetVariablesState();
	const TMap<FString, TSharedPtr<Ink::FObject>>& variables = variableState->GetVariables();
	int32 rval = variables.GetKeys( OutKeys );
	return rval;
}

void UInkpotStory::ObserveVariable( const FString& InVariable, TSharedPtr<FStoryVariableObserver> InObserver )
{
	StoryInternal->ObserveVariable( InVariable, InObserver );
}

void UInkpotStory::SetOnVariableChange( FOnInkpotVariableChange InDelegate, const FString &InVariable )
{
	TSharedPtr<FStoryVariableObserver> observer = MakeShared<FStoryVariableObserver>();
	observer->BindWeakLambda
	(
		InDelegate.GetUObject(), [this, InDelegate] (const FString& InVar, Ink::FValueType InType)
		{
			InDelegate.Execute( this, InVar, FInkpotValue(InType) );
		}
	);
	
	VariableObservers.AddUnique( InVariable, observer );
	StoryInternal->ObserveVariable( InVariable, observer );
}

bool UInkpotStory::ClearVariableChange( const UObject* Owner, const FString &Variable )
{
	if (!Owner || !VariableObservers.Contains(Variable))
		return false;

	bool bRemovedAtLeastOneEntry = false;
	
	for (auto Iterator = VariableObservers.CreateConstKeyIterator(Variable); Iterator; ++Iterator)
	{
		if (Iterator->Value.IsValid() && Owner == Iterator->Value->GetUObject())
		{
			StoryInternal->RemoveVariableObserver( Iterator->Value, Variable );
			VariableObservers.RemoveSingle( Variable, Iterator->Value );
			bRemovedAtLeastOneEntry = true;
		}
	}

	return bRemovedAtLeastOneEntry;
}

void UInkpotStory::ClearAllVariableObservers( const FString& Variable )
{
	if (!VariableObservers.Contains(Variable))
		return;
	
	for (auto Iterator = VariableObservers.CreateConstKeyIterator(Variable); Iterator; ++Iterator)
	{
		StoryInternal->RemoveVariableObserver( Iterator->Value, Variable );
	}

	VariableObservers.Remove(Variable);
}

void UInkpotStory::OnContinueInternal()
{
	UpdateChoices();
	DumpDebug();
	if(!EventOnContinue.IsBound())
		return;
	EventOnContinue.Broadcast( this );
}

void UInkpotStory::OnMakeChoiceInternal(TSharedPtr<Ink::FChoice> InChoice)
{
	if(!EventOnContinue.IsBound())
		return;
	int32 key = InChoice->GetIndex();
	UInkpotChoice ** choicePtr = Choices.FindByPredicate( [key]( UInkpotChoice *pChoice ) { return *pChoice == key; } );
	if(!choicePtr)
		return;
	DumpDebug( *choicePtr );
	EventOnMakeChoice.Broadcast( this, *choicePtr );
}

void UInkpotStory::OnEvaluateFunctionInternal(const FString& InFunctionName, const TArray<TSharedPtr<Ink::FValueType>>& InFunctionParms)
{

}

void UInkpotStory::OnCompleteEvaluateFunctionInternal(const FString& InFunctionName, const TArray<TSharedPtr<Ink::FValueType>>& InFunctionParms, const FString& OutParmName, TSharedPtr<Ink::FValueType> OutParmType)
{

}

void UInkpotStory::OnChoosePathStringInternal(const FString& InPath, const TArray<TSharedPtr<Ink::FValueType>>& InPathType )
{
	UpdateChoices();
	if(!EventOnChoosePath.IsBound())
		return;
	EventOnChoosePath.Broadcast( this, InPath ); 
}

void UInkpotStory::DumpDebug()
{
	FString currentFlow = GetCurrentFlowName();
	INKPOT_LOG("Flow         : %s", *currentFlow);

	if( GetAliveFlowCount() > 0 )
	{
		TArray<FString> flows = GetAliveFlowNames();
		if ( flows.Num() > 0 )
		{
			FString flowsAlive;
			for ( auto flow : flows )
			{
				flowsAlive.Append( flow );
				flowsAlive.Append( ", " );
			}
			INKPOT_LOG("Flows alive  : %s", *flowsAlive );
		}
	}

	FString currentText = GetCurrentText();
	INKPOT_LOG("Text         : %s", *currentText);

	const TArray<FString>& tags = GetCurrentTags();
	if( tags.Num() > 0 )
	{
		FString tagsSet;
		for( auto tag : tags )
		{
			tagsSet.Append( "'#" );
			tagsSet.Append( tag );
			tagsSet.Append( "' " );
		}
		INKPOT_LOG("CTags        : %s", *tagsSet);
	}

	TArray<FString> gtags = GlobalTags();
	if( gtags.Num() > 0 )
	{
		FString tagsSet;
		for( auto tag : gtags )
		{
			tagsSet.Append( "'#" );
			tagsSet.Append( tag );
			tagsSet.Append( "' " );
		}
		INKPOT_LOG("GTags        : %s", *tagsSet);
	}

	TArray<FString> keys;
	if(int num = GetVariableKeys( keys ))
	{
		for( int32 i=0; i<num; ++i )
		{
			const FString& key = keys[i];
			TSharedPtr<Ink::FObject> obj = GetVariable( key );
			const FString& value = obj->ToString();
			if(i==0)
			{
				INKPOT_LOG("Variables    : %s = %s", *key, *value);
			}
			else
			{
				INKPOT_LOG("             : %s = %s", *key, *value);
			}
		}
	}

	if(Choices.Num() > 0)
	{
		for( int32 i=0; i<Choices.Num(); ++i )
		{
			FString tagsSet;
			for( auto tag : Choices[i]->GetTags() )
			{
				tagsSet.Append( "'#" );
				tagsSet.Append( tag );
				tagsSet.Append( "' " );
			}
			if( i == 0 ) 
			{
				INKPOT_LOG( "Choice       : %d - %s  %s", Choices[ i ]->GetIndex(), *Choices[ i ]->GetString(), *tagsSet );
			}
			else
			{
				INKPOT_LOG( "             : %d - %s  %s", Choices[ i ]->GetIndex(), *Choices[ i ]->GetString(), *tagsSet );
			}
		}
	}
}

void UInkpotStory::DumpDebug(UInkpotChoice *InChoice)
{
	if(!InChoice)
		return;
	INKPOT_LOG("Chose        : %d - %s", InChoice->GetIndex(), *InChoice->GetString());
}


void UInkpotStory::DumpContainer(const FString &InName, TSharedPtr<Ink::FContainer> InContainer, int Indent)
{
	if(!InContainer)
		return;

	FString pad; 
	for(int i=0;i<Indent;++i)
		pad += '\t';

	INKPOT_LOG("%s** container(%s) - Begin", *pad,*InName );
	pad += '\t';

	TArray<TSharedPtr<Ink::FObject>> *contents = InContainer->GetContent().Get();
	for( TSharedPtr<Ink::FObject> obj : *contents)
	{
		TSharedPtr<Ink::FContainer> container = Ink::FObject::DynamicCastTo<Ink::FContainer>(obj);
		if( container )
		{
			DumpContainer( "inline", container, Indent + 1 );
		}
		else
		{
			FString entry = obj->ToString();
			entry.TrimEndInline();
			if ( entry.Len() )
				INKPOT_LOG( "%s%s", *pad, *entry );
		}
	}

	TSharedPtr<TMap<FString, TSharedPtr<Ink::FObject>>> namedContentPtr = InContainer->GetNamedContent();
	for( auto pair : *namedContentPtr )
	{
		TSharedPtr<Ink::FContainer> container = Ink::FObject::DynamicCastTo<Ink::FContainer>( pair.Value );
		if(!container)
			continue;
		DumpContainer( pair.Key, container, Indent + 1 );
	}

	pad.LeftChopInline(1);
	INKPOT_LOG("%s** container(%s) - End", *pad, *InName );
}


void UInkpotStory::DumpMainContent()
{
	TSharedPtr<Ink::FContainer> main = StoryInternal->GetMainContentContainer();
	DumpContainer( "<root>", main );
}

void UInkpotStory::DumpContentAtPath( const FString& InName )
{
	TSharedPtr<Ink::FPath> path = MakeShared<Ink::FPath>( InName );
	Ink::FSearchResult result = StoryInternal->ContentAtPath(path);
	DumpContainer(InName, result.GetObjectAsContainer() );
}

void UInkpotStory::DumpContentAtKnot( const FString& InName )
{
	TSharedPtr<Ink::FContainer> knotContainer = StoryInternal->KnotContainerWithName(InName);
	if(!knotContainer)
	{
		INKPOT_ERROR("%s is not a valid knot in this story", *InName );
		return;
	}
	DumpContainer(InName, knotContainer  );
}

void UInkpotStory::GatherAllStrings( TMap<FString, FString> &OutStrings )
{
	TSharedPtr<Ink::FContainer> main = StoryInternal->GetMainContentContainer();
	GatherAllStrings( "", main, OutStrings);
}

void UInkpotStory::GatherAllStrings( const FString &InRootName, TSharedPtr<Ink::FContainer> InContainer, TMap<FString, FString> &OutStrings )
{
	if(!InContainer)
		return;

	FString rootName = InRootName;
	FString containerName = InContainer->GetName();
	if (rootName.IsEmpty())
		rootName = containerName;
	else if (containerName.Len() > 0)
		rootName += TEXT(".") + containerName;

	TArray<TSharedPtr<Ink::FObject>> *contents = InContainer->GetContent().Get();
	int contentIndex = 1;
	for( TSharedPtr<Ink::FObject> obj : *contents)
	{
		TSharedPtr<Ink::FContainer> container = Ink::FObject::DynamicCastTo<Ink::FContainer>(obj);
		if( container )
		{
			GatherAllStrings( rootName, container, OutStrings  );
		}
		else
		{
			if(obj->CanCastTo(Ink::EInkObjectClass::FValueString))
			{
				FString entry = obj->ToString();
				entry.TrimEndInline();
				if ( entry.Len() )
				{
					FString key = FString::Printf(TEXT("%s.%02d"), *rootName, contentIndex);
					OutStrings.Add( key, entry );
					INKPOT_LOG( "%s : %s", *key, *entry );
					contentIndex++;
				}
			}
		}
	}

	TSharedPtr<TMap<FString, TSharedPtr<Ink::FObject>>> namedContentPtr = InContainer->GetNamedContent();
	for( auto pair : *namedContentPtr )
	{
		TSharedPtr<Ink::FContainer> container = Ink::FObject::DynamicCastTo<Ink::FContainer>( pair.Value );
		if(!container)
			continue;
		GatherAllStrings( rootName, container, OutStrings );
	}
}

TArray<FString> UInkpotStory::GetNamedContent()
{
	TSharedPtr<Ink::FContainer> container = StoryInternal->GetMainContentContainer();
	return GetNamedContent(container);
}

TArray<FString> UInkpotStory::GetNamedContentForKnot(const FString& InKnotName)
{
	TSharedPtr<Ink::FContainer> knotContainer = StoryInternal->KnotContainerWithName(InKnotName);
	return GetNamedContent(knotContainer);
}

TArray<FString> UInkpotStory::GetNamedContentForPath(const FString& InPath)
{
	TSharedPtr<Ink::FPath> path = MakeShared<Ink::FPath>(InPath);
	Ink::FSearchResult result = StoryInternal->ContentAtPath(path);
	return GetNamedContent(result.GetObjectAsContainer());
}

TArray<FString> UInkpotStory::GetNamedContent(TSharedPtr<Ink::FContainer> InContainer)
{
	TArray<FString> keys;
	if (!InContainer)
	{
		INKPOT_ERROR("Container is invalid, no content.");
		return keys;
	}

	TSharedPtr<TMap<FString, TSharedPtr<Ink::FObject>>> namedContentPtr = InContainer->GetNamedContent();
	for (auto pair : *namedContentPtr)
	{
		TSharedPtr<Ink::FContainer> container = Ink::FObject::DynamicCastTo<Ink::FContainer>(pair.Value);
		if (!container)
			continue;
		keys.Add(pair.Key);
	}

	return keys;
}

void UInkpotStory::BindExternalFunction( const FString &InFunctionName, FInkpotExternalFunction InFunction, bool bInLookAheadSafe  )
{
	TSharedPtr<FStoryExternalFunction> function = MakeShared<FStoryExternalFunction>();
	function->BindWeakLambda
	(
		InFunction.GetUObject(), [this, InFunction] (const TArray<Ink::FValueType> &InArguments) -> TSharedPtr<Ink::FValueType>
		{
			TArray<FInkpotValue> arguments;
			for(auto &argument : InArguments )
				arguments.Add( argument );

			FInkpotValue value = InFunction.Execute( arguments );
			return *value;
		}
	);
	StoryInternal->BindExternalFunctionGeneral( InFunctionName, function, bInLookAheadSafe );
}

void UInkpotStory::UnbindExternalFunction( const FString &InFunctionName )
{
	StoryInternal->UnbindExternalFunction( InFunctionName );
}

FOnStoryContinue& UInkpotStory::OnContinue()
{
	return EventOnContinue;
}

FOnMakeChoice& UInkpotStory::OnMakeChoice()
{
	return EventOnMakeChoice;
}

FOnChoosePath& UInkpotStory::OnChoosePath()
{
	return EventOnChoosePath;
}

FOnSwitchFlow& UInkpotStory::OnSwitchFlow()
{
	return EventOnSwitchFlow;
}

FOnStoryLoadJSON& UInkpotStory::OnStoryLoadJSON()
{
	return EventOnStoryLoadJSON;
}

UInkpotLine *UInkpotStory::GetCurrentLine()
{
	UInkpotLine* line = NewObject<UInkpotLine>( this );
	line->Initialise( GetCurrentText() );
	return line;
}

void UInkpotStory::ResetContent( TSharedPtr<FInkpotStoryInternal> InNewStoryContent )
{
	StoryInternal->ResetContent( InNewStoryContent );
}

void UInkpotStory::ResetState()
{
	StoryInternal->ResetState();
}

FString UInkpotStory::ToJSON()
{
	return StoryInternal->GetStoryState()->ToJson();
}

void UInkpotStory::LoadJSON(const FString &InJSON)
{
	StoryInternal->GetStoryState()->LoadJSON( InJSON );
	UpdateChoices();
	if(EventOnStoryLoadJSON.IsBound())
		EventOnStoryLoadJSON.Broadcast( this );
}

int UInkpotStory::GetStorySeed() const
{
	return StoryInternal->GetStoryState()->GetStorySeed();
}

void UInkpotStory::SetStorySeed( int Seed )
{
	StoryInternal->GetStoryState()->SetStorySeed( Seed );
}
