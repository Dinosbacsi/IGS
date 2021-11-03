#include "material.h"

/*
======================================================================================
	Anyag kezelõ függvények
*/
void Make_Material_Type(Material* material_type, char name[50], material_state state, material_category category, Material* requirement1, Material* requirement2)
{
	sprintf(material_type->name, name);
	material_type->state = state;
	material_type->category = category;
	material_type->requirement1 = requirement1;
	material_type->requirement2 = requirement2;
	material_type->exists = true;

	printf("%s material type created!\n", material_type->name);
}

Material* Transfer_Material(Material* material)
{
	//printf("\n%s. alapanyag athelyezve!", material->name);
	return material;
}

Material* Material_Type_From_Name(char material_name[50])
{
	for (int i = 0; i < sizeof(material_types); i++)
	{
		if (!strcmp(material_types[i].name, material_name))
		{
			return &material_types[i];
		}
	}

	return NULL;
}

material_state Material_State_Enum(char* sval)
{
	material_state result = solid;
	int i = 0;
	for (i = 0; material_state_table[i] != NULL; ++i, ++result)
		if (0 == strcmp(sval, material_state_table[i])) return result;
	return -1;
}

material_category Material_Category_Enum(char* sval) {
	material_category result = raw;
	int i = 0;
	for (i = 0; material_category_table[i] != NULL; ++i, ++result)
		if (0 == strcmp(sval, material_category_table[i])) return result;
	return -1;
}