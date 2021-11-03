#ifndef BUILDING_H_INCLUDED
#define BUILDING_H_INCLUDED

#include <string.h>
#include <math.h>

#include "map.h"
#include "model.h"
#include "utility.h"
#include "material.h"
#include "road.h"
#include "vehicle.h"

//Épület típusok
typedef enum { nothing, warehouse, processing_plant, factory, office, last } building_category;
static const char* const building_category_table[50] = { "NOTHING", "WAREHOUSE", "PROCESSING_PLANT", "FACTORY", "OFFICE", NULL };
static char* building_category_names_UPPER_CASE[] = { "NOTHING", "WAREHOUSE", "PROCESSING_PLANT", "FACTORY", "OFFICE" };
static building_category building_category_list[] = { nothing, warehouse, processing_plant, factory, office };

typedef struct Building Building;
typedef struct Building
{
    // Épület pozíciója, mérete, iránya
    vec2i pos;
    vec2i size;
    direction facing_direction;
    vec2i entry_point;

    // Épület kategóriája, modellje
    building_category category;
    struct Model building_model;

    // Épület neve,
    char name[50];
    int exists;

    // Termelés
    int storage_capacity;
    Material* storage[100];
    //Material** storage;
    Material* produces;
    Material* order_list[100];
    long order_cooldown;
    long delivery_cooldown;

    Building* source_from;
    Building* source_from2;
    Building* deliver_to;
}Building;

Building building_types[50];

#define building_limit 5000
Building buildings[building_limit];


/*
======================================================================================
    Épület kezelõ függvények
*/
// Épület létrehozása
void Building_Types_From_File(char* filename);
//void Make_Building_Type(Building* building_type, char name[50], struct Model building_model, building_category category, int size_x, int size_y);
void Make_Building_Type(Building* building_type, char name[50], char model_file_name[50], char texture_name[50], building_category category, int storage_size, int size_x, int size_y);
// Épület elhelyezés
void Place_Building_OLD(struct Model building_model, building_category category, int storage_capacity, char name[50], int x, int y, int size_x, int size_y, direction direction);
void Place_Building_By_Name(char building_name[], int x, int y, direction direction, Building building_types[]);
// Épület kirajzolása
void Draw_Building(Building building);
// Épület lebontása
void Bulldoze_Building_OLD(struct Virtual_Cursor v_cursor);

building_category Building_Type_Enum(char* sval);
char* Building_Type_String(building_category category);

/*
======================================================================================
    Épület szimuláció függvények
*/
void Building_Produce(Building* building);
bool Building_Has_Orders(Building* building);
Material* Get_Order(Building* building);
void Clear_Order_List(Building* building);
Material* Get_Storage_Space(Building* building);
Building* Get_Building_From_Entry_Point(int x, int y);
void Print_Building_Storage(Building* building);
bool Building_Spawned_Forklift(Building* building);
#endif // BUILDING_H_INCLUDED
