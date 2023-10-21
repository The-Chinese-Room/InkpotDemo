# Inkpot Demo
Small demo project in Unreal showing how to use the Inkpot plugin for Unreal Engine created by [The Chinese Room](https://www.thechineseroom.co.uk/).<br><br>
Inkpot is a wrapper for the scripting language **Ink** developed by [Inkle Studios](https://www.inklestudios.com/ink/)<br>
See the [Inkpot github](https://github.com/The-Chinese-Room/Inkpot) for more info on the Inkpot plugin itself.<br><br>
This project works with Unreal Engine version 5.3 or later.<br>

## Folder structure 

	Content
		InkpotDemo
			Framework 
				BP_InkpotDemoGameMode 
					- game mode Blueprint, creates UI, handles Inkpot OnStoryBegin.
			Maps
				Demo 		
					- A demo of how Ink scripts can interact with level blueprints and actors. 
				HelloInk	
					- The most minimal demo using the default ink story.
				Intro 		
					- An intro screen for those that don't read docs.
			Props
				BP_InkyCube	
					- actor blueprint showing how to get Ink variable change notification. 
			Stories			
				- all the Inkpot story assets.
			UI
				WBP_Display
					- the main display widget, this is where most of the Ink logic lies.
				WBP_Choice
					- widget for choices that can be made in a story, used by WBP_Display.


## Editing Ink source
Use Inky, which you can get from here.<br>
https://github.com/inkle/inky

## Importing ink files 
To import, drag the ink source file into a content browser window within the Unreal Editor.<br>
The will create the InkpotStoryAsset, that contains the compiled JSON from source.<br>

![DragFile](Documentation/DragFile.png)

## Setting up auto reimport
Open editor, preferences.<br> 
Look Under general, loading and saving, auto reimport.<br>
Add a new 'Directories to Monitor' entry by clicking the plus.<br>

Include the source folder, map directory to the corresponding folder in the unreal content browser.

![AutoReimportOptions](Documentation/AutoReimport.png)

Once this is setup, the content directory will automatically update when you update, create or delete any ink files in the source folder.

## Starting a story 
In blueprints, get the Inkpot subsystem and call BeginStory, passing in the InpotStoryAsset. 

![BeginStory](Documentation/BeginStory.png)

This returns the runtime InkpotStory that executes the script.  
In this demo, the stories are started by the Level Blueprint.

## Getting story updates
Once the story has been started using the BeginStory function of Inkpot, it will return an InkpotStory.<br>
The InkpotStory is the runtime for the Ink script execution.<br>
In this demo, the UI is hooked up to the InkpotStory created by the level blueprint.<br>
This is done in the game mode blueprint, BP_InkpotDemoGameMode.<br>

![BindStoryBegin](Documentation/BindBegin.png)

The UI, where all of the story updates happen, is defined by the widget WBP_Display.<br>
The event graph looks like this. <br>

![UIEventGraph](Documentation/UIEventGraph.png)

Here we see the events that drive Ink execution.<br>

### OnContinue 
This is called once the story has continued. Here we update the UI to show the current story's text, and update the choices if there are any. <br>
This event happens in response to Continue being called on the story.<br>

### OnMakeChoice
A choice has been made. In this demo the user has clicked on a choice shown ( see OnItemClicked ), and the ChooseChoice function has been called.<br> 
Here, we continue the story, which will result in OnContinue being called thereby updating the UI. <br>

### OnSwitchFlow
Flows allow several 'threads' of story to run at the same time. This is used in the demo level to give contextual story snippets fopr wherever the player has walked to.<br>
This event is called in response to calling the function SwitchFlow on the InkpotStory object.<br>

## Variables
Ink supports type free variables, Blueprints require typed variables.<br> 
To allow conversion between the two there is a blueprint function library to convert from one to the other.<br>

	/* Create an Ink Value from a boolean */
	FInkpotValue MakeBoolInkpotValue(bool bValue);

	/* Cast Ink value to a boolean */
	bool InkpotValueAsBool(FInkpotValue Value);

	/* Create an Ink Value from an integer */
	FInkpotValue MakeIntInkpotValue(int32 Value);

	/* Cast Ink value to an integer*/
	int32 InkpotValueAsInt(FInkpotValue Value);

	/* Create an Ink Value from a float */
	FInkpotValue MakeFloatInkpotValue(float Value);

	/* Cast Ink value to a float*/
	float InkpotValueAsFloat(FInkpotValue Value);

	/* Create an Ink Value from a string */
	FInkpotValue MakeStringInkpotValue(const FString &Value);

	/* Cast Ink value to a string*/
	FString InkpotValueAsString(FInkpotValue Value);

	/* Create an Ink List from an array of Strings */
	FInkpotValue MakeInkpotList(const TArray<FString> &Value);

	/* Get an array of strings from an Ink List */
	TArray<FString> InkpotValueAsList(FInkpotValue Value);    

![MakeInkpotValue](Documentation/MakeInkpotValue.png)

Alternatively, values can be set directly through the InkpotStory object using one of the Set or Get functions.<br>

	void SetBool(const FString &Variable, bool bValue);
	bool GetBool(const FString &Variable);

	void SetInt(const FString &Variable, int32 Value);
	int32 GetInt(const FString &Variable);

	void SetFloat( const FString& Variable, float Value );
	float GetFloat( const FString& Variable );

	void SetString( const FString& Variable, const FString& Value );
	FString GetString( const FString& Variable );

	void SetEmpty( const FString& Variable );

![StorySetNGet](Documentation/StorySetNGet.png)


### Change notification
There are a couple of ways of getting change notification from Inkpot, *Story Change Delegate* and *Inkpot Watch Component*.

#### Story Change Delegate
Variable change notification can be achieved through binding to the set on variable change delegate of the story.
![OnVariableChange](Documentation/SetOnVaraibleChange.png)

#### InkPotWatch component
The InkPotWatch component allows Actors to receive the notification of variable change.
Open up the Actor blueprint and Click Add component, choose InkpotWatch.

![AddComponent](Documentation/AddComponent.png)

Rename it to represent its purpose

![RenameComponent](Documentation/RenameComponent.png)

Then hook up the event, 

![ChangeNotify](Documentation/ChangeNotify.png)

And when this blueprint is placed in the world, set the 'Variable Watch' field to the name of the ink variable to be watched.

![InstanceDetails](Documentation/InstanceDetails.png)

---

# Testing InkPlusPlus
We have 168 active tests in Inkpot that test the implementaion of the InkPlusPlus module.<br>
These can all be run through the *Session Frontend* within the Unreal editor.<br>
To run the tests, first open the *Session Frontend* from Tools, Session Frontend.<br>

![OpenSessionFrontEnd](Documentation/OpenSessionFrontEnd.png)

Select the automation panel, and click the check box next to **Inkpot**.<br>
Then hit the play button to start the tests.<br>

![RunTests](Documentation/RunTests.png)

You will note that a couple of the tests fail, which are concerned with JSON serialisation. This is a feature that we did not need in our usage case for Ink. 
There are also some tests that have not yet been implemented, see .\\Inkpot\\TestInkSource\\Incomplete.

