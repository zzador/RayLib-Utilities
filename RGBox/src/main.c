// Small example for RGBox layout

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <raylib.h>

#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_RICONS
#include <raygui.h>
#undef RAYGUI_IMPLEMENTATION

#include "rgbox.h"

int main(int argc, char* pArgv[])
{
	int bExit = 0,
		width = 512,
		height = 512;
	
	RGBox fillerA, fillerB, fillerD, fillerE,
		  winBox, menu, menuButtonA, menuButtonB, outerContent,
		  innerContent, resolutionA, resolutionB, resolutionC, exit;
	
	InitWindow(width, height, "Raylib");
	
	GuiEnable();
	SetExitKey(-1);
	
	while (!WindowShouldClose() && !bExit)
	{
		// Layout
		CreateRootRGBox(RGBOX_HORIZONTAL, width, height, &winBox);
		
		// Side menu
		CreateRGBox(&winBox, RGBOX_VERTICAL, 0, 0, 0, &menu);
		
		CreateRGBox(&menu, 0, 0, 128, 28, &menuButtonA);
		menuButtonA.paddingTop = 4;
		menuButtonA.paddingLeft = 4;
		menuButtonA.paddingRight = 4;
		
		CreateRGBox(&menu, 0, 0, 128, 28, &menuButtonB);
		menuButtonB.paddingTop = 4;
		menuButtonB.paddingLeft = 4;
		menuButtonB.paddingRight = 4;
		
		// Content
		CreateRGBox(&winBox, RGBOX_HORIZONTAL, 1, 0, 0, &outerContent);
		
		CreateRGBox(&outerContent, 0, 1, 0, 0, &fillerA); // Filler for centering
		CreateRGBox(&outerContent, RGBOX_VERTICAL, 0, 0, 0, &innerContent);
		CreateRGBox(&outerContent, 0, 1, 0, 0, &fillerB); // Filler for centering
		
		// Main buttons
		CreateRGBox(&innerContent, 0, 1, 0, 0, &fillerD); // Filler for centering
		
		CreateRGBox(&innerContent, 0, 0, 128, 24, &resolutionA);
		CreateRGBox(&innerContent, 0, 0, 128, 28, &resolutionB);
		resolutionB.paddingTop = 4;
		
		CreateRGBox(&innerContent, 0, 0, 128, 28, &resolutionC);
		resolutionC.paddingTop = 4;
		
		CreateRGBox(&innerContent, 0, 0, 128, 36, &exit);
		exit.paddingTop = 12;
		
		CreateRGBox(&innerContent, 0, 1, 0, 0, &fillerE); // Filler for centering
		
		// Calculate layout
		LayoutRGBox(&winBox);
		
		BeginDrawing();
		
			ClearBackground(RAYWHITE);
			
			DrawRectangleRec(menu.rectangle, (Color){128, 128, 128, 255});
			
			GuiButton(menuButtonA.rectangle, "Button A");
			GuiButton(menuButtonB.rectangle, "Button B");
			
			if (GuiButton(resolutionA.rectangle, "512 x 512"))
			{
				width = 512;
				height = 512;
				SetWindowSize(width, height);
			}
			
			if (GuiButton(resolutionB.rectangle, "512 x 768"))
			{
				width = 512;
				height = 768;
				SetWindowSize(width, height);
			}
			
			if (GuiButton(resolutionC.rectangle, "1280 x 512"))
			{
				width = 1280;
				height = 512;
				SetWindowSize(width, height);
			}
			
			if (GuiButton(exit.rectangle, "Exit"))
				bExit = 1;
			
		EndDrawing();
	}
	
	CloseWindow();
	
	return 0;
}
