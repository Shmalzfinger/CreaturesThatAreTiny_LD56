/*
	Creatures That Are Tiny
	Copyright (c) 2024 Jan Salecker
*/

#include "raylib.h"
#include "resource_dir.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>		// for INT_MIN + INT_MAX

int posArray[2] = { 300, 400 };

const int worldOriginX = 0;
const int worldOriginY = 0;
const int worldSizeX = 1600;
const int worldSizeY = 900;

bool pause = false;
bool collision = false;

void setup() {
	// initialization


	
}

void draw() {
}

void drawPlayer(int x, int y) {
	DrawPixel(x, y, WHITE);
}

bool isInBoundaries(x, y) {
	// true if this condition is met
	if ((x > 0) && (x < (worldSizeX - 2)) && (y > 0) && (y < (worldSizeY - 2))) {
		return true;
	}
	else {
		return false;
	}
}

void handleInput() {

	if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
		if (posArray[0] < (worldSizeX - 1)) {
			posArray[0] += 1;
		} else {
			collision = true;
		}
	}
	if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
		if (posArray[0] > (worldOriginX + 1)) {
			posArray[0] -= 1;
		} else {
			collision = true;
		}
	}
	if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
		if (posArray[1] > (worldOriginY + 1)) {
			posArray[1] -= 1;
		} else {
			collision = true;
		}
	}
	if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
		if (posArray[1] < (worldSizeY - 1)) {
			posArray[1] += 1;
		} else {
			collision = true;
		}
	}

	float mouseWheel = GetMouseWheelMove();
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

	int playerX = 300;
	int playerY = 400;
	Vector2 playerPos = (Vector2){ 300, 400 };

	// audio stuff
	InitAudioDevice();
	SetMasterVolume(0.03f);
	Music music = LoadMusicStream("resources/music/song_01.mp3");
	Sound snd_beep1 = LoadSound("resources/sfx/beep_01.wav");
	PlayMusicStream(music);

	Font fontMono = LoadFont("VeraMono.ttf");
	
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
	SetWindowMonitor(0);

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory - PNG ALPHA should be possible!?
	Texture glass = LoadTexture("m_glass_trans.png");

	// SetWindowIcon(Image image);

	HideCursor();

	// NOTE: Images are loaded in CPU memory(RAM); textures are loaded in GPU memory(VRAM)
	// NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)

	Image background = LoadImage("bg_01.png");						// Load image data into CPU memory (RAM)
	Color* colors = LoadImageColors(background);
	int backgroundWidth = background.width;
	Texture2D tex_background = LoadTextureFromImage(background);	// Image converted to texture, GPU memory (RAM -> VRAM)
	UnloadImage(background);										// Unload image data from CPU memory (RAM)

	background = LoadImageFromTexture(tex_background);				// Load image from GPU texture (VRAM -> RAM)
	UnloadTexture(tex_background);									// Unload texture from GPU memory (VRAM)

	tex_background = LoadTextureFromImage(background);				// Recreate texture from retrieved image data (RAM -> VRAM)
	// UnloadImage(background);										// Unload retrieved image data from CPU memory (RAM)


	//Image toEnlarge = LoadImage("bg_01.png");
	Image toEnlarge = LoadImage("bg_01.png");
	
	//Image Enlarged = ImageResizeNN(toEnlarge);
	// To draw a resized texture, you should SetTextureFilter(texture, TEXTURE_FILTER_POINT).

	// SetTargetFPS(60);

	// Count GetMonitorCount().toStr()
	//char str1[12] = "Mon Count: ";
	//int N = GetMonitorCount();
	//char str2[2];
	//_itoa(N, str2, 10);
	//strcat(str1, str2);

	// game loop

	while (!IsMusicReady(music)) {
		//wait(5);
	}

	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		UpdateMusicStream(music);   // Update music buffer with new stream data
		
		// Restart music playing (stop and play)
		if (IsKeyPressed(KEY_SPACE))
		{
			StopMusicStream(music);
			PlayMusicStream(music);
		}

		// Pause/Resume music playing
		if (IsKeyPressed(KEY_P))
		{
			pause = !pause;

			if (pause) PauseMusicStream(music);
			else ResumeMusicStream(music);
		}
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
		// DrawText("Creatures That Are Tiny", 20, 20, 20, WHITE);
		DrawTextEx(fontMono, "Creatures That Are Tiny", (Vector2) { 20, 20 }, 24, 1, WHITE); // Draw text using font and additional parameters
		DrawText("by Treeation", 20, 40, 20, BEIGE);

		DrawText("MOVE THE CROPPING RECTANGLE WITH MOUSE", 20, 120, 20, DARKPURPLE);

		char str3[50] = "Current Monitor: ";
		int nrMon = GetCurrentMonitor();
		char str4[1];
		_itoa(nrMon, str4, 10);
		strcat(str3, str4);
		DrawText(str3, 20, 140, 20, BLUE);

		char str5[4];
		_itoa(GetMouseX(), str5, 10);
		//_itoa(posArray[0], str5, 10);
		DrawText(str5, 20, 200, 20, RED);

		char str6[4];
		_itoa(GetMouseY(), str6, 10);
		//_itoa(posArray[1], str6, 10);
		DrawText(str6, 80, 200, 20, RED);

		char str7[16];
		char str8[16];
		char str9[16];
		//Color GetPixelColor(void* srcPtr, int format);
		int index = (posArray[1] * backgroundWidth) + posArray[0];
		Color pixel = colors[index];
		_itoa(pixel.r, str7, 10);
		_itoa(pixel.g, str8, 10);
		_itoa(pixel.b, str9, 10);
		// TO DO set 2d array with boolean values that represents a grid of colliding walls (colors),
		// so we can check at input handling if we are colliding 

		//char pixelStr[64] = { strcat(str7, str8) };
		//pixelStr[] = strcat(pixelStr, str9);
		DrawText(str7, 20, 240, 20, RED);
		DrawText(str8, 80, 240, 20, GREEN);
		DrawText(str9, 140, 240, 20, BLUE);

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
		/*Rectangle rectSrc = { 0, 0, 64, 64 };
		Rectangle rectDst = { 0, 0, 64, 64 };
		ImageDraw(&toEnlarge, background, rectDst, rectSrc, WHITE);*/	// Draw a source image within a destination image (tint applied to source)
		
		// Rectangle crop = { GetMouseX(), GetMouseY(), GetMouseX() + 32, GetMouseY() + 32 };
		
		int cropX1 = 0;
		int cropY1 = 0;
		int cropSize = 64;
		// int cropX1 = max(cropSize, GetMouseX());
		if ((GetMouseX() > worldOriginX + cropSize) && (GetMouseX() < (worldSizeX - cropSize))) {
			cropX1 = GetMouseX(); //+ cropSize;
		}
		int cropX2 = cropX1 + cropSize;
		if ((GetMouseY() > worldOriginY + cropSize) && (GetMouseY() < (worldSizeY - cropSize))) {
			cropY1 = GetMouseY(); // +cropSize;
		}
		// int cropY1 = min(worldSizeY - cropSize, GetMouseY());
		int cropY2 = cropY1 + cropSize;
		Rectangle cropRect = { cropX1, cropY1, cropSize, cropSize };
		
		ImageCrop(& toEnlarge, cropRect);
		Texture2D newTexture = LoadTextureFromImage(toEnlarge);

		
		// Texture2D 
		//DrawCircle(GetMouseX() - 32, GetMouseY() - 32, 64, YELLOW);	// Draw a color-filled circle
		//DrawTexture(newTexture, GetMouseX() - 128, GetMouseY() - 128, RED);
		//Rectangle rect3 = { GetMouseX() - 16, GetMouseY() - 16, 64, 64 };
		Vector2 position = (Vector2){ GetMouseX() + 16, GetMouseY() + 16 };
		
		int frameWidth = newTexture.width / 6;
		int frameHeight = newTexture.height;
		// Source rectangle (part of the texture to use for drawing)
		Rectangle sourceRec = { 0.0f, 0.0f, (float)frameWidth, (float)frameHeight };

		// Destination rectangle (screen rectangle where drawing part of texture)
		Rectangle destRec = { screenWidth / 2.0f, screenHeight / 2.0f, frameWidth * 2.0f, frameHeight * 2.0f };

		Rectangle blonkRect = { GetMouseX() + 16, GetMouseY() + 64, 64, 64};
		// Origin of the texture (rotation/scale point), it's relative to destination rectangle size
		Vector2 origin = { (float)frameWidth, (float)frameHeight };

		int rotation = 0;

		DrawTexturePro(newTexture, cropRect, blonkRect, origin, (float)rotation, WHITE);
		// here upscaling works:
		//DrawTextureEx(newTexture, position, 0, 4, WHITE);  // Draw a Texture2D with extended parameters
		
		//DrawTextureRec(newTexture, rect3, position, SKYBLUE);
		// Vector2 origin = (Vector2){ 0, 0 };
		//Vector2 origin = (Vector2){ 0, 0 };
		// Rectangle rectSrc = { 0, 0, 32, 32 };
		// Rectangle rectDst = { 0, 0, 64, 64 };
		// DrawTexturePro(newTexture, rectSrc, rectDst, origin, 0, WHITE); // Draw a part of a texture defined by a rectangle with 'pro' parameters
		DrawTexture(glass, GetMouseX(), GetMouseY(), WHITE);
		DrawTexture(newTexture, 1200, 700, WHITE);

		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();

		UnloadTexture(newTexture);

		handleInput();

		if (collision == true) {
			if (!IsSoundPlaying(snd_beep1)) {
				PlaySound(snd_beep1);
			}
			collision = false;
		}

		worldTick();
	}



	// cleanup
	ShowCursor();
	// unload our texture so it can be cleaned up
	UnloadTexture(glass);
	UnloadImage(toEnlarge);
	UnloadFont(fontMono);
	void UnloadImageColors(Color * colors);

	UnloadMusicStream(music);   // Unload music stream buffers from RAM
	UnloadSound(snd_beep1);
	CloseAudioDevice();         // Close audio device (music streaming is automatically stopped)

	// destory the window and cleanup the OpenGL context
	CloseWindow();

	return 0;

}
