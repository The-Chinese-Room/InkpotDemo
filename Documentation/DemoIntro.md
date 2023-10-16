# Inkpot Demo 

For more info see InpotDemo/Documentation

## Folder structure 

	Content
		InkpotDemo
			Framework 
				BP_InkpotDemoGameMode 
					- game mode Blueprint, creates UI, handles Inkpot OnStoryBegin.
			Maps
				Demo 		
					- A small demo of how Ink scripts can interct with level blueprints and actors. 
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
					- the main display widget, for this demo this is where most of the Ink handling logic lies.
				WBP_Choice
					- widget for choices that can be made in an ink story, used by WBP_Display.

