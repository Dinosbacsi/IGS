#ifndef UTILITY_H_INCLUDED
#define UTILITY_H_INCLUDED

#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS

//Általános headerek
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <float.h>
#include <time.h>
#include <windows.h>

    //SDL, OpenGL, SOIL
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_mixer.h>
#include <GL\GLU.h>
#include <GL\freeglut.h>
#include <SOIL.h>

#define PI 3.14159

//#define max(a,b) \
//   ({ __typeof__ (a) _a = (a); \
//       __typeof__ (b) _b = (b); \
//     _a > _b ? _a : _b; })
//
//#define min(a,b) \
//   ({ __typeof__ (a) _a = (a); \
//       __typeof__ (b) _b = (b); \
//     _a < _b ? _a : _b; })

//Irányváltozók (kamera mozgásához)
enum cam_direction { up, down, left, right };
// Irány változók
typedef enum { north, south, east, west } direction;

// Pozíció struktúrák
typedef struct vec3
{
    float x, y, z;
} vec3;
typedef struct vec2
{
    float x, y;
} vec2;
// Pozíció struktúrák (integerrel)
typedef struct vec3i
{
    int x, y, z;
} vec3i;
typedef struct vec2i
{
    int x, y;
} vec2i;

//
struct Cursor
{
    vec2i pos;
    vec2i last;
    vec2i dif;
};

typedef struct Virtual_Cursor
{
    vec3 pos;
}Virtual_Cursor;

struct Camera
{
    vec3 pos;
    float angle_h;
    float angle_v;
    float distance;
    float speed;
    float sensitivity;
    int field_of_view;
    vec3 POV;
};

#include "map.h"

/*
======================================================================================
    Kamera
*/
// Kamera pozícionálása
void Position_Camera(struct Camera* cam);
// Kamera mozgatása adott helyre
void Move_Camera_To(struct Camera* cam, float cam_pos_x, float cam_pos_y);
// Kamera mozgatás nézethez relatívan
void Move_Camera_Relative(struct Camera* cam, enum cam_direction direction);
// Kamera forgatása egérrel
void Rotate_Camera(struct Camera* cam, struct Cursor* cursor, SDL_Window* gWindow, int SCREEN_WIDTH, int SCREEN_HEIGHT);

/*
======================================================================================
    3D kurzor
*/
// 3D-s kurzor kirajzolása
void Draw_3D_Cursor(struct Virtual_Cursor v_cursor);
// 3D kurzor helyének meghatározása
void Calculate3DCursorLocation(int cursorX, int cursorY, Virtual_Cursor* v_cursor);
// 3D kurzor játékos területen tartása
void Restrict_3D_Cursor_To_Player_Area(Virtual_Cursor* v_cursor);

/*
======================================================================================
    Négyzetrács
*/
// Teljes négyzetrács kirajzolása
void Draw_Full_Grid(int w, int l);
// Kurzor közelében négyzetrács kirajzolása
void Draw_Local_Grid(struct Virtual_Cursor v_cursor);

/*
======================================================================================
    HUD
*/
void Render_Bitmap_String(int x, int y, int z, void* font, char* string, float r, float g, float b);
void Render_Bitmap_String_With_Backdrop(int x, int y, int z, void* font, char* string, float r, float g, float b, float br, float bg, float bb);

/*
======================================================================================
    Random matek
*/
float Distance(float x1, float x2, float y1, float y2);
int randInRange(int min, int max);
#endif // UTILITY_H_INCLUDED
