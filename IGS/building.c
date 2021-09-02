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
        printf("Nem lehetett megnyitni a(z) %s fajlt!\n", filename);
    }
    else
    {
        int i = 0;
        while (i < sizeof(building_types) && fgets(bc_line, sizeof(bc_line), bc_file) != NULL)
        {
            char building_name[50];
            char model_name[50];
            char texture_name[50];
            char building_category_name[50];
            int building_size_x;
            int building_size_y;

            if (sscanf(bc_line, "%s\t%s\t%s\t%d\t%d\t%s", building_name, model_name, texture_name, &building_size_x, &building_size_y, building_category_name))
            {
                building_category new_building_type_category = Building_Type_Enum(building_category_name);

                Make_Building_Type(&building_types[i + 1], building_name, model_name, texture_name, new_building_type_category, building_size_x, building_size_y);
            }

            i++;
        }
    }
}
/*
void Make_Building_Type(Building* building_type, char name[50], struct Model building_model, building_category category, int size_x, int size_y)
{
    sprintf(building_type->name, name);
    building_type->building_model = building_model;
    building_type->category = category;
    building_type->size.x = size_x;
    building_type->size.y = size_y;
}
*/
void Make_Building_Type(Building* building_type, char name[50], char model_file_name[50], char texture_name[50], building_category category, int size_x, int size_y)
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
    building_type->size.x = size_x;
    building_type->size.y = size_y;

    printf("Epulet tipus letrehozva! \n");
}

void Place_Building_OLD(struct Model building_model, building_category category, int x, int y, int size_x, int size_y, direction direction, Building buildings[], int building_limit, struct Tile tiles[map_width][map_length])
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

        buildings[i].pos.x = x;
        buildings[i].size.x = size_x;
        buildings[i].pos.y = y;
        buildings[i].size.y = size_y;
        buildings[i].facing_direction = direction;

        // IDEGLENES - ha gyár, akkor termel készterméket
        if (category == factory)
        {
            buildings[i].produces = &material_types[1];
        }
        // Rendelési lista NULL alapérték
        for (int j = 0; j < 10; j++)
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
                buildings[i].entry_point.x = roundf(x + size_x / 2) + 1;
                buildings[i].entry_point.y = y;
            }
            else if (direction == south)
            {
                buildings[i].entry_point.x = roundf(x - size_x / 2) - 1;
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
                buildings[i].entry_point.y = roundf(y - size_x / 2) - 1;
                if (size_x % 2 == 0)
                {
                    buildings[i].entry_point.y++;
                }
            }
            else if (direction == west)
            {
                buildings[i].entry_point.x = x;
                buildings[i].entry_point.y = roundf(y + size_x / 2) + 1;
            }
        }

        if (Check_Tile(buildings[i].entry_point.x, buildings[i].entry_point.y, tiles) == 3)
        {
            Road_Segment* road_segment_to_split = tiles[buildings[i].entry_point.x][buildings[i].entry_point.y].occupied_by_road_segment;
            Split_Road_Segment(road_segment_to_split, road_segments, road_nodes, tiles, buildings[i].entry_point.x, buildings[i].entry_point.y);
        }
    }
}
void Place_Building_By_Name(char building_name[], int x, int y, direction direction, Building building_types[], Building buildings[], int building_limit, struct Tile tiles[map_width][map_length])
{
    for (int i = 0; i < 50; i++)
    {
        if (strcmp(building_types[i].name, building_name) == 0)
        {
            Place_Building_OLD(building_types[i].building_model, building_types[i].category, x, y, building_types[i].size.x, building_types[i].size.y, direction, buildings, building_limit, tiles);
            printf("\nEpulel elhelyezve!");
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
        glTranslatef(building.pos.x + 0.5, 0, 0);
    }
    else
    {
        glTranslatef(building.pos.x, 0, 0);
    }
    if (fmodf((float)building.size.y, 2.0) == 0)
    {
        //glTranslatef(0, building.pos.y + 0.25 * building.size.y, 0);
        glTranslatef(0, building.pos.y + 0.5, 0);
    }
    else
    {
        glTranslatef(0, building.pos.y, 0);
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

void Bulldoze_Building_OLD(struct Virtual_Cursor v_cursor, struct Building buildings[], Tile tiles[map_width][map_length])
{
    int x = roundf(v_cursor.pos.x);
    int y = roundf(v_cursor.pos.y);
    Building* bulldozed_building = tiles[x][y].occupied_by_building;

    // Tile-ok felszabadítása
    for (int w = 0; w < map_width; w++)
    {
        for (int l = 0; l < map_length; l++)
        {
            if (tiles[w][l].occupied_by_building == bulldozed_building)
                tiles[w][l].occupied_by_building = NULL;
        }
    }

    // Épület lebontása
    bulldozed_building->exists = 0;
    bulldozed_building->category = none;
}

building_category Building_Type_Enum(char* sval)
{
    building_category result = nothing;
    int i = 0;
    for (i = 0; building_category_table[i] != NULL; ++i, ++result)
        if (0 == strcmp(sval, building_category_table[i])) return result;
    return -1;
}

/*
======================================================================================
    Épület szimuláció függvények
*/
void Building_Produce(Building* building)
{
    // Készlet ellenőrzése
    int requirement1_i;
    int requirement2_i;
    bool requirement1_fulfilled = false;
    bool requirement2_fulfilled = false;
    for (int i = 0; i < sizeof(building->storage) - 1; i++)
    {
        if (building->storage[i] == building->produces->requirement1)
        {
            requirement1_fulfilled = true;
            requirement1_i = i;
        }
        //else if (building->produces->requirement2->exists && building->storage[i] == building->produces->requirement2)
        else if (building->produces->requirement2 != NULL && building->storage[i] == building->produces->requirement2)
        {
            requirement2_fulfilled = true;
            requirement2_i = i;
        }
    }

    // Ha nincs készlet, rendelés
    if (!requirement1_fulfilled)
    {
        for (int i = 0; i < sizeof(building->order_list); i++)
        {
            if (building->order_list[i] == NULL)
            {
                building->order_list[i] = building->produces->requirement1;
                printf("\n%s. epulet rendelt %s alapanyagot!", building->name, building->produces->requirement1->name);
                break;
            }
        }
    }
    if (building->produces->requirement2 != NULL && !requirement2_fulfilled)
    {
        for (int i = 0; i < sizeof(building->order_list); i++)
        {
            if (building->order_list[i] == NULL)
            {
                building->order_list[i] = building->produces->requirement2;
                printf("\n%s. epulet rendelt %s alapanyagot!", building->name, building->produces->requirement2->name);
                break;
            }
        }
    }

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
        building->storage[requirement1_i]->exists = true;
        building->storage[requirement1_i] = building->produces;
    }
}

Material* Get_Order(Building* building)
{
    // Ha talál rendelést, visszatér azzal
    for (int i = 0; i < sizeof(building->order_list); i++)
    {
        if (building->order_list[i] != NULL)
            return building->order_list[i];
    }

    // Ha nem volt rendelés, akkor visszatér NULL-al
    return NULL;
}
