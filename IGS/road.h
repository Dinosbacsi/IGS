#ifndef ROAD_H_INCLUDED
#define ROAD_H_INCLUDED

#include "model.h"

#include "utility.h"
#include "map.h"

#define segment_limit 5000

// Node típusok
typedef enum { dead_end, straight, curve, intersection_3_way, intersection_4_way } node_type;

typedef struct Road_Type {
    char name[50];
    struct Model* straight;
    struct Model* curve;
    struct Model* intersection_3_way;
    struct Model* intersection_4_way;
    struct Model* dead_end;
}Road_Type;

typedef struct Node
{
    vec2i pos;
    int number_of_connections;
    node_type type;
    direction facing_direction;
    Road_Type* road_type;

    vec2i connection_north;
    vec2i connection_east;
    vec2i connection_south;
    vec2i connection_west;

    bool exists;
    bool checked;
}Node;

typedef struct Road_Segment
{
    Node* A;
    Node* B;
    int length;
    direction road_direction;
    Road_Type* road_type;

    bool exists;
}Road_Segment;

//Utak
Node road_nodes[map_width][map_length];
Road_Segment road_segments[segment_limit];
Node new_segment_A;
Node new_segment_B;
Road_Segment new_segment;

/*
======================================================================================
    Út kezelõ függvények
*/
void Place_Road_Segment(Road_Segment road_segments[], Node road_nodes[map_width][map_length], Road_Type* road_type, Tile tiles[map_width][map_length], int a_x, int a_y, int b_x, int b_y);
void Split_Road_Segment(Road_Segment* road_to_split, Road_Segment road_segments[], Node road_nodes[map_width][map_length], Tile tiles[map_width][map_length], int x, int y);
void Update_Road_Node(Node* road_node, Tile tiles[map_width][map_length]);
void Draw_Road_Node(Node* node);
//void Draw_Road_Segment(Road_Segment road, struct Model road_model);
void Draw_Road_Segment(Road_Segment road);
void Delete_Road_Segment(Road_Segment* deleted_road_segment, Node road_nodes[map_width][map_length], Tile tiles[map_width][map_length]);
void Delete_Road_Node(int x, int y, Road_Segment road_segments[], Node road_nodes[map_width][map_length], Tile tiles[map_width][map_length]);

#endif // ROAD_H_INCLUDED
