#ifndef BUILDING_H_INCLUDED
#define BUILDING_H_INCLUDED

#include <math.h>

#include "map.h"
#include "model.h"
#include "utility.h"
#include "material.h"
#include "road.h"

//Épület típusok
typedef enum { nothing, warehouse, processing_plant, factory, office, last } building_category;
static const char* const building_category_table[50] = { "NOTHING", "WAREHOUSE", "PROCESSING_PLANT", "FACTORY", "OFFICE", NULL };
static char* building_category_names_UPPER_CASE[] = { "NOTHING", "WAREHOUSE", "PROCESSING_PLANT", "FACTORY", "OFFICE" };
static building_category building_category_list[] = { nothing, warehouse, processing_plant, factory, office };

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
    Material* storage[10];
    Material* produces;
    Material* order_list[10];
    long order_cooldown;
}Building;

Building building_types[50];


/*
======================================================================================
    Épület kezelõ függvények
*/
// Épület létrehozása
void Building_Types_From_File(char* filename);
//void Make_Building_Type(Building* building_type, char name[50], struct Model building_model, building_category category, int size_x, int size_y);
void Make_Building_Type(Building* building_type, char name[50], char model_file_name[50], char texture_name[50], building_category category, int size_x, int size_y);
// Épület elhelyezés
void Place_Building_OLD(struct Model building_model, building_category category, int x, int y, int size_x, int size_y, direction direction, struct Building buildings[], int building_limit);
void Place_Building_By_Name(char building_name[], int x, int y, direction direction, Building building_types[], Building buildings[], int building_limit);
// Épület kirajzolása
void Draw_Building(Building building);
// Épület lebontása
void Bulldoze_Building_OLD(struct Virtual_Cursor v_cursor, Building buildings[]);

building_category Building_Type_Enum(char* sval);

/*
======================================================================================
    Épület szimuláció függvények
*/
void Building_Produce(Building* building);
Material* Get_Order(Building* building);


#endif // BUILDING_H_INCLUDED
