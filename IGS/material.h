#ifndef MATERIAL_H_INCLUDED
#define MATERIAL_H_INCLUDED

#include "utility.h"

typedef enum { solid, liquid, gas, liquid_gas, all} material_state;
static const char* const material_state_table[] = { "solid", "liquid", "gas", "liquid_gas", "all", NULL};

typedef enum { raw, processed, finished } material_category;
static const char* const material_category_table[] = { "raw", "processed", "finished", NULL };

typedef struct Material Material;
typedef struct Material
{
	// Tulajdonságok
	char name[50];
	material_state state;
	material_category category;
	// Követelmények
	Material* requirement1;
	Material* requirement2;
	// Állapot
	bool exists;
}Material;

Material material_types[50];

/*
======================================================================================
	Anyag kezelő függvények
*/
void Make_Material_Type(Material* material_type, char name[50], material_state state, material_category category, Material* requirement1, Material* requirement2);
Material* Transfer_Material(Material* material);
Material* Material_Type_From_Name(char material_name[50]);
material_state Material_State_Enum(char* sval);
material_category Material_Category_Enum(char* sval);
#endif // MATERIAL_H_INCLUDED