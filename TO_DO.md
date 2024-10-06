# TO DO

only allow glass to stay in bounds

~~display sprite underneath glass if playerPos in glassArea~~

display SUCCESS msg when goal area is colliding with player -> exit game

 bool CheckCollisionSpheres(Vector3 center1, float radius1, Vector3 center2, float radius2);   // Check collision between two spheres
 // Draw a part of a texture defined by a rectangle
 DrawTextureRec(Texture2D texture, Rectangle source, Vector2 position, Color tint);   
 // or Draw a part of a texture defined by a rectangle with 'pro' parameters
DrawTexturePro(Texture2D texture, Rectangle source, Rectangle dest, Vector2 origin, float rotation, Color tint); 

add splaysh screen(s)
add zoom mask
add timer

Add license

some sort of magnification rendering inside the glass
display resolution. limit play area?
use rayGUI for menu? change font. fade out text
how to compile resources into .exe?

## try
Color GetImageColor(Image image, int x, int y);    // Get image pixel color at (x, y) position


## Add game state
0: initialising
1: play
2: paused
3: game event, e.g. dialogue, cut scenes
9: quitting

## Gameplay

control dot with WASD
control magnifying glass with mouse
go through maze and avoid enemies
defeat enemies using sun beams?

a guy who won't let you pass, until you give him your money

switch tool with mouse wheel (or num keys)
tools: magnifying glass, pat, money

options: left-handed mouse

### Add Game instructions
how to play
how to control
how to end the game

### Add browser link
void OpenURL(const char *url);  

## Assets

### Images

Lense
Creatures
Backgrounds
Tiles

Logo_bumpers: Treeation, UDSOUDS

### Sounds
Logo_bumpers

### Music
Title 

note to self: 
song_01.wav		"2010 35zhugo_4.wav"
song_02.wav		"2010 36undichnichnochselber_2.wav"


#### IF I HAVE TIME IN THE END

Fisheye shader from https://www.raylib.com/examples/shaders/loader.html?name=shaders_postprocessing
add menu / title screen
add logo bumpers (w/ sfx)
add other sfx
add options