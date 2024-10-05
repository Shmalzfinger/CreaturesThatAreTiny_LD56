/*
	Creatures That Are Tiny
	Copyright (c) 2024 Jan Salecker
*/

#include "raylib.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#include <stdio.h>
#include <stdlib.h>

int posArray[2] = { 300, 400 };

void setup() {
	// initialization
	//Font LoadFont("VeraMono.ttf");

	
}

void draw() {
}

void drawPlayer(int x, int y) {
	DrawPixel(x, y, WHITE);
}

void handleInput() {

	//int pressedKey = GetKeyPressed();
	//
	//switch (pressedKey) {
	//	case 87:	posArray[1] = posArray[1] - 1;	break;
	//	case 256:	printf("ESCAPED! \n");			break;
	//	default:									break;
	//}

	if (IsKeyDown(KEY_RIGHT)) {
		posArray[0] += 1;
	}
	if (IsKeyDown(KEY_LEFT)) posArray[0] -= 1;
	if (IsKeyDown(KEY_UP)) posArray[1] -= 1;
	if (IsKeyDown(KEY_DOWN)) posArray[1] += 1;

	//if (pressedKey != 0) {
	//	printf("KEY: %i\n", pressedKey);
	//}

	float mouseWheel = GetMouseWheelMove();		// Get mouse wheel movement for X or Y, whichever is larger
	if (mouseWheel > 0) {
		printf("mouse wheel UP: %f\n", mouseWheel);	
	}
	else if (mouseWheel < 0) {
		printf("mouse wheel DOWN: %f\n", mouseWheel);
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

	Image background = LoadImage("bg_01.png");						// Load image data into CPU memory (RAM)
	Texture2D tex_background = LoadTextureFromImage(background);	// Image converted to texture, GPU memory (RAM -> VRAM)
	UnloadImage(background);										// Unload image data from CPU memory (RAM)

	background = LoadImageFromTexture(tex_background);				// Load image from GPU texture (VRAM -> RAM)
	UnloadTexture(tex_background);									// Unload texture from GPU memory (VRAM)

	tex_background = LoadTextureFromImage(background);				// Recreate texture from retrieved image data (RAM -> VRAM)
	// UnloadImage(background);										// Unload retrieved image data from CPU memory (RAM)


	//Image toEnlarge = LoadImage("bg_01.png");
	Image toEnlarge = LoadImage("bg_01.png");

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
		// Image Copy(Image image);	// Create an image duplicate (useful for transformations)
		// TO DO: create image from current screen buffer, crop to mouse area, enlarge and display!
		// OR!!!
		// same, but add player sprite and crop it by glass texture
		// Image imageCopy = LoadImageFromScreen();
		//Image backgroundCopy = LoadImageFromTexture(tex_background);

		// Rectangle rect1 = { GetMouseX() - 32, GetMouseY() - 32, 64, 64};
		Rectangle rect1 = { 0, 0, 64, 64 };
		// Image backgroundCopy = ImageFromImage(background, rect1);        // Create an image from another image piece
		// struct Color;                  // Color, 4 components, R8G8B8A8 (32bit)
		Rectangle rectSrc = { 0, 0, 64, 64 };
		Rectangle rectDst = { 0, 0, 64, 64 };
		// 4294967295
		// Color tintCol = { 4294967295 / 2, 4294967295, 4294967295, 255 };
		ImageDraw(&toEnlarge, background, rectDst, rectSrc, WHITE);	// Draw a source image within a destination image (tint applied to source)
		Texture2D newTexture = LoadTextureFromImage(toEnlarge);
		//DrawTexture(newTexture, GetMouseX() - 128, GetMouseY() - 128, RED);
		Rectangle rect3 = { GetMouseX() - 32, GetMouseY() - 32, 64, 64 };
		Vector2 position = (Vector2){ GetMouseX(), GetMouseY() };
		DrawTextureEx(newTexture, position, 0, 4, WHITE);  // Draw a Texture2D with extended parameters
		//DrawTextureRec(newTexture, rect3, position, SKYBLUE);
		// Vector2 origin = (Vector2){ 0, 0 };
		Vector2 origin = (Vector2){ 0, 0 };
		// Rectangle rectSrc = { 0, 0, 32, 32 };
		// Rectangle rectDst = { 0, 0, 64, 64 };
		// DrawTexturePro(newTexture, rectSrc, rectDst, origin, 0, WHITE); // Draw a part of a texture defined by a rectangle with 'pro' parameters
		DrawTexture(glass, GetMouseX(), GetMouseY(), WHITE);

		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();

		UnloadTexture(newTexture);

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