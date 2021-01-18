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
	int bExit = 0;
	int width = 512;
	int height = 512;
	
	RGBox fillerA, fillerB, fillerD, fillerE,
		winBox, menu, menuButtonA, menuButtonB, content,
		content2, resolutionA, resolutionB, resolutionC, exit;
	
	InitWindow(width, height, "Raylib");
	
	GuiEnable();
	SetExitKey(-1);
	
	while (!WindowShouldClose() && !bExit)
	{
		// Layout
		CreateRootRGBox(RGBOX_HORIZONTAL, width, height, &winBox);
		
		// Side menu
		CreateRGBox(&winBox, RGBOX_VERTICAL, 0, 128, 0, &menu);
		
		CreateRGBox(&menu, RGBOX_HORIZONTAL, 0, 0, 28, &menuButtonA);
		menuButtonA.margin[RGBOX_TOP] = 4;
		menuButtonA.margin[RGBOX_LEFT] = 4;
		menuButtonA.margin[RGBOX_RIGHT] = 4;
		
		CreateRGBox(&menu, RGBOX_HORIZONTAL, 0, 0, 28, &menuButtonB);
		menuButtonB.margin[RGBOX_TOP] = 4;
		menuButtonB.margin[RGBOX_LEFT] = 4;
		menuButtonB.margin[RGBOX_RIGHT] = 4;
		
		// Content
		CreateRGBox(&winBox, RGBOX_HORIZONTAL, 1, 0, 0, &content);
		
		CreateRGBox(&content, RGBOX_HORIZONTAL, 1, 0, 0, &fillerA); // Filler for centering
		CreateRGBox(&content, RGBOX_VERTICAL, 0, 128, 0, &content2);
		CreateRGBox(&content, RGBOX_HORIZONTAL, 1, 0, 0, &fillerB); // Filler for centering
		
		// Main buttons
		CreateRGBox(&content2, RGBOX_VERTICAL, 1, 0, 0, &fillerD); // Filler for centering
		
		CreateRGBox(&content2, RGBOX_VERTICAL, 0, 0, 24, &resolutionA);
		CreateRGBox(&content2, RGBOX_VERTICAL, 0, 0, 28, &resolutionB);
		resolutionB.margin[RGBOX_TOP] = 4;
		
		CreateRGBox(&content2, RGBOX_VERTICAL, 0, 0, 28, &resolutionC);
		resolutionC.margin[RGBOX_TOP] = 4;
		
		CreateRGBox(&content2, RGBOX_VERTICAL, 0, 0, 36, &exit);
		exit.margin[RGBOX_TOP] = 12;
		
		CreateRGBox(&content2, RGBOX_VERTICAL, 1, 0, 0, &fillerE); // Filler for centering
		
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
