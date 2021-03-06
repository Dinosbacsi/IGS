#ifndef VEHICLE_H_INCLUDED
#define VEHICLE_H_INCLUDED

#include "model.h"
#include "utility.h"
#include "map.h"
#include "road.h"
#include "material.h"
#include "building.h"

#define vehicle_limit 100
#define vehicle_turning_treshold 2

typedef enum { going_to_destination, at_destination, leaving_world } vehicle_status;

typedef struct Vehicle
{
    // Jármû pozíciója
    vec2i tile;
    vec3 pos;
    vec3 rotate;
    direction facing;
    direction previous_facing;
    bool turning;
    float speed;
    float max_speed;
    float target_speed;
    float acceleration_rate;

    // Jármű célja
    bool exists;
    Node* destination_node;
    Node* next_node;
    Node* previous_node;
    Node* path_nodes[100];
    Tile* current_tile;
    int current_node_in_path;
    vehicle_status status;

    // Jármû kerekeinek pozíciója
    // sorrend: BE, JE, BH, JH
    vec3 wheel[4];
    float wheel_rotate;
    int wheel_turn;
    bool inverted_steering;

    // Jármû modelljei
    struct Model* vehicle_model;
    struct Model* wheel_model;

    // Anyag kapacitás
    int capacity;
    Material* cargo[10];
    material_state cargo_type;

    Building* home;
}Vehicle;

//Járművek
//Vehicle test_vehicle;
Vehicle vehicle_types[20];
Vehicle vehicles[vehicle_limit];

/*
======================================================================================
    Jármű kezelõ függvények
*/
void Draw_Vehicle(Vehicle* vehicle);
void Draw_Forklift_Box(Vehicle* vehicle, struct Model* box);
int Place_Vehicle(Vehicle vehicles[], Vehicle* vehicle_type, int tile_x, int tile_y, Road_Segment road_segments[], Node road_nodes[map_width][map_length]);
void Delete_Vehicle(Vehicle* vehicle);
void Vehicle_Cruise(Vehicle* vehicle, Node road_nodes[map_width][map_length]);
void Vehicle_Cruise_Choose_Direction(Vehicle* vehicle, Node road_nodes[map_width][map_length]);
void Vehicle_Follow_Path(Vehicle* vehicle);
void Vehicle_Go_North(Vehicle* vehicle, Node road_nodes[map_width][map_length]);
void Vehicle_Go_East(Vehicle* vehicle, Node road_nodes[map_width][map_length]);
void Vehicle_Go_South(Vehicle* vehicle, Node road_nodes[map_width][map_length]);
void Vehicle_Go_West(Vehicle* vehicle, Node road_nodes[map_width][map_length]);
void Animate_Vehicle_Turning(Vehicle* vehicle);
void Vehicle_Turn_Left(Vehicle* vehicle);
void Vehicle_Turn_Right(Vehicle* vehicle);
void Vehicle_Steer_Straight(Vehicle* vehicle);
void Move_Vehicle(Vehicle* vehicle);
void Vehicle_Accelerate(Vehicle* vehicle);
void Vehicle_Decelerate(Vehicle* vehicle);
void Find_Path(Vehicle* vehicle, Node road_nodes[map_width][map_length]);
void Vehicle_Leave_World(Vehicle* vehicle);
void Check_For_Traffic_Ahead(Vehicle* this_vehicle);
bool Vehicle_Is_Empty(Vehicle* vehicle);

void Print_Vehicle_Cargo(Vehicle* vehicle);
#endif // VEHICLE_H_INCLUDED
