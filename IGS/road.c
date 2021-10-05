#include "road.h"

/*
======================================================================================
    Út kezelõ függvények
*/
void Place_Road_Segment(Road_Segment road_segments[], Node road_nodes[map_width][map_length], Road_Type* road_type, int a_x, int a_y, int b_x, int b_y)
{
    // Debug info kiíratás
    printf("\n========== Ut letrehozasa ==========\n");

    //Road_Segment new_road;
    int new_road_index = 0;
    while (new_road_index <= segment_limit && road_segments[new_road_index].exists == true)
        new_road_index++;
    if (new_road_index > segment_limit)
    {
        // RETURN HIBA VAGY VALAMI
    }

    // Megnézni, hogy A node helyén már létezik-e másik
    // Ha nem létezik, akkor létrehozás
    if (road_nodes[a_x][a_y].exists == false)
    {
        road_nodes[a_x][a_y].exists = true;
        road_nodes[a_x][a_y].pos.x = a_x;
        road_nodes[a_x][a_y].pos.y = a_y;
        road_nodes[a_x][a_y].road_type = road_type;

        printf("A node letrehozva az x = %d, y = %d helyen!\n", road_nodes[a_x][a_y].pos.x, road_nodes[a_x][a_y].pos.y);
    }

    // Megnézni, hogy B node helyén már létezik-e másik
    // Ha nem létezik, akkor létrehozás
    if (road_nodes[b_x][b_y].exists == false)
    {
        road_nodes[b_x][b_y].exists = true;
        road_nodes[b_x][b_y].pos.x = b_x;
        road_nodes[b_x][b_y].pos.y = b_y;
        road_nodes[b_x][b_y].road_type = road_type;

        printf("B node letrehozva az x = %d, y = %d helyen!\n", road_nodes[b_x][b_y].pos.x, road_nodes[b_x][b_y].pos.y);
    }

    // Node-ok közötti csatlakozás kialakítása
    if (a_x == b_x)
    {
        if (a_y > b_y)
        {
            road_nodes[a_x][a_y].connection_north.x = b_x;
            road_nodes[a_x][a_y].connection_north.y = b_y;

            road_nodes[b_x][b_y].connection_south.x = a_x;
            road_nodes[b_x][b_y].connection_south.y = a_y;
        }
        else if (b_y > a_y)
        {
            road_nodes[a_x][a_y].connection_south.x = b_x;
            road_nodes[a_x][a_y].connection_south.y = b_y;

            road_nodes[b_x][b_y].connection_north.x = a_x;
            road_nodes[b_x][b_y].connection_north.y = a_y;
        }
    }
    else if (a_y == b_y)
    {
        if (a_x > b_x)
        {
            road_nodes[a_x][a_y].connection_west.x = b_x;
            road_nodes[a_x][a_y].connection_west.y = b_y;

            road_nodes[b_x][b_y].connection_east.x = a_x;
            road_nodes[b_x][b_y].connection_east.y = a_y;
        }
        else if (b_x > a_x)
        {
            road_nodes[a_x][a_y].connection_east.x = b_x;
            road_nodes[a_x][a_y].connection_east.y = b_y;

            road_nodes[b_x][b_y].connection_west.x = a_x;
            road_nodes[b_x][b_y].connection_west.y = a_y;
        }
    }

    // Node-ok típusának frissítése
    Update_Road_Node(&road_nodes[a_x][a_y]);
    Update_Road_Node(&road_nodes[b_x][b_y]);

    // Útszakasz létrehozása, node-ok hozzárendelése
    road_segments[new_road_index].exists = true;
    road_segments[new_road_index].A = &road_nodes[a_x][a_y];
    road_segments[new_road_index].B = &road_nodes[b_x][b_y];
    road_segments[new_road_index].road_type = road_type;
    printf("Ut letrehozva az A(%d; %d), B(%d; %d) helyen!\n", road_segments[new_road_index].A->pos.x, road_segments[new_road_index].A->pos.y, road_segments[new_road_index].B->pos.x, road_segments[new_road_index].B->pos.y);

    // Út  hosszának meghatározása
    if (a_x == b_x)
    {
        if (a_y > b_y)
            road_segments[new_road_index].length = a_y - b_y;
        else
            road_segments[new_road_index].length = b_y - a_y;
    }
    else if (a_y == b_y)
    {
        if (a_x > b_x)
            road_segments[new_road_index].length = a_x - b_x;
        else
            road_segments[new_road_index].length = b_x - a_x;
    }
    printf("Ut hossza kiszamolva: %d0m hosszu.\n", road_segments[new_road_index].length);

    // Ha az új Útszakasz bármelyik Node-ja másik útszakaszban van benne, akkor annak az útszakasznak a kettéosztása
    if (tiles[road_segments[new_road_index].A->pos.x][road_segments[new_road_index].A->pos.y].occupied_by_road_segment != NULL)
    {
        Split_Road_Segment(tiles[road_segments[new_road_index].A->pos.x][road_segments[new_road_index].A->pos.y].occupied_by_road_segment, road_segments, road_nodes, road_segments[new_road_index].A->pos.x, road_segments[new_road_index].A->pos.y);
    }
    if (tiles[road_segments[new_road_index].B->pos.x][road_segments[new_road_index].B->pos.y].occupied_by_road_segment != NULL)
    {
        Split_Road_Segment(tiles[road_segments[new_road_index].B->pos.x][road_segments[new_road_index].B->pos.y].occupied_by_road_segment, road_segments, road_nodes, road_segments[new_road_index].B->pos.x, road_segments[new_road_index].B->pos.y);
    }

    // Tile-ok lefoglalása
    if (a_x == b_x)
    {
        if (a_y < b_y)
        {
            for (int i_y = a_y + 1; i_y < b_y; i_y++)
            {
                tiles[a_x][i_y].occupied_by_road_segment = &road_segments[new_road_index];
            }
        }
        else
        {
            for (int i_y = b_y + 1; i_y < a_y; i_y++)
            {
                tiles[a_x][i_y].occupied_by_road_segment = &road_segments[new_road_index];
            }
        }
    }
    else if (a_y == b_y)
    {
        if (a_x < b_x)
        {
            for (int i_x = a_x + 1; i_x < b_x; i_x++)
            {
                tiles[i_x][a_y].occupied_by_road_segment = &road_segments[new_road_index];
            }
        }
        else
        {
            for (int i_x = b_x + 1; i_x < a_x; i_x++)
            {
                tiles[i_x][a_y].occupied_by_road_segment = &road_segments[new_road_index];
            }
        }
    }
}
void Update_Road_Node(Node* node)
{
    node->number_of_connections = 0;
    bool connection_to_east = false;
    bool connection_to_north = false;
    bool connection_to_south = false;
    bool connection_to_west = false;

    if (node->connection_east.x != 0 || node->connection_east.y != 0)
    {
        node->number_of_connections++;
        connection_to_east = true;
    }
    if (node->connection_north.x != 0 || node->connection_north.y != 0)
    {
        node->number_of_connections++;
        connection_to_north = true;
    }
    if (node->connection_south.x != 0 || node->connection_south.y != 0)
    {
        node->number_of_connections++;
        connection_to_south = true;
    }
    if (node->connection_west.x != 0 || node->connection_west.y != 0)
    {
        node->number_of_connections++;
        connection_to_west = true;
    }

    switch (node->number_of_connections)
    {
    case 0:
    {
        node->exists = false;
        break;
    }
    case 1:
    {
        if (connection_to_east)
            node->facing_direction = east;
        else if (connection_to_north)
            node->facing_direction = north;
        else if (connection_to_south)
            node->facing_direction = south;
        else if (connection_to_west)
            node->facing_direction = west;

        node->type = dead_end;
        break;
    }
    case 2:
    {
        if (connection_to_north && connection_to_east)
            node->facing_direction = north;
        else if (connection_to_north && connection_to_west)
            node->facing_direction = west;
        else if (connection_to_south && connection_to_east)
            node->facing_direction = east;
        else if (connection_to_south && connection_to_west)
            node->facing_direction = south;

        node->type = curve;

        if (connection_to_north && connection_to_south)
        {
            node->facing_direction = north;
            node->type = straight;
        }
        else if (connection_to_east && connection_to_west)
        {
            node->facing_direction = east;
            node->type = straight;
        }
        break;
    }
    case 3:
    {
        if (!connection_to_east)
            node->facing_direction = west;
        else if (!connection_to_north)
            node->facing_direction = south;
        else if (!connection_to_south)
            node->facing_direction = north;
        else if (!connection_to_west)
            node->facing_direction = east;

        node->type = intersection_3_way;
        break;
    }
    case 4:
    {
        node->facing_direction = north;
        node->type = intersection_4_way;
        break;
    }
    }

    if (node->exists)
    {
        tiles[node->pos.x][node->pos.y].occupied_by_node = node;
    }
    else
    {
        tiles[node->pos.x][node->pos.y].occupied_by_node = NULL;
    }
}

void Draw_Road_Node(Node* node)
{
    glPushMatrix();
    glTranslatef(node->pos.x, node->pos.y, 0);
    switch (node->facing_direction)
    {
    case north:
        glRotatef(-90, 0, 0, 1);
        break;
    case east:
        glRotatef(0, 0, 0, 1);
        break;
    case south:
        glRotatef(90, 0, 0, 1);
        break;
    case west:
        glRotatef(180, 0, 0, 1);
        break;
    }
    switch (node->type)
    {
    case dead_end:
        Draw_Model(node->road_type->dead_end);
        break;
    case straight:
        Draw_Model(node->road_type->straight);
        break;
    case curve:
        Draw_Model(node->road_type->curve);
        break;
    case intersection_3_way:
        Draw_Model(node->road_type->intersection_3_way);
        break;
    case intersection_4_way:
        Draw_Model(node->road_type->intersection_4_way);
        break;
    }
    glPopMatrix();
}

//void Draw_Road_Segment(Road_Segment road, struct Model road_model)
void Draw_Road_Segment(Road_Segment road)
{
    glPushMatrix();

    // Ha az út X tengellyel párhuzamos
    if (road.A->pos.x == road.B->pos.x)
    {
        // Megnézni, hogy A vagy B-e a kisebb és ott kezdeni
        if (road.A->pos.y < road.B->pos.y)
        {
            glTranslatef(road.A->pos.x, road.A->pos.y, 0);
        }
        else
        {
            glTranslatef(road.B->pos.x, road.B->pos.y, 0);
        }

        // Út kirajzolása a szakasz hossza mentén
        glRotatef(90, 0, 0, 1);
        glTranslatef(1, 0, 0);
        for (int i = 0; i < road.length - 1; i++)
        {
            //Draw_Model(&road_model);
            Draw_Model(road.road_type->straight);
            glTranslatef(1, 0, 0);
        }
    }
    // Ha az út Y tengelylyel párhuzamos
    else if (road.A->pos.y == road.B->pos.y)
    {
        // Megnézni, hogy A vagy B a kisebb és ott kezdeni
        if (road.A->pos.x < road.B->pos.x)
        {
            glTranslatef(road.A->pos.x, road.A->pos.y, 0);
        }
        else
        {
            glTranslatef(road.B->pos.x, road.B->pos.y, 0);
        }

        // Út kirajzolása a szakasz hossza mentén
        glTranslatef(1, 0, 0);
        for (int i = 0; i < road.length - 1; i++)
        {
            //Draw_Model(&road_model);
            Draw_Model(road.road_type->straight);
            glTranslatef(1, 0, 0);
        }
    }

    glPopMatrix();
}
void Delete_Road_Segment(Road_Segment* deleted_road_segment, Node road_nodes[map_width][map_length])
{
    //Road_Segment* deleted_road_segment = tiles[x][y].occupied_by_road_segment;

    // Tile-ok felszabadítása
    for (int w = 0; w < map_width; w++)
    {
        for (int l = 0; l < map_length; l++)
        {
            if (tiles[w][l].occupied_by_road_segment == deleted_road_segment)
                tiles[w][l].occupied_by_road_segment = NULL;
        }
    }

    // Node-ok közötti kapcsolat törlése
    int a_x = deleted_road_segment->A->pos.x;
    int a_y = deleted_road_segment->A->pos.y;
    int b_x = deleted_road_segment->B->pos.x;
    int b_y = deleted_road_segment->B->pos.y;
    if (a_x == b_x)
    {
        if (a_y > b_y)
        {
            road_nodes[a_x][a_y].connection_north.x = 0;
            road_nodes[a_x][a_y].connection_north.y = 0;

            road_nodes[b_x][b_y].connection_south.x = 0;
            road_nodes[b_x][b_y].connection_south.y = 0;
        }
        else if (b_y > a_y)
        {
            road_nodes[a_x][a_y].connection_south.x = 0;
            road_nodes[a_x][a_y].connection_south.y = 0;

            road_nodes[b_x][b_y].connection_north.x = 0;
            road_nodes[b_x][b_y].connection_north.y = 0;
        }
    }
    else if (a_y == b_y)
    {
        if (a_x > b_x)
        {
            road_nodes[a_x][a_y].connection_west.x = 0;
            road_nodes[a_x][a_y].connection_west.y = 0;

            road_nodes[b_x][b_y].connection_east.x = 0;
            road_nodes[b_x][b_y].connection_east.y = 0;
        }
        else if (b_x > a_x)
        {
            road_nodes[a_x][a_y].connection_east.x = 0;
            road_nodes[a_x][a_y].connection_east.y = 0;

            road_nodes[b_x][b_y].connection_west.x = 0;
            road_nodes[b_x][b_y].connection_west.y = 0;
        }
    }
    deleted_road_segment->A->number_of_connections--;
    deleted_road_segment->B->number_of_connections--;
    Update_Road_Node(deleted_road_segment->A);
    Update_Road_Node(deleted_road_segment->B);

    // Út szakasz törlése
    deleted_road_segment->exists = 0;
    deleted_road_segment->length = 0;
    deleted_road_segment->A = NULL;
    deleted_road_segment->B = NULL;
}
void Split_Road_Segment(Road_Segment* road_to_split, Road_Segment road_segments[], Node road_nodes[map_width][map_length], int x, int y)
{
    // Eredeti Node koordináták megjegyzése
    int a_x = road_to_split->A->pos.x;
    int a_y = road_to_split->A->pos.y;
    int b_x = road_to_split->B->pos.x;
    int b_y = road_to_split->B->pos.y;

    // Eredeti szakasz törlése
    Delete_Road_Segment(road_to_split, road_nodes);

    // Új szakaszok létrehozása
    Place_Road_Segment(road_segments, road_nodes, road_to_split->road_type, a_x, a_y, x, y);
    Place_Road_Segment(road_segments, road_nodes, road_to_split->road_type, x, y, b_x, b_y);
}

void Delete_Road_Node(int x, int y, Road_Segment road_segments[], Node road_nodes[map_width][map_length])
{
    for (int i = 0; i < segment_limit; i++)
    {
        if (road_segments[i].A == &road_nodes[x][y] || road_segments[i].B == &road_nodes[x][y])
        {
            Delete_Road_Segment(&road_segments[i], road_nodes);
        }
    }
}
