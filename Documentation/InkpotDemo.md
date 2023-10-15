# Inkpot Demo

## Editing Ink source
Use Inky, which you can get from here.

## Importing ink files 
To import, drag the ink source file into a content browser window within the Unreal Editor. 
The will create the InkpotStoryAsset, that contains the compiled JSON source.
![DragFile](images/DragFile.png)

## Setting up auto reimport
Open editor, preferences. 
Look Under general, loading and saving, auto reimpor.
add new 'Directories to Monitor' entry .
Include the source folder, map directory to the corresponding folder in the unreal content browser.
![AutoReimportOptions](images/AutoReimport.png)

## Starting a story 
In blueprints, get the Inkpot subsystem and call BeginStory, passing in the InpotStoryAsset. 
![Alt text](images/BeginStory.png)
This returns the runtime InkpotStory that executes the script.  
In this demo, the stories are started by the Level Blueprint.

## Getting story updates
In this demo, the UI is hooked up 
### Continue 
### Choices 
### Selection 

## Variables
### Setting a variable
### Getting a variable
### Change notification

## Flow control





