#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#include <stdbool.h>
#include "utility.h"

typedef struct Building Building;
typedef struct Road_Segment Road_Segment;
typedef struct Node Node;

#define map_width 150
#define map_length 300
#define player_zone_start_x 50
#define player_zone_start_y 100
#define player_zone_width 50
#define player_zone_length 100

#define tree_limit 500

// Tile kijelölő színek
typedef enum { none, red, yellow } highlight;

typedef struct Tile
{
    vec2i pos;
    int occupied;
    Building* occupied_by_building;
    Road_Segment* occupied_by_road_segment;
    Node* occupied_by_node;
    bool highlighted;
}Tile;

typedef struct Scenery_Object
{
    vec3 pos;
    float rotate;
    struct Model* model;
    bool exsists;
    float distance_from_camera;
}Scenery_Object;

void Draw_Skybox(GLuint sky_texture);

/*
======================================================================================
    Tile kezelõ függvények
*/
//Tile-ok
Tile test_tile;
Tile tiles[map_width][map_length];
// Tile kirajzolása
void Draw_Tile(struct Tile tile, GLuint tile_texture);
// Tile állapotának ellenőrzése
int Check_Tile(int x, int y);
// Tile kijelölés kirajzolása
void Draw_Highlight(struct Tile tile);

#endif // MAP_H_INCLUDED
