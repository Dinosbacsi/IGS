#include "building.h"

/*
======================================================================================
	Épület kezelõ függvények
*/
void Building_Types_From_File(char* filename)
{
	FILE* bc_file;
	char bc_line[100];

	bc_file = fopen(filename, "r");
	if (!bc_file)
	{
		printf("ERROR: %s file could not be opened!\n", filename);
	}
	else
	{
		int i = 0;
		while (i < sizeof(building_types) / sizeof(Building) && fgets(bc_line, sizeof(bc_line), bc_file) != NULL)
		{
			char building_name[50];
			char model_name[50];
			char texture_name[50];
			char building_category_name[50];
			int building_size_x;
			int building_size_y;
			int storage_size;
			int milkrun_vehicle_limit;

			if (sscanf(bc_line, "%s\t%s\t%s\t%d\t%d\t%s\t%d\t%d", building_name, model_name, texture_name, &building_size_x, &building_size_y, building_category_name, &storage_size, &milkrun_vehicle_limit))
			{
				building_category new_building_type_category = Building_Type_Enum(building_category_name);

				Make_Building_Type(&building_types[i + 1], building_name, model_name, texture_name, new_building_type_category, storage_size, milkrun_vehicle_limit, building_size_x, building_size_y);
			}

			i++;
		}
	}
}

void Make_Building_Type(Building* building_type, char name[50], char model_file_name[50], char texture_name[50], building_category category, int storage_size, int milkrun_vehicle_limit, int size_x, int size_y)
{
	GLuint building_texture = SOIL_load_OGL_texture
	(
		texture_name,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB
	);
	Load_Model(model_file_name, &building_type->building_model, building_texture);

	sprintf(building_type->name, name);
	building_type->category = category;
	building_type->storage_capacity = storage_size;
	building_type->milkrun_vehicle_limit = milkrun_vehicle_limit;
	building_type->size.x = size_x;
	building_type->size.y = size_y;

	printf("Building type created! \n");
}

void Place_Building(Building* new_building, bool place_entry)
{
	int i = 0;

	// Első üres rekesz keresése az épületek listájában
	while (buildings[i].exists != 0 && i <= building_limit)
	{
		i++;
	}
	// Ha van üres hely az épületek listájában
	if (i <= building_limit)
	{
		// Épület elhelyezése
		buildings[i].exists = 1;
		buildings[i].building_model = new_building->building_model;
		buildings[i].category = new_building->category;

		char building_index[10] = "";
		sprintf(building_index, "_%d", i);
		sprintf(buildings[i].name, strcat(new_building->name, building_index));

		buildings[i].pos.x = new_building->pos.x;
		buildings[i].size.x = new_building->size.x;
		buildings[i].pos.y = new_building->pos.y;
		buildings[i].size.y = new_building->size.y;
		buildings[i].facing_direction = new_building->facing_direction;

		buildings[i].storage_capacity = new_building->storage_capacity;
		buildings[i].milkrun_vehicle_limit = new_building->milkrun_vehicle_limit;

		// Tárhely lista nullázása
		for (int j = 0; j < new_building->storage_capacity; j++)
		{
			buildings[i].storage[j] = NULL;
		}
		// Rendelési lista NULL alapérték
		for (int j = 0; j < sizeof(buildings[i].order_list) / sizeof(Material*); j++)
		{
			buildings[i].order_list[j] = NULL;
		}

		// Tile-ok lefoglalása
		if (new_building->facing_direction == north || new_building->facing_direction == south)
		{
			int lx = (int)((float)new_building->pos.x - ((float)new_building->size.x) / 2);
			int ly = (int)((float)new_building->pos.y - ((float)new_building->size.y) / 2);

			for (int ix = 1; ix <= new_building->size.x; ix++)
			{
				for (int iy = 1; iy <= new_building->size.y; iy++)
				{
					tiles[lx + ix][ly + iy].occupied_by_building = &buildings[i];
				}
			}

			// Épület belépőpontjának elhelyezése
			if (place_entry)
			{
				if (new_building->facing_direction == north)
				{
					buildings[i].entry_point.x = (int)(roundf((float)(new_building->pos.x + new_building->size.x / 2)) + 1);
					buildings[i].entry_point.y = new_building->pos.y;
				}
				else if (new_building->facing_direction == south)
				{
					buildings[i].entry_point.x = (int)(roundf((float)(new_building->pos.x - new_building->size.x / 2)) - 1);
					if (new_building->size.x % 2 == 0)
					{
						buildings[i].entry_point.x++;
					}

					buildings[i].entry_point.y = new_building->pos.y;
				}
			}
			
		}
		else if (new_building->facing_direction == east || new_building->facing_direction == west)
		{
			int lx = (int)((float)new_building->pos.x - ((float)new_building->size.y) / 2);
			int ly = (int)((float)new_building->pos.y - ((float)new_building->size.x) / 2);

			for (int ix = 1; ix <= new_building->size.y; ix++)
			{
				for (int iy = 1; iy <= new_building->size.x; iy++)
				{
					tiles[lx + ix][ly + iy].occupied_by_building = &buildings[i];
				}
			}

			// Épület belépőpontjának elhelyezése
			if (place_entry)
			{
				if (new_building->facing_direction == east)
				{
					buildings[i].entry_point.x = new_building->pos.x;
					buildings[i].entry_point.y = (int)roundf(new_building->pos.y - new_building->size.x / 2) - 1;
					if (new_building->size.x % 2 == 0)
					{
						buildings[i].entry_point.y++;
					}
				}
				else if (new_building->facing_direction == west)
				{
					buildings[i].entry_point.x = new_building->pos.x;
					buildings[i].entry_point.y = (int)roundf(new_building->pos.y + new_building->size.x / 2) + 1;
				}
			}
		}

		if (Check_Tile(buildings[i].entry_point.x, buildings[i].entry_point.y) == 3)
		{
			Road_Segment* road_segment_to_split = tiles[buildings[i].entry_point.x][buildings[i].entry_point.y].occupied_by_road_segment;
			Split_Road_Segment(road_segment_to_split, road_segments, road_nodes, buildings[i].entry_point.x, buildings[i].entry_point.y);
		}
	}
}

void Place_Building_OLD(struct Model building_model, building_category category, int storage_capacity, char name[50], int x, int y, int size_x, int size_y, direction direction)
{
	int i = 0;

	// Első üres rekesz keresése az épületek listájában
	while (buildings[i].exists != 0 && i <= building_limit)
	{
		i++;
	}
	// Ha van üres hely az épületek listájában
	if (i <= building_limit)
	{
		// Épület elhelyezése
		buildings[i].exists = 1;
		buildings[i].building_model = building_model;
		buildings[i].category = category;

		char building_index[10] = "";
		sprintf(building_index, "_%d", i);
		sprintf(buildings[i].name, strcat(name, building_index));

		buildings[i].pos.x = x;
		buildings[i].size.x = size_x;
		buildings[i].pos.y = y;
		buildings[i].size.y = size_y;
		buildings[i].facing_direction = direction;

		buildings[i].storage_capacity = storage_capacity;

		// Tárhely lista nullázása
		for (int j = 0; j < storage_capacity; j++)
		{
			buildings[i].storage[j] = NULL;
		}
		// Rendelési lista NULL alapérték
		for (int j = 0; j < sizeof(buildings[i].order_list) / sizeof(Material*); j++)
		{
			buildings[i].order_list[j] = NULL;
		}

		// Tile-ok lefoglalása
		if (direction == north || direction == south)
		{
			int lx = (int)((float)x - ((float)size_x) / 2);
			int ly = (int)((float)y - ((float)size_y) / 2);

			for (int ix = 1; ix <= size_x; ix++)
			{
				for (int iy = 1; iy <= size_y; iy++)
				{
					tiles[lx + ix][ly + iy].occupied_by_building = &buildings[i];
				}
			}

			// Épület belépőpontjának elhelyezése
			if (direction == north)
			{
				buildings[i].entry_point.x = (int)(roundf((float)(x + size_x / 2)) + 1);
				buildings[i].entry_point.y = y;
			}
			else if (direction == south)
			{
				buildings[i].entry_point.x = (int)(roundf((float)(x - size_x / 2)) - 1);
				if (size_x % 2 == 0)
				{
					buildings[i].entry_point.x++;
				}

				buildings[i].entry_point.y = y;
			}
		}
		else if (direction == east || direction == west)
		{
			int lx = (int)((float)x - ((float)size_y) / 2);
			int ly = (int)((float)y - ((float)size_x) / 2);

			for (int ix = 1; ix <= size_y; ix++)
			{
				for (int iy = 1; iy <= size_x; iy++)
				{
					tiles[lx + ix][ly + iy].occupied_by_building = &buildings[i];
				}
			}

			// Épület belépőpontjának elhelyezése
			if (direction == east)
			{
				buildings[i].entry_point.x = x;
				buildings[i].entry_point.y = (int)roundf(y - size_x / 2) - 1;
				if (size_x % 2 == 0)
				{
					buildings[i].entry_point.y++;
				}
			}
			else if (direction == west)
			{
				buildings[i].entry_point.x = x;
				buildings[i].entry_point.y = (int)roundf(y + size_x / 2) + 1;
			}
		}

		if (Check_Tile(buildings[i].entry_point.x, buildings[i].entry_point.y) == 3)
		{
			Road_Segment* road_segment_to_split = tiles[buildings[i].entry_point.x][buildings[i].entry_point.y].occupied_by_road_segment;
			Split_Road_Segment(road_segment_to_split, road_segments, road_nodes, buildings[i].entry_point.x, buildings[i].entry_point.y);
		}
	}
}

void Place_Building_By_Name(char building_name[], int x, int y, direction direction, Building building_types[])
{
	for (int i = 0; i < 50; i++)
	{
		if (strcmp(building_types[i].name, building_name) == 0)
		{
			Place_Building_OLD(building_types[i].building_model, building_types[i].category, building_types[i].storage_capacity, building_name, x, y, building_types[i].size.x, building_types[i].size.y, direction);
			printf("\nBuilding placed!");
		}
	}
}

void Draw_Building(Building building)
{
	glPushMatrix();
	// Épület pozícionálása
	if (fmodf((float)building.size.x, 2.0) == 0)
	{
		//glTranslatef(building.pos.x + 0.25 * building.size.x, 0, 0);
		glTranslatef((float)building.pos.x + 0.5f, 0, 0);
	}
	else
	{
		glTranslatef((float)building.pos.x, 0, 0);
	}
	if (fmodf((float)building.size.y, 2.0) == 0)
	{
		//glTranslatef(0, building.pos.y + 0.25 * building.size.y, 0);
		glTranslatef(0.0f, (float)building.pos.y + 0.5f, 0.0f);
	}
	else
	{
		glTranslatef(0.0f, (float)building.pos.y, 0.0f);
	}
	switch (building.facing_direction)
	{
	case north:
		break;
	case east:
		glRotatef(-90, 0, 0, 1);
		break;
	case south:
		glRotatef(-180, 0, 0, 1);
		break;
	case west:
		glRotatef(-270, 0, 0, 1);
		break;
	}

	// Épület kirajzolása
	Draw_Model(&building.building_model);
	glPopMatrix();
}

void Bulldoze_Building_OLD(struct Virtual_Cursor v_cursor)
{
	int x = (int)roundf(v_cursor.pos.x);
	int y = (int)roundf(v_cursor.pos.y);
	Building* bulldozed_building = tiles[x][y].occupied_by_building;

	if (bulldozed_building != NULL)
	{
		// Buidling info delete
		bulldozed_building->milkrun_vehicle_limit = 0;

		// Tile-ok felszabadítása
		for (int w = 0; w < map_width; w++)
		{
			for (int l = 0; l < map_length; l++)
			{
				if (tiles[w][l].occupied_by_building == bulldozed_building)
					tiles[w][l].occupied_by_building = NULL;
			}
		}

		// Épület változók nullázása
		bulldozed_building->source_from = NULL;
		bulldozed_building->source_from2 = NULL;
		bulldozed_building->deliver_to = NULL;

		// Épület lebontása
		//free(bulldozed_building->storage);
		bulldozed_building->exists = 0;
		bulldozed_building->category = none;
	}
}

building_category Building_Type_Enum(char* sval)
{
	building_category result = nothing;
	for (int i = 0; building_category_table[i] != NULL; ++i, ++result)
		if (0 == strcmp(sval, building_category_table[i])) return result;
	return -1;
}

char* Building_Type_String(building_category category)
{
	char result[50] = "";
	for (int i = 0; building_category_table[i] != NULL; ++i)
	{
		if (building_category_list[i] == category)
		{
			sprintf(result, building_category_table[i]);
		}
	}

	return result;
}

/*
======================================================================================
	Épület szimuláció függvények
*/
void Building_Produce(Building* building)
{
	printf("\n\n========== Building produce / order ==========");

	// Készlet ellenőrzése
	int requirement1_i;
	int requirement2_i;
	bool requirement1_fulfilled = false;
	bool requirement2_fulfilled = false;
	for (int i = 0; i < building->storage_capacity; i++)
	{
		if (building->produces->requirement1 != NULL && building->storage[i] == building->produces->requirement1)
		{
			requirement1_fulfilled = true;
			requirement1_i = i;
		}

		if (building->produces->requirement2 != NULL && building->storage[i] == building->produces->requirement2)
		{
			requirement2_fulfilled = true;
			requirement2_i = i;
		}
	}

	if (building->produces->requirement2 == NULL)
	{
		requirement2_fulfilled = true;
	}

	// Ha nincs készlet, rendelés
	if (!requirement1_fulfilled && building->produces->requirement1 != NULL && building->produces->requirement1->category == raw)
	{
		Material** order_list;
		int order_list_size = 0;
		if (building->source_from == NULL)
		{
			order_list = building->order_list;
			order_list_size = (int)roundf((float)building->storage_capacity / 2.0f);
		}
		else
		{
			order_list = building->source_from->order_list;
			order_list_size = (int)roundf((float)building->source_from->storage_capacity / 2.0f);
		}

		for (int i = 0; i < order_list_size; i++)
		{
			if (order_list[i] == NULL)
			{
				order_list[i] = building->produces->requirement1;
				printf("\n%s ordered %s material!", building->name, building->produces->requirement1->name);
				break;
			}
		}
	}
	if (building->produces->requirement2 != NULL && !requirement2_fulfilled && building->produces->requirement2->category == raw)
	{
		Material** order_list;
		int order_list_size = 0;
		if (building->source_from2 == NULL)
		{
			order_list = building->order_list;
			order_list_size = (int)roundf((float)building->storage_capacity / 2.0f);
		}
		else
		{
			order_list = building->source_from2->order_list;
			order_list_size = (int)roundf((float)building->source_from2->storage_capacity / 2.0f);
		}

		for (int i = 0; i < order_list_size; i++)
		{
			if (order_list[i] == NULL)
			{
				order_list[i] = building->produces->requirement2;
				printf("\n%s ordered %s material!", building->name, building->produces->requirement2->name);
				break;
			}
		}
		/*for (int i = 0; i < sizeof(building->order_list) / sizeof(Material*); i++)
		{
			if (building->order_list[i] == NULL)
			{
				building->order_list[i] = building->produces->requirement2;
				printf("\n%s ordered %s material!", building->name, building->produces->requirement2->name);
				break;
			}
		}*/
	}
	// Ha van beállított raktára az épületnek, akkor áthelyezés annak a rendelési listájába
	/*if (building->source_from != NULL)
	{
		for (int i = 0; i < sizeof(building->order_list) / sizeof(Material*); i++)
		{
			for (int j = 0; j < sizeof(building->source_from->order_list) / sizeof(Material*); j++)
			{
				if (building->order_list[i] != NULL && building->source_from->order_list[j] == NULL)
				{
					building->source_from->order_list[j] = building->order_list[i];
					building->order_list[i] = NULL;
					printf("\nOrder swap happened.");
				}
			}
		}
	}*/

	// Ha van készlet, termelés
	if (requirement1_fulfilled && requirement2_fulfilled)
	{
		// Alapanyag felhasználása
		building->storage[requirement1_i] = NULL;
		if (building->produces->requirement2 != NULL)
		{
			building->storage[requirement2_i] = NULL;
		}

		// Új anyag előállítása
		building->storage[requirement1_i] = building->produces;
		building->storage[requirement1_i]->exists = true;
		

		printf("\n%s. produced %s!", building->name, building->produces->name);
	}
}

bool Building_Has_Orders(Building* building)
{
	bool has_orders = false;

	for (int i = 0; i < sizeof(building->order_list) / sizeof(Material*); i++)
	{
		if (building->order_list[i] != NULL)
		{
			has_orders = true;
			return has_orders;
		}
	}

	return has_orders;
}

Material* Get_Order(Building* building)
{
	Material* order = NULL;

	for (int i = 0; i < sizeof(building->order_list) / sizeof(Material*); i++)
	{
		if (building->order_list[i] != NULL)
		{
			order = building->order_list[i];
			building->order_list[i] = NULL;
			return order;
		}
	}

	return order;
}

void Clear_Order_List(Building* building)
{
	for (int i = 0; i < sizeof(building->order_list) / sizeof(Material*); i++)
	{
		building->order_list[i] = NULL;
	}
}

Material* Get_Storage_Space(Building* building)
{
	Material* storage_space = NULL;

	for (int i = 0; i < building->storage_capacity; i++)
	{
		if (building->storage[i] == NULL)
			storage_space = building->storage[i];
	}

	return storage_space;
}

bool Building_Has_Storage(Building* building)
{
	bool has_storage = false;
	
	for (int i = 0; i < building->storage_capacity; i++)
	{
		if (building->storage[i] == NULL)
		{
			has_storage = true;
			break;
		}
	}

	return has_storage;
}

Material* Get_Product(Building* building)
{
	Material* material = NULL;

	for (int i = 0; i < building->storage_capacity && material == NULL; i++)
	{
		if (building->storage[i] != NULL)
			material = building->storage[i];
	}

	return material;
}

Building* Get_Building_From_Entry_Point(int x, int y)
{
	Building* found_building = NULL;

	for (int i = 0; i < sizeof(buildings) / sizeof(Building); i++)
	{
		if (buildings[i].entry_point.x == x && buildings[i].entry_point.y == y)
			found_building = &buildings[i];
	}

	return found_building;
}

void Print_Building_Storage(Building* building)
{
	printf("\nBuilding storage info: \n");
	for (int i = 0; i < building->storage_capacity; i++)
	{
		if (building->storage[i] != NULL)
		{
			printf("Storage #%d: %s \n", i + 1, building->storage[i]->name);
		}
		else
		{
			printf("Storage #%d: [empty] \n", i + 1);
		}
	}
}

bool Building_Spawned_Forklift(Building* building)
{
	bool has_forklift = false;

	for (int i = 0; i < sizeof(vehicles) / sizeof(Vehicle); i++)
	{
		if (vehicles[i].home == building)
		{
			has_forklift = true;
			return has_forklift;
		}
	}

	return has_forklift;
}

int Building_Milkrun_Spawned(Building* building)
{
	int number_of_vehicles = 0;

	for (int i = 0; i < sizeof(vehicles) / sizeof(Vehicle); i++)
	{
		if (vehicles[i].home == building)
		{
			number_of_vehicles++;
		}
	}

	return number_of_vehicles;
}

int Building_Has_Finished_Product(Building* building)
{
	int finished_products = 0;

	for (int i = 0; i < building->storage_capacity; i++)
	{
		if (building->storage[i] != NULL && building->storage[i]->category == finished)
			finished_products++;
	}

	return finished_products;
}

void Building_Log(Building* building)
{
	char building_name[50];
	strcpy(building_name, building->name);

	// Counting order list
	int ordered_materials = 0;
	for (int i = 0; i < sizeof(building->order_list) / sizeof(Material*); i++)
	{
		if (building->order_list[i] != NULL)
			ordered_materials++;
	}
	// Counting materials
	int source_material_1 = 0;
	int source_material_2 = 0;
	int finished_materials = 0;
	int all_materials = 0;
	for (int i = 0; i < building->storage_capacity; i++)
	{
		if (building->storage[i] != NULL)
		{
			if (building->produces != NULL && building->storage[i] == building->produces->requirement1)
			{
				source_material_1++;
			}
			else if (building->produces != NULL && building->storage[i] == building->produces->requirement2)
			{
				source_material_2++;
			}
			else if (building->produces != NULL && building->storage[i] == building->produces)
			{
				finished_materials++;
			}
			
			all_materials++;
		}
	}


	FILE* fp;
	char filename[150];
	strcpy(filename, strcat(building_name, ".txt"));

	bool log_file_empty;
	fp = fopen(building_name, "r");
	if (fp == NULL)
	{
		log_file_empty = true;
	}
	else
	{
		log_file_empty = false;
		fclose(fp);
	}

	// Write into file
	fp = fopen(building_name, "a");
	if (fp != NULL)
	{
		if (log_file_empty)
		{
			char requirement_1_name[50] = "null";
			char requirement_2_name[50] = "null";
			char produces_name[50] = "null";
			if (building->produces != NULL)
			{
				if (building->produces->requirement1 != NULL)
					sprintf(requirement_1_name, building->produces->requirement1->name);
				if (building->produces->requirement2 != NULL)
					sprintf(requirement_2_name, building->produces->requirement2->name);
				sprintf(produces_name, building->produces->name);
			}

			fprintf(fp, "%s\t%s\t%s\t%s\t%s\t%s\n", "Time", "Ordered", requirement_1_name, requirement_2_name, produces_name, "All");
			fprintf(fp, "%d\t%d\t%d\t%d\t%d\t%d\n", glutGet(GLUT_ELAPSED_TIME), ordered_materials, source_material_1, source_material_2, finished_materials, all_materials);
			fclose(fp);
		}
		else
		{
			fprintf(fp, "%d\t%d\t%d\t%d\t%d\t%d\n", glutGet(GLUT_ELAPSED_TIME), ordered_materials, source_material_1, source_material_2, finished_materials, all_materials);
			fclose(fp);
		}
	}
}