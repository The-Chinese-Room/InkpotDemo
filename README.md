# Inkpot Demo
Small demo project in Unreal showing how to use the Inkpot plugin for Unreal Engine created by [The Chinese Room](https://www.thechineseroom.co.uk/).<br><br>
Inkpot is a wrapper for the scripting language **Ink** developed by [Inkle Studios](https://www.inklestudios.com/ink/)<br>
See the [Inkpot github](https://github.com/The-Chinese-Room/Inkpot) for more info on the Inkpot plugin itself.<br><br>
This project works with Unreal Engine version 5.4 or later.<br>

---

## Folder structure 

	Content
		InkpotDemo
			Framework 
				BP_InkpotDemoGameMode 
					- game mode Blueprint, creates UI, 
					handles Inkpot OnStoryBegin.
			Maps
				Demo 		
					- A demo of Ink scripts interacting with 
					level blueprints and actors. 
				HelloInk	
					- The most minimal demo using the default ink story.
				Intro 		
					- An intro screen for those that don't read docs.
				Test 		
					- for testing out all list operations.
			Props
				BP_InkyCube	
					- actor blueprint showing how to get Ink variable
					change notification. 
			Stories			
				- all the Inkpot story assets.
			UI
				WBP_Display
					- the main display widget, 
					this is where most of the Ink control logic lies.
				WBP_Choice
					- widget for choices that can be made in a story, 
					used by WBP_Display.

---

## Editing Ink source
Use Inky, which you can get from here.<br>
https://github.com/inkle/inky

---

## Importing ink files 
To import, drag the ink source file into a content browser window within the Unreal Editor.<br>
The will create the InkpotStoryAsset, that contains the compiled JSON from source.<br>

![DragFile](Documentation/DragFile.png)

### Note: .net 5.0 requirement
If your Ink file fails to import, then you most likely do not have **.net 5.0** installed.<br>
Inkpot compiles your source when you import using the InkleCate compiler, & InkleCate requires the **.net 5.0** framework.<br>
To fix install the **.net 5.0** framework <br>
https://dotnet.microsoft.com/en-us/download/dotnet/thank-you/runtime-5.0.17-windows-x64-installer<br>

---

## Setting up auto reimport
Open editor, preferences.<br> 
Look Under general, loading and saving, auto reimport.<br>
Add a new 'Directories to Monitor' entry by clicking the plus.<br>

Include the source folder, map directory to the corresponding folder in the unreal content browser.

![AutoReimportOptions](Documentation/AutoReimport.png)

Once this is setup, the content directory will automatically update when you update, create or delete any ink files in the source folder.

---

## Starting a story 
In blueprints, get the Inkpot subsystem and call BeginStory, passing in the InpotStoryAsset. 

![BeginStory](Documentation/BeginStory.png)

This returns the runtime InkpotStory that executes the script.  
In this demo, the stories are started by the Level Blueprint.

---

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

---

## Variables
Ink supports type free variables, Blueprints require typed variables.<br><br>  
### First value types
For bool, int, float and string types use the blueprint function library, UInkpotValueLibrary, to convert from one to the other.<br>

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

	/* Get an Inkpot List from an Ink List */
	FInkpotList InkpotValueAsList(FInkpotValue Value);    

![MakeInkpotValue](Documentation/InpotValueAs.png)

![MakeInkpotValue](Documentation/MakeInkpotValue.png)

Alternatively, values can be set directly through the InkpotStory object using one of the Set or Get functions.<br>
These functions also report whether the variable exists in the script via the return parameter *Success*. 

	void SetBool(const FString &Variable, bool bValue, bool &Success );
	void GetBool(const FString &Variable, bool &ReturnValue, bool &Success );

	void SetInt(const FString &Variable, int32 Value, bool &Success );
	void GetInt(const FString &Variable, int32 &ReturnValue, bool &Success );

	void SetFloat( const FString& Variable, float Value, bool &Success );
	void GetFloat( const FString& Variable, float &ReturnValue, bool &Success );

	void SetString( const FString& Variable, const FString& Value, bool &Success );
	void GetString( const FString& Variable, FString &ReturnValue, bool &Success );

	void SetEmpty( const FString& Variable );

![StorySetNGet](Documentation/StorySetNGet.png)

To check if a variable simply exists, call this function.<br>

	bool IsVariableDefined( const FString& Variable );

![VariableExists](Documentation/VariableExistence.png)

### List types
Ink's List type is a litte more complex. It requires a reference to an *origin* defined within the story on creation.<br>
Ink's List has a corresponding type FInkpotList that can be used easily within blueprints.<br>
To create an Inkpot list to within blueprints use one of the following functions from UInkpotListLibrary.<br>

	/* converts comma delimited list of item names to a new inkpotlist */
	FInkpotList MakeInkpotList(UInkpotStory *Story, FString Origin, FString Value);

	/* converts string array of item names to a new inkpotlist */
	FInkpotList MakeInkpotListFromStringArray(UInkpotStory *Story, FString Origin, TArray<FString> Values);

![MakeInkpotList](Documentation/MakeInkpotList.png)

To get an Inkpot list from a Inkpot value, simply use the conversion function from UInkpotValueLibrary.<br>

	/* Get an array of strings from an Ink List */
	FInkpotList InkpotValueAsList(FInkpotValue Value);

![InkpotValueAsList](Documentation/InkpotValueAsList.png)

Once you have an inkpot list, you can manipulate it as you would in a regular Ink script using the following functions.<br> 

	/* prints the contents of the list to a comma delimeted string. */
	void ToString( const FInkpotList &Value, FString &ReturnValue, bool bUseOrigin );

	/* Converts an inkpot list to an array of strings. */
	void ToStringArray(const FInkpotList &Value, TArray<FString> &ReturnValue, bool bUseOrigin);

	/* Returns a new list that is the combination of both lists passed in. 
	Equivalent to calling (A + B) in ink.. */
	FInkpotList Union(const FInkpotList &A, const FInkpotList &B);

	/* Returns a new list that is the intersection of both lists passed in. 
	Equivalent to calling (A ^ B) in ink.
	FInkpotList Intersect(const FInkpotList &A, const FInkpotList &B);

	/* Returns true if there is an intersection of both lists passed in, 
	bool HasIntersection(const FInkpotList &A, const FInkpotList &B);

	/* Returns a new list that is the first list with items in the second list removed. 
	Equivalent to calling (A - B) in ink.*/
	FInkpotList Without( const FInkpotList &A, const FInkpotList &B );

	/* Returns true if the the first list contains all the items that are in the second list. 
	Equivalent to calling (A ? B) in ink.
	bool ContainsList( const FInkpotList &Source, const FInkpotList &Querant );

	/* Returns true if the list contains an item matching the given name. 
	bool ContainsItem(const FInkpotList &Source, const FString &ItemName);

	/* Returns true if all the item values in the first list 
	are greater than all the item values in the second list. 
	Equivalent to calling (A > B) in ink. */
	bool GreaterThan(const FInkpotList &A, const FInkpotList &B);

	/* Returns true if the item values in the first list overlap 
	or are all greater than the item values in the second list. 
	Equivalent to (A >= B) in ink. */
	bool GreaterThanOrEquals(const FInkpotList &A, const FInkpotList &B);

	/* Returns true if all the item values first list are 
	less than all the item values in the second list. 
	Equivalent to calling (A < B) in ink. */
	bool LessThan(const FInkpotList &A, const FInkpotList &B);

	/* Returns true if the item values in the first list overlap 
	or are all less than the item values in the second list. 
	Equivalent to (A <= B) in ink. */
	bool LessThanOrEquals(const FInkpotList &A, const FInkpotList &B);

	/* Returns true if the both lists contain the same items, false otherwise. */
	bool Equals(const FInkpotList &A, const FInkpotList &B);

	/* Returns a list containing the min item from the passed in list. 
	Equivalent of calling ( LIST_MIN( A ) ) in ink. */
	FInkpotList MinItem(const FInkpotList &A);

	/* Returns a list containing the max item from the passed in list. 
	Equivalent of calling ( LIST_MAX( A ) ) in ink. */
	FInkpotList MaxItem(const FInkpotList &A);

	/* Returns a list containing the inverse of the list passed in 
	with respect to the origin. 
	Equivalent of calling LIST_INVERT( A ) in ink*/
	FInkpotList Inverse(const FInkpotList &A);

	/* Returns a list containing the all of the items 
	as defined by the list origin. 
	Equivalent of calling LIST_ALL( A ).*/
	FInkpotList All(const FInkpotList &A);

![ListOperations](Documentation/ListOperations.png)	

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

## External Functions
As of version 0.2.20 of Inkpot, you can bind external C++ and Blueprint functions to Ink, so that they can be called directly from the Ink script.<br>

### Hooking up to Blueprints
Here are the steps you need to follow to bind a _Blueprint_ function to Ink.<br>
First, in your ink script define the external function as you normally would.<br>
( don't forget to reimport your Ink files )<br> 

	EXTERNAL MyExternalBPFunction( ABoolean, TheAnswer, PI, Message, AreasVisited )

Use the _Bind External Function_ node, to declare the binding to your story, like so<br>

![BindeExternalNode](Documentation/BindeExternalNode.png)

_Function Name_ is what we called the function when we declared it in the ink script, this should match.<br>

_Function_ should link to a Blueprint function matching the general signature of _FInkpotValue MyFunction( const TArray\<FInkpotValue\> &Values )_<br>

The easiest way of doing this is as follows:<br>

1. Drag off the Function pin, then select _Create Event_ under _Event Dispatchers_<br>

![BindExternalEventDispatcher](Documentation/BindExternalEventDispatcher.png)

2. Then from the new _Create Event_ node, select _Create a matching function_.<br>

![BindExternalCreateMatching](Documentation/BindExternalCreateMatching.png)

3. All going well, you will be presented with your shiny new Blueprint function.<br>
Which here simply prints the first parameter (a string).<br>

![BindExternalFunctionBody](Documentation/BindExternalFunctionBody.png)

### Parameter Passing
Parameters from Ink are passed into the external function as an array of Inkpot values.<br>
The parameters are ordered in the array with the left most function argument being the first at index 0 of the array, the second argument will be at index 1 and so on.<br>

As values in Ink are not typed, you'll need to use helper functions to convert from Inkpot Values to the typed values that Blueprints use.<br>
To make things easier, here are some helper functions to convert the elements of the Inkpot Value arrays.<br>

![BindExternalHelpers](Documentation/BindExternalHelpers.png)

See the demo for a more complete example.<br>

---

## Debug Log
Inkpot has it's own debug category, which you can filter the OutputLog by.
This can be turned off by settings and CVars.

![DebugLog](Documentation/DebugLog.png)

---

## The Blotter ( Inkpot Debug )
The Blotter is an Unreal editor utility that can be used to view and set values whilst your story is running in the Unreal Editor.<br>

### Running the Blotter
The blotter can be found in the content folder of the Inkpot plugin.<br>
To see this in the content browser, make sure you have *Show Plugin Content* checked in the folder settings.<br><br>
![ShowPluginContent](Documentation/ShowPluginContent.png)

To run it, right click on *Inkpot Debug* and select *Run Editor Utility Widget*.<br><br>
![RunEditorUtility](Documentation/RunEditorUtility.png)

You should see this. <br><br>
![BlotterScreen](Documentation/BlotterScreen.png)

### Blotter Sections 

* **Current**<br>
Shows the current line of text for the story.<br><br>
* **Choices**<br> 
Lists the current set of choices for the story.<br><br>
* **Tags**<br>
lists current and global tags if any.<br><br>
* **Flow**<br>
Shows current, and all other active flow names.<br><br>
* **Variables**<br>
Shows a list of all variables defined.<br> 
Bools, ints, floats and strings can be edited directly.<br>
List types require that the item is defined in an origin, ie a LIST declaration within the ink script.<br>
When adding items from other lists, prefix the item name with the origin name of the other list.<br><br>
* **Origins**<br>
Shows all of the list origins that are present in the current story. <br>
A list origin defines the items that can be present in a list variable.<br>
A LIST declaration within an ink script declares both the origin and the variable, which can confuse at first.<br><br>

The blotter will update any time a continue happens or a variable is changed within the Ink runtime.<br>
You should therefore make sure that your game is in an inactive state before editing the values of variables.<br>
( the next inkpot update will likely feature some form of pause control )

---

## Settings and CVars
As of 0.4.20 Inkpot has settings to control its operation.
These can be found in the Plugins->Inkpot section of Project Settings.

![SettingsAndCVars](Documentation/SettingsAndCVars.png)

### Debug Log 
Turns debug log on or off.<br> 
CVar Inkpot.DebugLog<br>
Default is true.<br>

### Reload If Asset Changed
For runtime, reloads and restarts the story if the asset has been reimported ( mainly with auto-reimport on ). <br>
Allows for quick edits of ink whilst running the game. <br>
CVar Inkpot.ReloadIfAssetChanged<br>
Default is false.<br>

### Replay If Reloaded
For runtime, if the story has been reloaded, if this option is set, Inkpot will replay all choices, path & flow changes made to the current point.<br>
Allows for quick edits of ink scripts whilst running the game, 'live edit'. <br>
CVar Inkpot.ReplayIfReloaded<br>
Default is false.<br>

---

# Testing InkPlusPlus
We have 180 active tests in Inkpot that test the implementaion of the InkPlusPlus module.<br>
These can all be run through the *Session Frontend* within the Unreal editor.<br>
To run the tests, first open the *Session Frontend* from Tools, Session Frontend.<br>

![OpenSessionFrontEnd](Documentation/OpenSessionFrontEnd.png)

Select the automation panel, and click the check box next to **Inkpot**.<br>
Then hit the play button to start the tests.<br>

![RunTests](Documentation/RunTests.png)


