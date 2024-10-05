/*
	Creatures That Are Tiny
	Copyright (c) 2024 Jan Salecker
*/

#include "raylib.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#include <stdio.h>
#include <stdlib.h>

void setup() {
	// initialization
}

void draw() {
	// drawing

}

void drawPlayer(int x, int y) {
	DrawPixel(x, y, WHITE);
	// DrawPixelV(pos, WHITE);
}

void handleInput() {

	int pressedKey = GetKeyPressed();	// Get key pressed (keycode), call it multiple times for keys queued, returns 0 when the queue is empty
	
	switch (pressedKey) {
		case 256: printf("ESCAPED! \n");	break;
		default:							break;
	}

	if (pressedKey != 0) {
		printf("KEY: %i\n", pressedKey);
	}

}

// advance game loop; calculate stuff
void worldTick() {

}

int main ()
{
	setup();

	const int worldSizeX = 1600;
	const int worldSizeY = 900;

	int playerX = 300;
	int playerY = 400;
	Vector2 playerPos = (Vector2){ 300, 400 };
	int posArray[2] = {300, 400};
	
	// sounds
	//struct Wave; // Wave type, defines audio wave data
	//struct Sound; // Basic Sound source and buffer
	//struct Music; // Music type (file streaming from memory)

	// Tell the window to use vysnc and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);


	// Create the window and OpenGL context
	int screenWidth = GetScreenWidth();                                   // Get current screen width
	int screenHeight = GetScreenHeight();
	int renderX = GetRenderWidth();                                   // Get current render width (it considers HiDPI)
	int renderY = GetRenderHeight();                                  // Get current render height (it considers HiDPI)

	printf("screenWidth %i\n", screenWidth);
	printf("screenHeight %i\n", screenHeight);
	printf("renderX %i\n", renderX);
	printf("renderY %i\n", renderY);

	InitWindow(screenWidth, screenHeight, "Creatures That Are Tiny");
	ToggleFullscreen();

	// DEBUG stuff! second monitor used to see debugging window -> remove for RELEASE!
	SetWindowMonitor(1);

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory - PNG ALPHA should be possible!?
	Texture glass = LoadTexture("m_glass_trans.png");

	// SetWindowIcon(Image image);

	HideCursor();

	// NOTE: Images are loaded in CPU memory(RAM); textures are loaded in GPU memory(VRAM)
	// NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)

	Image background = LoadImage("bg_01.png");			// Load image data into CPU memory (RAM)
	Texture2D tex_background = LoadTextureFromImage(background);	// Image converted to texture, GPU memory (RAM -> VRAM)
	UnloadImage(background);										// Unload image data from CPU memory (RAM)

	background = LoadImageFromTexture(tex_background);					// Load image from GPU texture (VRAM -> RAM)
	UnloadTexture(tex_background);											// Unload texture from GPU memory (VRAM)

	tex_background = LoadTextureFromImage(background);							// Recreate texture from retrieved image data (RAM -> VRAM)
	UnloadImage(background);												// Unload retrieved image data from CPU memory (RAM)

	// SetTargetFPS(60);

	// Count GetMonitorCount().toStr()
	char str1[12] = "Mon Count: ";
	int N = GetMonitorCount();
	char str2[2];
	_itoa(N, str2, 10);
	strcat(str1, str2);

	// game loop
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		//draw();
		BeginDrawing();

		// Setup the backbuffer for drawing (clear color and depth buffers)
		ClearBackground(BLACK);

		// background
		// DrawTexture(tex_background, screenWidth / 2 - tex_background.width / 2, screenHeight / 2 - tex_background.height / 2, WHITE);
		DrawTexture(tex_background, screenWidth, screenHeight, WHITE);

		// draw avatar
		drawPlayer(posArray[0], posArray[1]);

		// draw some text using the default font
		DrawText("Creatures That Are Tiny", 20, 20, 20, WHITE);
		DrawText("by Treeation", 20, 40, 20, WHITE);

		DrawText(str1, 20, 120, 20, BLUE);		
		
		char str3[50] = "Current Monitor: ";
		int O = GetCurrentMonitor();
		char str4[1];
		_itoa(O, str4, 10);
		strcat(str3, str4);
		DrawText(str3, 20, 140, 20, BLUE);

		char str5[3];
		_itoa(GetMouseX(), str5, 10);
		DrawText(str5, 20, 200, 20, RED);

		char str6[3];
		_itoa(GetMouseY(), str6, 10);
		DrawText(str6, 80, 200, 20, RED);

		// draw our texture to the screen
		// if this is currently the glass, then do magnifying stuff!
		//Image ImageCopy(Image image);	// Create an image duplicate (useful for transformations)

		DrawTexture(glass, GetMouseX(), GetMouseY(), WHITE);

		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();


		handleInput();

		worldTick();
	}



	// cleanup
	ShowCursor();
	// unload our texture so it can be cleaned up
	UnloadTexture(glass);

	// destory the window and cleanup the OpenGL context
	CloseWindow();

	return 0;

}