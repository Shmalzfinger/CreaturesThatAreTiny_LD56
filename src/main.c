/*
	Creatures That Are Tiny
	Copyright (c) 2024 Jan Salecker
*/

#include "raylib.h"
#include "resource_dir.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>		// possibly needed for INT_MIN + INT_MAX

int posArray[2] = { 300, 400 };

const int worldOriginX = 0;
const int worldOriginY = 0;
const int worldSizeX = 1600;
const int worldSizeY = 900;

// TODO: Update Goal Center!!!
//Vector2 goalCenter = { 1426, 753 };
Vector2 goalCenter = { 1426, 753 };

bool pause = false;
bool collision = false;
bool win = false;

bool RightForbidden = false;
bool LeftForbidden = false;
bool UpForbidden = false;
bool DownForbidden = false;

int playerSpriteIndexX = 0;  int playerSpriteIndexY = 0;
//int toolsMax = 3;
//int toolSelected = 0;

void setup() {
}

void draw() {
}

void drawPlayer(int x, int y) {
	DrawPixel(x, y, WHITE);
}

bool isInBoundaries(x, y) {
	if ((x > 0) && (x < (worldSizeX - 2)) && (y > 0) && (y < (worldSizeY - 2))) {
		return true;
	}
	else {
		return false;
	}
}

// function declarations
//bool isColorForbidden(int, int, Color);
void displayWin();

void cleanup();

void handleInput() {
	// optimize: only check if a button is pressed
	if (GetKeyPressed != 0) {
	
		if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
			if ((posArray[0] < (worldSizeX - 1)) && !RightForbidden) {
				posArray[0] += 1;
				playerSpriteIndexX = 3;
			} else {
				collision = true;
			}
		}
		if ((IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) && !LeftForbidden) {
			if (posArray[0] > (worldOriginX + 1)) {
				posArray[0] -= 1;
				playerSpriteIndexX = 0;
			} else {
				collision = true;
			}
		}
		if ((IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) && !UpForbidden) {
			if (posArray[1] > (worldOriginY + 1)) {
				posArray[1] -= 1;
				playerSpriteIndexY = 0;
			} else {
				collision = true;
			}
		}
		if ((IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) && !DownForbidden) {
			if (posArray[1] < (worldSizeY - 1)) {
				posArray[1] += 1;
				playerSpriteIndexY = 3;
			} else {
				collision = true;
			}
		}
	}	// if no KEY was pressed 
	else {
		playerSpriteIndexX = 2; playerSpriteIndexY = 2;
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
	SetMasterVolume(0.6f);
	// RANDOMIZE SONG?
	Music music = LoadMusicStream("resources/music/song_01.mp3");
	Music musicWin = LoadMusicStream("resources/music/song_02.mp3");
	//Sound snd_beep1 = LoadSound("resources/sfx/beep_01.wav");
	Sound snd_beep1 = LoadSound("resources/sfx/hit_01.wav");
	PlayMusicStream(music);

	Font fontMono = LoadFont("VeraMono.ttf");
	
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
	//SetWindowMonitor(1);

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	Image imgSpriteSheet = LoadImage("spritesheet_128_3.png");
	Texture playerSheet = LoadTextureFromImage(imgSpriteSheet);
	//Texture playerSheet = LoadTexture("spritesheet_512.png");

	int playerSpritesTotal = 16;
	float playerSpritesTotalX = 4;
	float playerSpritesTotalY = 4;
	Rectangle playerRec = { 0.0f, 0.0f, (float)playerSheet.width / playerSpritesTotalX, (float)playerSheet.height / playerSpritesTotalY };
	// Rectangle playerRec = { 0, 0, playerSheet.width, playerSheet.height };

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
	//UnloadImage(background);										// Unload image data from CPU memory (RAM)

	background = LoadImageFromTexture(tex_background);				// Load image from GPU texture (VRAM -> RAM)
	UnloadTexture(tex_background);									// Unload texture from GPU memory (VRAM)

	tex_background = LoadTextureFromImage(background);				// Recreate texture from retrieved image data (RAM -> VRAM)
	// UnloadImage(background);										// Unload retrieved image data from CPU memory (RAM)


	//Image toEnlarge = LoadImage("bg_01.png");
	Image toEnlarge = LoadImage("bg_01.png");

	//Image alphaMask = LoadImage("zoom_mask_smol.png");
	
	//Image Enlarged = ImageResizeNN(toEnlarge);
	// To draw a resized texture, you should SetTextureFilter(texture, TEXTURE_FILTER_POINT).

	// SetTargetFPS(60);

	// Count GetMonitorCount().toStr()
	//char str1[12] = "Mon Count: ";
	//int N = GetMonitorCount();
	//char str2[2];
	//_itoa(N, str2, 10);
	//strcat(str1, str2);
	

	// splash screen
	Image splashImage = LoadImage("splash.png");
	Texture splashTex = LoadTextureFromImage(splashImage);
	int i;
	int spacing = 100;
	for (i = 0; i < 250; i++) {
		BeginDrawing();
		ClearBackground(BLACK);
		DrawTexture(splashTex, screenWidth, screenHeight, WHITE);
		//DrawRectangle(worldOriginX + spacing, worldOriginY + spacing, worldSizeX - spacing * 2, worldSizeY - spacing * 2, (Color) { 112, 31, 126, 212 });
		DrawTextEx(fontMono, "Treeation 2024", (Vector2) { worldSizeX - 400, worldSizeY - 200 }, 36, 8, BROWN);
		//DrawTextEx(fontMono, "creatures that are tiny", (Vector2) { worldOriginX + spacing * 2, worldOriginY + spacing * 3 }, 64, 12, GOLD);
		DrawText("Made for Ludum Dare 56. Thank you very much & enjoy!", worldSizeX - 600, worldOriginY + spacing * 7.5 + spacing / 2, 20, BLACK);
		DrawText("Made for Ludum Dare 56. Thank you very much & enjoy!", worldSizeX - 604, worldOriginY + spacing * 7.47 + spacing / 2, 20, WHITE);
		//if (IsImageReady(splashImage)) {
		//	DrawImage(splashImage);
		//}
		EndDrawing();
	}
	UnloadTexture(splashTex);
		
	// game loop

	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		
		if (!win) UpdateMusicStream(music);   // Update music buffer with new stream data
		if (win) UpdateMusicStream(musicWin);
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

		// draw some text using the default font
		// DrawText("Creatures That Are Tiny", 20, 20, 20, WHITE);
		DrawTextEx(fontMono, "Creatures That Are Tiny", (Vector2) { 20, 20 }, 24, 1, WHITE); // Draw text using font and additional parameters
		DrawText("by Treeation", 20, 40, 20, BEIGE);

		DrawText("MOVE WITH WASD OR ARROW KEYS AND USE YOUR MOUSE :)", 20, 120, 20, DARKPURPLE);

		/*char str3[50] = "Current Monitor: ";
		int nrMon = GetCurrentMonitor();
		char str4[1];
		_itoa(nrMon, str4, 10);
		strcat(str3, str4);
		DrawText(str3, 20, 140, 20, BLUE);*/
		DrawText("Escape to quit.", 20, 140, 20, BLUE);

		// DEBUGGING DISPLAYS
		//char str5[4];
		//_itoa(GetMouseX(), str5, 10);
		//DrawText(str5, 20, 200, 20, RED);

		//char str6[4];
		//_itoa(GetMouseY(), str6, 10);
		//DrawText(str6, 80, 200, 20, RED);

		//char str7[16];
		//char str8[16];
		//char str9[16];
		int colorIndex = (posArray[1] * backgroundWidth) + posArray[0];
		Color pixel = colors[colorIndex];
		//_itoa(pixel.r, str7, 10);
		//_itoa(pixel.g, str8, 10);
		//_itoa(pixel.b, str9, 10);

		//DrawText(str7, 20, 240, 20, RED);
		//DrawText(str8, 80, 240, 20, GREEN);
		//DrawText(str9, 140, 240, 20, BLUE);

		// Get right pixel color
		colorIndex = (posArray[1] * backgroundWidth) + posArray[0] + 1;
		pixel = colors[colorIndex];
		if (pixel.r == 0 && pixel.g == 13 && pixel.b == 147) {
			RightForbidden = true;
		}
		else {
			RightForbidden = false;
		}
		// Get left pixel color
		colorIndex = (posArray[1] * backgroundWidth) + posArray[0] - 1;
		pixel = colors[colorIndex];
		if (pixel.r == 0 && pixel.g == 13 && pixel.b == 147) {
			LeftForbidden = true;
		}
		else {
			LeftForbidden = false;
		}
		// Get up pixel color
		colorIndex = ((posArray[1] - 1)* backgroundWidth) + posArray[0];
		pixel = colors[colorIndex];
		if (pixel.r == 0 && pixel.g == 13 && pixel.b == 147) {
			UpForbidden = true;
		}
		else {
			UpForbidden = false;
		}
		// Get down pixel color
		colorIndex = ((posArray[1] + 1) * backgroundWidth) + posArray[0];
		pixel = colors[colorIndex];
		if (pixel.r == 0 && pixel.g == 13 && pixel.b == 147) {
			DownForbidden = true;
		}
		else {
			DownForbidden = false;
		}

		// draw avatar
		drawPlayer(posArray[0], posArray[1]);

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
		int cropSize = 16;
		// int cropX1 = max(cropSize, GetMouseX());
		if ((GetMouseX() > worldOriginX + cropSize) && (GetMouseX() < (worldSizeX - cropSize))) {
			cropX1 = GetMouseX() + 32; //+ cropSize;
		}
		int cropX2 = cropX1 + cropSize;
		if ((GetMouseY() > worldOriginY + cropSize) && (GetMouseY() < (worldSizeY - cropSize))) {
			cropY1 = GetMouseY() + 32; // +cropSize;
		}
		// int cropY1 = min(worldSizeY - cropSize, GetMouseY());
		int cropY2 = cropY1 + cropSize;
		Rectangle cropRect = { cropX1, cropY1, cropSize, cropSize };
		
		//ImageCrop(& toEnlarge, cropRect);
		//ImageAlphaPremultiply(&toEnlarge);
		//ImageAlphaMask(&toEnlarge, alphaMask);
		//Texture2D newTexture = LoadTextureFromImage(toEnlarge);
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

		Rectangle blonkRect = { GetMouseX() + 16, GetMouseY() + 16, 64, 64};
		// Origin of the texture (rotation/scale point), it's relative to destination rectangle size
		//Vector2 origin = { (float)frameWidth, (float)frameHeight };
		Vector2 origin = { (0, 0) };
		int rotation = 0;
		//DrawTexturePro(Texture2D texture, Rectangle source, Rectangle dest, Vector2 origin, float rotation, Color tint); 
		// text, source, dest, origin, rotation, color
		
		// zoomies
		//BeginBlendMode(0);                              // Begin blending mode (alpha, additive, multiplied, subtract, custom)
		//SetTextureFilter(newTexture, 4);
		DrawTexturePro(newTexture, cropRect, blonkRect, origin, (float)rotation, WHITE);
		//EndBlendMode(0);                                    // End blending mode (reset to default: alpha blending)
		
		// here upscaling works:
		//DrawTextureEx(newTexture, position, 0, 4, WHITE);  // Draw a Texture2D with extended parameters
		
		// if player is in glasse area, draw BIG smol player from spritesheet
		// CheckCollisionSpheres(Vector3 center1, float radius1, Vector3 center2, float radius2);
		// vector3 center1
		
		if (CheckCollisionCircles((Vector2) { posArray[0], posArray[1] }, 16, goalCenter, 24)) {
			win = true;
			displayWin();
			int spacing = 100;
			DrawRectangle(worldOriginX + spacing, worldOriginY + spacing, worldSizeX - spacing * 2, worldSizeY - spacing * 2, DARKPURPLE);
			DrawTextEx(fontMono, "YOU ARE HAPPY! YOU HAVE WON!", (Vector2) { worldOriginX + spacing * 2, worldOriginY + spacing * 3 }, 64, 12, GOLD);
			DrawTextEx(fontMono, "CONGRATULATIONS!!!!", (Vector2) { worldOriginX + spacing * 2.01, worldOriginY + spacing * 3.01 + spacing / 2 }, 64, 12, BLACK);
			DrawTextEx(fontMono, "CONGRATULATIONS!!!!", (Vector2) { worldOriginX + spacing * 2, worldOriginY + spacing * 3 + spacing / 2 }, 65, 12, GOLD);
			DrawText("PRESS ESCAPE TO QUIT.", 120, 600, 18, DARKPURPLE);
			StopMusicStream(music);
			PlayMusicStream(musicWin);
			//EndDrawing();
			//waitTime(3);
			//cleanup();	// not yet really cleaning up!
			//OpenURL(const char* url)
			//return 0;
		}
		else {
			win = false;
		}

		if (!(GetMouseX() < (worldSizeX - 32) && GetMouseY() < (worldSizeY - 32))) {
			return;
		} else {
			if (CheckCollisionCircles((Vector2) { posArray[0], posArray[1] }, 16, (Vector2) { GetMouseX() + 56, GetMouseY() + 56 }, 16)) {
				playerRec.x = playerSpriteIndexX * (float)playerSheet.width / playerSpritesTotalX;
				playerRec.y = playerSpriteIndexY * (float)playerSheet.height / playerSpritesTotalY;
				//playerRec.x = 0;
		
				DrawTextureRec(playerSheet, playerRec, (Vector2) { posArray[0] - 16, posArray[1] - 16 }, WHITE);
				//DrawTexture(playerSheet, posArray[0], posArray[1], WHITE);
			}
		}
		//DrawTextureRec(newTexture, rect3, position, SKYBLUE);
		// Vector2 origin = (Vector2){ 0, 0 };
		//Vector2 origin = (Vector2){ 0, 0 };
		// Rectangle rectSrc = { 0, 0, 32, 32 };
		// Rectangle rectDst = { 0, 0, 64, 64 };
		// DrawTexturePro(newTexture, rectSrc, rectDst, origin, 0, WHITE); // Draw a part of a texture defined by a rectangle with 'pro' parameters
		
		if (GetMouseX() < (worldSizeX - 32) && GetMouseY() < (worldSizeY - 32)) {
			DrawTexture(glass, GetMouseX(), GetMouseY(), WHITE);
		}

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
	UnloadTexture(glass);
	UnloadTexture(playerSheet);
	UnloadImage(toEnlarge);
	//UnloadTexture(newTexture);
	//UnloadImage(alphaMask);
	UnloadFont(fontMono);
	void UnloadImageColors(Color* colors);
	UnloadMusicStream(music);   // Unload music stream buffers from RAM
	UnloadMusicStream(musicWin);
	UnloadSound(snd_beep1);
	CloseAudioDevice();         // Close audio device (music streaming is automatically stopped)
	CloseWindow();
	return 0;
}

//void cleanup() {
//
//}

// function definitions
bool isColorForbidden(int x, int y, Color* colors) {
	//Image background2 = LoadImage("bg_01.png");
	//Color* colors = LoadImageColors(background2);
	int colorIndex = (y * worldSizeX) + x;
	//Color pixel = colors[colorIndex];
	//UnloadImage(background2);

	//if (pixel.r == 0 && pixel.g == 13 && pixel.b == 147) {
	//	return true;
	//}
	//else {
	//	return false;
	//}
	//_itoa(pixel.r, str7, 10);
	//_itoa(pixel.g, str8, 10);
	//_itoa(pixel.b, str9, 10);
}

void displayWin() {

}
