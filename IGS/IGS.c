/*
======================================================================================
    Ipari Vállalatok Termelési és Gyártási Folyamatainak Grafikus Szimulációja
                            szakdolgozat program
                        Pente Viktor Domonkos (ENOMZK)
======================================================================================
*/

/*
======================================================================================
    Headerek
*/

#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS

//Általános headerek
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <float.h>
#include <windows.h>

//SDL, OpenGL, SOIL
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_mixer.h>
#include <GL\GLU.h>
#include <GL\freeglut.h>
#include <SOIL.h>

    //Saját headerek
#include "model.h"
#include "utility.h"
#include "map.h"
#include "building.h"
#include "road.h"
#include "vehicle.h"
#include "simulation.h"
#include "gui.h"


/*
======================================================================================
    Globális változók
*/
//SDL ablak és OpenGL kontextus
SDL_Window* gWindow = NULL;
SDL_GLContext gContext;
//SDL esemény változó
SDL_Event e;
//Ablak méret
int SCREEN_WIDTH = 1280;
int SCREEN_HEIGHT = 720;
//SDL Mixer csatornák száma
int mix_channels = 5;
//Főciklus futásáról döntő változó
bool running = 1;
//Egérgomb lenyomás változó
bool mouse_left_clicked = false;
//Játékmód változók
enum mode { normal, build, road, bulldoze };
enum mode game_mode;
//Debug mód változó
bool debug = false;
//Menükezelő változók
int menu_highlighted_building_category = 1;
int menu_selected_building_category = -1;
int menu_highlighted_building_type = 1;
int menu_selected_building_type = -1;
int bt_c;
//Kamera
struct Camera camera;
//Kurzor
struct Cursor cursor;
struct Virtual_Cursor v_cursor;
//billentyűzet állapota
const Uint8* state;
//Alap fénybeállítások
GLfloat light_pos_default[] = { 0.5f, 1.0f, 1.0f, 0.0f };           //fény pozíciója
GLfloat light_color_default[] = { 1.0f, 1.0f, 1.0f, 1.0f };   //fény színe
GLfloat global_ambient_light[] = { 0.3f, 0.3f, 0.3f, 1.0f };  //globális fény
    //Épület limit
#define building_limit 5000
    //Út node limit
#define node_limit 5000
#define segment_limit 5000
// IDEIGLENES(?)
    //Textúra változók
GLuint tex_grass;
GLuint tex_sky;
GLuint tex_igs_warehouse_1;
GLuint tex_igs_warehouse_2;
GLuint tex_igs_factory_1;
GLuint tex_igs_tank_1;
GLuint tex_igs_truck_small_box;
GLuint tex_igs_road_new;
GLuint tex_igs_road_new2;
GLuint tex_igs_road_main;
GLuint tex_igs_tree_1;
GLuint tex_igs_shrubline_1;
// Modell változók
struct Model igs_hills;
struct Model test_model;
struct Model igs_warehouse_1;
struct Model igs_warehouse_2;
struct Model igs_factory_1;
struct Model igs_tank_1;
struct Model test_truck;
struct Model test_wheel;
struct Model igs_road_straight;
struct Model igs_road_curve;
struct Model igs_road_3_way;
struct Model igs_road_4_way;
struct Model igs_road_dead_end;
struct Model igs_road_main_straight;
struct Model igs_road_main_curve;
struct Model igs_road_main_3_way;
struct Model igs_road_main_4_way;
struct Model igs_road_main_dead_end;
struct Model igs_tree_1;
struct Model igs_shrubline_1;

//Épületek
Building test_building;
Building new_building;
Building igs_warehouse_small;
Building igs_warehouse_medium;
Building igs_factory_small;
Building igs_tank_small;
Building buildings[building_limit];
//Fák
Scenery_Object trees[tree_limit];
//Utak - IDEIGLENES
Road_Segment test_road;
Road_Segment test_road2;
Node test_A;
Node test_B;
Road_Type road_normal;
Road_Type road_main;
//Épület kategóriák, típusok
//Building building_types[50];

//Anyagok
//Felirat
char test[50] = "TESZT FELIRAT";
char text[50];
char text_fps[10];

/*
======================================================================================
    Függvény prototípusok
*/
void Initialize_SDL();
void Initialize_SDL_Mixer();
void Initialize_OpenGL();
void Initialize_Textures();
void Initialize_Models();
void Initialize_Map();
void Initialize_Interface();
void Event_Handler();
void Interface_Button_Handler();
void Mouse_Handler();
void Build_Mode_Handler();
void Road_Mode_Handler();
void Bulldoze_Mode_Handler();
void Simulation();
void Render_Scene();
void Render_Interface();
void Render_Bitmap_String(int x, int y, int z, void* font, char* string, float r, float g, float b);
void Render_Bitmap_String_With_Backdrop(int x, int y, int z, void* font, char* string, float r, float g, float b, float br, float bg, float bb);
void Set_2D_Projection();
void Restore_3D_Projection();
void Reshape();
void Close();


/*
======================================================================================
    Main függvény
*/
int main(int argc, char* args[])
{
    // SDL inicializáció ---------------------------------------------------------
    Initialize_SDL();
    // SDL mixer inicializáció ---------------------------------------------------
    Initialize_SDL_Mixer();
    // OpenGL inicializáció ------------------------------------------------------
    Initialize_OpenGL();
    // GLUT inicializáció
    glutInit(&argc, args);
    // Textúrák betöltése
    Initialize_Textures();
    // Modellek betöltése
    Initialize_Models();
    // Pálya inicializáláas
    Initialize_Map();
    // UI inicializálása
    Initialize_Interface();


    camera.pos.x = player_zone_start_x + player_zone_width / 2;
    camera.pos.y = player_zone_start_y + player_zone_length / 2;
    camera.pos.z = 0;
    camera.angle_h = 0;
    camera.angle_v = 1;
    camera.distance = 5;
    camera.speed = 0.01f;
    camera.sensitivity = 0.005f;
    camera.field_of_view = 30;

    // Program futása
    while (running)
    {
        // SDL esemény kezelés
        Event_Handler();
        // Egérmozgatás kezelés
        Mouse_Handler();
        // Építő mód
        if (game_mode == build)
        {
            Build_Mode_Handler();
        }
        if (game_mode == road)
        {
            Road_Mode_Handler();
        }
        // Bontó mód
        if (game_mode == bulldoze)
        {
            Bulldoze_Mode_Handler();
        }

        // Szimuláció
        Simulation();
        // Renderelés
        Render_Scene();
        // Ablak átméretezés
        Reshape();
    }

    // SDL leállítása, erőforrások felszabadítása, ablak bezárása
    Close();

    return 0;
}

/*
======================================================================================
    Függvények
*/
void Initialize_SDL()
{
    //Inicializáció
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("HIBA! SDL nem inicializálódott! \nSDL hibakód: %s \n", SDL_GetError());
    }

    //Ablak és kontextus létrehozása
    gWindow = SDL_CreateWindow("IGS", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    gContext = SDL_GL_CreateContext(gWindow);

    //OpenGL verzió beállítása (2.1)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    //OpenGL élsimítás beállítása
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    //Átméretezhető ablak beállítása
    SDL_SetWindowResizable(gWindow, SDL_TRUE);
    //VSync beállítása
    if (SDL_GL_SetSwapInterval(1) < 0)
    {
        printf("HIBA! VSync nem állítódott be! \nSDL hibakód: %s \n", SDL_GetError());
    }
}

void Initialize_SDL_Mixer()
{
    // Inicializáció
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0)
    {
        printf("HIBA! SDL Mixer nem inicializálódott! \nSDL Mixer hibakód: %s\n", Mix_GetError());
    }

    // Csatornák számának beállítása
    if (Mix_AllocateChannels(mix_channels) < 0)
    {
        printf("HIBA! Nem lehetett beállítani a hang csatornák számát! \nSDL hibakód: %s\n", SDL_GetError());
    }

    // Hangeffekt fájlok betöltése

    // Zene fájlok betöltése
}

void Initialize_OpenGL()
{
    GLenum error = GL_NO_ERROR;

    // Modelview Mátrix inicializálás
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // Hiba ellenőrzés
    error = glGetError();
    if (error != GL_NO_ERROR)
    {
        printf("HIBA! OpenGL nem inicializálódott! \nOpenGL hibakód: %s\n", gluErrorString(error));
    }

    //OpenGL beállítások
    //glEnable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_MULTISAMPLE_ARB);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Köd
    glEnable(GL_FOG);
    float FogCol[3] = { 0.8f,0.9f,0.9f };
    glFogfv(GL_FOG_COLOR, FogCol);
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogf(GL_FOG_START, 50.f);
    glFogf(GL_FOG_END, 200.f);


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //Árnyékolás módjának beállítása
    glShadeModel(GL_SMOOTH);

    //Fény beállítás
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos_default);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color_default);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient_light);

    // Clear Color beállítás
    glClearColor(0.8f, 0.8f, 0.8f, 0.0f);
    glClearDepth(1.0f);
}

void Initialize_Textures()
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Textúra beállítások
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glEnable(GL_TEXTURE_2D);

    // Textúrák betöltése
    tex_grass = SOIL_load_OGL_texture
    (
        "textures/ground_grass.png",
        /*SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT*/
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB
    );
    tex_sky = SOIL_load_OGL_texture
    (
        "textures/skybox_1.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB
    );
    tex_igs_warehouse_1 = SOIL_load_OGL_texture
    (
        "textures/igs_warehouse_1.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB
    );
    tex_igs_warehouse_2 = SOIL_load_OGL_texture
    (
        "textures/igs_warehouse_2.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB
    );
    tex_igs_factory_1 = SOIL_load_OGL_texture
    (
        "textures/igs_factory_1.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB
    );
    tex_igs_tank_1 = SOIL_load_OGL_texture
    (
        "textures/igs_tank_1.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB
    );
    tex_igs_truck_small_box = SOIL_load_OGL_texture
    (
        "textures/igs_truck_small_box.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB
    );
    tex_igs_road_new = SOIL_load_OGL_texture
    (
        "textures/igs_road_new.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB
    );
    tex_igs_road_new2 = SOIL_load_OGL_texture
    (
        "textures/igs_road_new2.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB
    );
    tex_igs_road_main = SOIL_load_OGL_texture
    (
        "textures/igs_road_main.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB
    );
    tex_igs_tree_1 = SOIL_load_OGL_texture
    (
        "textures/igs_tree_2.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB
    );
    tex_igs_shrubline_1 = SOIL_load_OGL_texture
    (
        "textures/igs_shrubline_1.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB
    );

    glBindTexture(GL_TEXTURE_2D, tex_grass);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
}

void Initialize_Models()
{
    Load_Model("models/igs_hills_1.obj", &igs_hills, tex_grass);
    Load_Model("models/igs_warehouse_1.obj", &test_model, tex_igs_warehouse_1);
    Load_Model("models/igs_warehouse_1.obj", &igs_warehouse_1, tex_igs_warehouse_1);
    Load_Model("models/igs_warehouse_2.obj", &igs_warehouse_2, tex_igs_warehouse_2);
    Load_Model("models/igs_factory_1.obj", &igs_factory_1, tex_igs_factory_1);
    Load_Model("models/igs_tank_1.obj", &igs_tank_1, tex_igs_tank_1);
    Load_Model("models/igs_truck_small_box.obj", &test_truck, tex_igs_truck_small_box);
    Load_Model("models/igs_truck_small_wheel.obj", &test_wheel, tex_igs_truck_small_box);

    Load_Model("models/igs_road_straight.obj", &igs_road_straight, tex_igs_road_new);
    Load_Model("models/igs_road_curve.obj", &igs_road_curve, tex_igs_road_new);
    Load_Model("models/igs_road_3_way.obj", &igs_road_3_way, tex_igs_road_new);
    Load_Model("models/igs_road_4_way.obj", &igs_road_4_way, tex_igs_road_new);
    Load_Model("models/igs_road_end.obj", &igs_road_dead_end, tex_igs_road_new2);

    Load_Model("models/igs_road_main_straight.obj", &igs_road_main_straight, tex_igs_road_main);
    Load_Model("models/igs_road_main_curve.obj", &igs_road_main_curve, tex_igs_road_main);
    Load_Model("models/igs_road_main_3_way.obj", &igs_road_main_3_way, tex_igs_road_main);
    Load_Model("models/igs_road_main_4_way.obj", &igs_road_main_4_way, tex_igs_road_main);

    Load_Model("models/igs_tree_2d_1.obj", &igs_tree_1, tex_igs_tree_1);
    Load_Model("models/igs_shrubline_1.obj", &igs_shrubline_1, tex_igs_shrubline_1);
}

void Initialize_Map()
{
    // Ciklus a Tile-ok inicializálására
    for (int i = 0; i < map_width; i++)
    {
        for (int j = 0; j < map_length; j++)
        {
            tiles[i][j].pos.x = i;
            tiles[i][j].pos.y = j;
            tiles[i][j].occupied = 0;

            road_nodes[i][j].exists = false;
        }
    }
    // Épülettípusok inicializálása
    Building_Types_From_File("building_categories.txt");
    // Épületek tömb inicializálása
    for (int i = 0; i < sizeof(buildings)/sizeof(Building); i++)
    {
        buildings[i].exists = false;
        buildings[i].category = nothing;
        
        for (int l = 0; l < sizeof(buildings[i].storage) / sizeof(Material*); l++)
        {
            buildings[i].storage[l] = NULL;
        }

        for (int l = 0; l < sizeof(buildings[i].order_list) / sizeof(Material*); l++)
        {
            buildings[i].order_list[l] = NULL;
        }

        buildings[i].produces = NULL;
    }

    // Anyagtípusok inicializáláas
    printf("\n========== Anyagtipusok letrehozasa ==========\n");
    Make_Material_Type(&material_types[0], "Alapanyag", solid, raw, NULL, NULL);
    Make_Material_Type(&material_types[1], "Kesztermek", solid, finished, &material_types[0], NULL);

    // Porta
    char kis_porta[50] = "SMALL_GATE";
    Place_Building_By_Name(kis_porta, 99, 149, north, building_types, buildings, building_limit);

    // JÁRMŰ STRUKTÚRA TESZT
    test_vehicle.vehicle_model = test_truck;
    test_vehicle.wheel_model = test_wheel;
    test_vehicle.pos.x = 25.0f;
    test_vehicle.pos.y = 50.8f;
    test_vehicle.pos.z = 0.0f;
    test_vehicle.rotate.z = -10.0f;
    test_vehicle.max_speed = 0.02f;
    test_vehicle.acceleration_rate = 0.0001f;

    test_vehicle.wheel[0].x = 0.32f;
    test_vehicle.wheel[1].x = 0.32f;
    test_vehicle.wheel[2].x = 0.0f;
    test_vehicle.wheel[3].x = 0.0f;

    test_vehicle.wheel[0].y = 0.065f;
    test_vehicle.wheel[1].y = -0.065f;
    test_vehicle.wheel[2].y = 0.065f;
    test_vehicle.wheel[3].y = -0.065f;

    test_vehicle.wheel[0].z = 0.03f;
    test_vehicle.wheel[1].z = 0.03f;
    test_vehicle.wheel[2].z = 0.03f;
    test_vehicle.wheel[3].z = 0.03f;

    // Út típusok inicializáció
    sprintf(road_normal.name, "Atlagos ut");
    road_normal.straight = &igs_road_straight;
    road_normal.curve = &igs_road_curve;
    road_normal.intersection_3_way = &igs_road_3_way;
    road_normal.intersection_4_way = &igs_road_4_way;
    road_normal.dead_end = &igs_road_dead_end;

    sprintf(road_main.name, "Fo ut");
    road_main.straight = &igs_road_main_straight;
    road_main.curve = &igs_road_main_curve;
    road_main.intersection_3_way = &igs_road_main_3_way;
    road_main.intersection_4_way = &igs_road_main_4_way;
    road_main.dead_end = &igs_road_dead_end;

    // Utak tömb inicializálása
    for (int x = 0; x < map_width; x++)
    {
        for (int y = 0; y < map_length; y++)
        {
            road_nodes[x][y].exists = false;
            road_nodes[x][y].checked = false;
        }
    }

    for (int i = 0; i < sizeof(road_segments) / sizeof(Road_Segment); i++)
    {
        road_segments[i].exists = false;
    }

    // Fúót elhelyezése
    Place_Road_Segment(road_segments, road_nodes, &road_main, 104, 60, 104, 299);
    Place_Road_Segment(road_segments, road_nodes, &road_main, 104, 60, 110, 60);
    Place_Road_Segment(road_segments, road_nodes, &road_main, 110, 0, 110, 60);
    Place_Road_Segment(road_segments, road_nodes, &road_main, 104, 149, 98, 149);

    // Fájlból fa teszt
    FILE* file;
    char* filename = "tree_test.txt";

    char line[100];

    file = fopen(filename, "r");
    if (!file)
    {
        printf("Nem lehetett megnyitni a(z) %s fajlt!\n", filename);
    }
    else
    {
        int i = 0;
        while (i < tree_limit && fgets(line, sizeof(line), file) != NULL)
        {
            int tree_model;
            if (sscanf(line, "%f\t%f\t%f\t%f\t%d", &trees[i].pos.x, &trees[i].pos.y, &trees[i].pos.z, &trees[i].rotate, &tree_model))
            {
                switch (tree_model)
                {
                case 1:
                    trees[i].model = &igs_tree_1;
                    break;
                case 2:
                    trees[i].model = &igs_shrubline_1;
                    break;
                }

                trees[i].exsists = true;
            }

            i++;
        }
    }
}

void Initialize_Interface()
{
    printf("\n========== UI inicializalas ==========\n");
    for (int i = 0; i < sizeof(buttons) / sizeof(Button); i++)
    {
        if (buttons[i].exsists)
        {
            Delete_Button(&buttons[i]);
        }
    }

    char button_name[50];
    char button_group[50];
    sprintf(button_group, "sidemenu");

    sprintf(button_name, "NEW BUILDING");
    Create_Button(button_name, button_group, 10, 30, 205, 50, text_color_white, bg_color, text_color_white, bg_color_hover);

    sprintf(button_name, "NEW ROAD");
    Create_Button(button_name, button_group, 10, 55, 205, 50, text_color_white, bg_color, text_color_white, bg_color_hover);

    sprintf(button_name, "BULLDOZE");
    Create_Button(button_name, button_group, 10, 80, 205, 50, text_color_white, bg_color, text_color_white, bg_color_hover);
}

void Event_Handler()
{
    while (SDL_PollEvent(&e) != 0)
    {
        switch (e.type)
        {
            // Ablak bezárás esemény
        case SDL_QUIT:
            running = 0;
            break;
            // Egérgörgő esemény
        case SDL_MOUSEWHEEL:
            if (e.wheel.y < 0)
                camera.distance += 1;
            else if (e.wheel.y > 0)
                camera.distance -= 1;
            break;
            // Egérgomb lenyomás esemény
        case SDL_MOUSEBUTTONDOWN:
            if (e.button.button == SDL_BUTTON_LEFT)
            {
                // Megnézni, melyik gomb lett megnyomva
                Button* clicked_button = Clicked_Button(cursor.pos.x, cursor.pos.y);

                // Gombnyomás lekezelése
                if (clicked_button != NULL)
                {
                    char* clicked_button_text = clicked_button->name;
                    strcpy(clicked_button_text, clicked_button->name);

                    // Oldal menü
                    if (!strcmp(clicked_button->group, "sidemenu"))
                    {
                        // Gomb színek visszaállítása alaphelyzetbe
                        for (int i = 0; i < sizeof(buttons) / sizeof(Button); i++)
                        {
                            Change_Button_Text(&buttons[i], text_color_white, text_color_white);
                        }

                        if (!strcmp(clicked_button_text, "NEW BUILDING"))
                        {
                            if (game_mode != build)
                            {
                                game_mode = build;
                                Create_Button_List(building_category_names_UPPER_CASE, sizeof(building_category_names_UPPER_CASE) / sizeof(building_category_names_UPPER_CASE[0]), "building categories", 230, 5);

                                // Megnyomott gomb kiemelés szín
                                Change_Button_Text(clicked_button, text_color_yellow, text_color_yellow);
                            }
                            else
                            {
                                game_mode = normal;

                                // Kilépés az adott menüből
                                Delete_Button_List("building types");
                                Delete_Button_List("building categories");
                            }
                        }
                        else
                        {
                            Delete_Button_List("building types");
                            Delete_Button_List("building categories");
                        }
                        
                        if (!strcmp(clicked_button_text, "NEW ROAD"))
                        {
                            if (game_mode != road)
                            {
                                game_mode = road;

                                // Megnyomott gomb kiemelés szín
                                Change_Button_Text(clicked_button, text_color_yellow, text_color_yellow);
                            }
                            else
                            {
                                game_mode = normal;
                            }
                        }

                        if (!strcmp(clicked_button_text, "BULLDOZE"))
                        {
                            if (game_mode != bulldoze)
                            {
                                game_mode = bulldoze;
                            }
                            else
                            {
                                game_mode = normal;
                            }
                        }
                    }
                    else if (!strcmp(clicked_button->group, "building categories"))
                    {
                        Delete_Button_List("building types");

                        for (int i = 0; i < sizeof(building_types) / sizeof(Building); i++)
                        {
                            if (building_types[i].category == Building_Type_Enum(clicked_button->name))
                            {
                                Add_To_Button_List(building_types[i].name, "building types", 450, 5);
                            }
                        }

                        // Gomb színek visszaállítása alaphelyzetbe
                        for (int i = 0; i < sizeof(buttons) / sizeof(Button); i++)
                        {
                            if (!strcmp(buttons[i].group, "building categories"))
                            {
                                Change_Button_Text(&buttons[i], text_color_white, text_color_white);
                            }
                            
                        }

                        // Megnyomott gomb kiemelés szín
                        Change_Button_Text(clicked_button, text_color_yellow, text_color_yellow);
                    }
                    else if (!strcmp(clicked_button->group, "building types"))
                    {
                        for (int i = 0; i < sizeof(building_types) / sizeof(Building); i++)
                        {
                            if (!strcmp(building_types[i].name, clicked_button->name))
                            {
                                // A megtalált indexű épülettípus behelyezése az új épületbe
                                new_building.category = building_types[i].category;
                                new_building.building_model = building_types[i].building_model;
                                new_building.size.x = building_types[i].size.x;
                                new_building.size.y = building_types[i].size.y;
                            }
                        }

                        // Gomb színek visszaállítása alaphelyzetbe
                        for (int i = 0; i < sizeof(buttons) / sizeof(Button); i++)
                        {
                            if (!strcmp(buttons[i].group, "building types"))
                            {
                                Change_Button_Text(&buttons[i], text_color_white, text_color_white);
                            }

                        }

                        // Megnyomott gomb kiemelés szín
                        Change_Button_Text(clicked_button, text_color_yellow, text_color_yellow);
                    }
                }
                else
                {
                    mouse_left_clicked = true;
                }
            }
            else if (e.button.button == SDL_BUTTON_RIGHT)
            {
                // Pozíció mentése kameraforgatáshoz
                cursor.last.x = cursor.pos.x;
                cursor.last.y = cursor.pos.y;
            }
            break;
            // Billentyű lenyomás esemény
        case SDL_KEYDOWN:
            switch (e.key.keysym.sym)
            {
                // Escape : kilépés
            case SDLK_ESCAPE:
                running = 0;
                break;
                // F11 : Debug infó ki/bekapcsolása
            case SDLK_F11:
                debug = !debug;
                break;

                // N : építő mód
            case SDLK_n:
                // Építő mód ki/be kapcsolása
                if (game_mode == build)
                    game_mode = normal;
                else
                    game_mode = build;

                // Menü sor változó alaphelyzetbe állítása
                menu_selected_building_category = -1;

                // Építendő épület alaphelyzetbe állítása
                new_building.category = nothing;
                new_building.facing_direction = north;
                break;

                // M : útépító mód
            case SDLK_m:
                // Út építő mód ki/be kapcsolása
                if (game_mode == road)
                {
                    game_mode = normal;

                    new_segment.exists = false;
                }
                else
                    game_mode = road;
                mouse_left_clicked = false;
                break;

                // B : bontó mód
            case SDLK_b:
                if (game_mode == bulldoze)
                    game_mode = normal;
                else
                    game_mode = bulldoze;
                break;

                // 1 : építő módban -> raktár
            case SDLK_1:
                if (game_mode == build)
                    new_building.category = warehouse;
                new_building.building_model = test_model;
                break;
                // 2 : debug módban -> test jármű
            case SDLK_2:
                if (debug == 1)
                {
                    Place_Vehicle(vehicles, &test_vehicle, (int)roundf(v_cursor.pos.x), (int)roundf(v_cursor.pos.y), road_segments, road_nodes);
                }
                break;

                // R : építő módban -> forgatás
            case SDLK_r:
                if (game_mode == build)
                    switch (new_building.facing_direction)
                    {
                    case north:
                        new_building.facing_direction = east;
                        break;
                    case east:
                        new_building.facing_direction = south;
                        break;
                    case south:
                        new_building.facing_direction = west;
                        break;
                    case west:
                        new_building.facing_direction = north;
                        break;
                    }
                break;

                // FEL : menüben -> kiválasztás fel
            case SDLK_UP:
                if (game_mode == build && menu_selected_building_category == -1)
                {
                    if (menu_highlighted_building_category > 1)
                        menu_highlighted_building_category--;
                }
                else if (game_mode == build && menu_selected_building_type == -1)
                {
                    if (menu_highlighted_building_type > 1)
                        menu_highlighted_building_type--;
                }
                break;

                // LE : menüben -> kiválasztás le
            case SDLK_DOWN:
                if (game_mode == build && menu_selected_building_category == -1)
                {
                    if (menu_highlighted_building_category < last - 1)
                        menu_highlighted_building_category++;
                }
                else if (game_mode == build && menu_selected_building_type == -1)
                {
                    if (menu_highlighted_building_type < bt_c)
                        menu_highlighted_building_type++;
                }
                break;

                // ENTER : menüben -> belépés a kategóriába
                //       : kategóriában -> épület kiválasztása
            case SDLK_RETURN:
                if (game_mode == build && menu_selected_building_category == -1)
                {
                    menu_selected_building_category = menu_highlighted_building_category;
                }
                else if (game_mode == build && menu_selected_building_category != -1 && menu_selected_building_type == -1)
                {
                    menu_selected_building_type = menu_highlighted_building_type;
                }
                break;

                // BACKSPACE : menüben -> kilépés a kategóriából
            case SDLK_BACKSPACE:
                if (game_mode == build && menu_selected_building_category != -1)
                {
                    menu_highlighted_building_category = menu_selected_building_category;
                    menu_selected_building_category = -1;
                    menu_highlighted_building_type = 1;
                }
                else if (game_mode == build && menu_selected_building_category == -1)
                {
                    game_mode = normal;
                }
                break;
            }
        }
    }
}

void Mouse_Handler()
{
    // Kamera forgatás egérnyomásra
    if (SDL_GetMouseState(&cursor.pos.x, &cursor.pos.y) & SDL_BUTTON(3))
    {
        Rotate_Camera(&camera, &cursor, gWindow, SCREEN_WIDTH, SCREEN_HEIGHT);
    }
    else
        // Kamera mozgatása élgörgetéssel
    {
        SDL_GetMouseState(&cursor.pos.x, &cursor.pos.y);
        // Kurzor a képernyő felső szélén
        if (cursor.pos.x <= 10)
            Move_Camera_Relative(&camera, up);
        // Kurzor a képernyő alsó szélén
        else if (cursor.pos.x >= SCREEN_WIDTH - 10)
            Move_Camera_Relative(&camera, down);
        // Kurzor a képernyő bal szélén
        if (cursor.pos.y <= 10)
            Move_Camera_Relative(&camera, left);
        // Kurzor a képernyő jobb szélén
        else if (cursor.pos.y >= SCREEN_HEIGHT - 10)
            Move_Camera_Relative(&camera, right);


        if (camera.pos.x < player_zone_start_x)
            camera.pos.x = player_zone_start_x;
        if (camera.pos.x > player_zone_start_x + player_zone_width)
            camera.pos.x = player_zone_start_x + player_zone_width;
        if (camera.pos.y < player_zone_start_y)
            camera.pos.y = player_zone_start_y;
        if (camera.pos.y > player_zone_start_y + player_zone_length)
            camera.pos.y = player_zone_start_y + player_zone_length;
    }
}

void Build_Mode_Handler()
{
    // Ha van kiválasztott épülettípus:
    if (menu_selected_building_type != -1)
    {
        int bt_selected = 0;
        int bt_i = 0;

        // Végiglépkedés az épülettípusokon, megkeresve a listájukból a kiválasztottat
        while (bt_selected < menu_selected_building_type)
        {
            if (building_types[bt_i].category == building_category_list[menu_selected_building_category])
            {
                bt_selected++;
            }

            bt_i++;
        }

        if (bt_selected != 0 && bt_selected < 50)
        {
            printf("\n %d. epulettipus kivalasztva!", bt_i - 1);

            // A megtalált indexű épülettípus behelyezése az új épületbe
            new_building.category = building_types[bt_i - 1].category;
            new_building.building_model = building_types[bt_i - 1].building_model;
            new_building.size.x = building_types[bt_i - 1].size.x;
            new_building.size.y = building_types[bt_i - 1].size.y;

            menu_selected_building_type = -1;
        }
    }

    if (new_building.category != nothing)
    {
        //Tile-ok ellenőrzése
        bool tile_is_free = true;
        if (new_building.facing_direction == north || new_building.facing_direction == south)
        {
            int lx = (int)((float)new_building.pos.x - ((float)new_building.size.x) / 2);
            int ly = (int)((float)new_building.pos.y - ((float)new_building.size.y) / 2);

            for (int ix = 1; ix <= new_building.size.x; ix++)
            {
                for (int iy = 1; iy <= new_building.size.y; iy++)
                {
                    if (Check_Tile(lx + ix, ly + iy) != 0)
                        tile_is_free = false;
                }
            }
        }
        else if (new_building.facing_direction == east || new_building.facing_direction == west)
        {
            int lx = (int)((float)new_building.pos.x - ((float)new_building.size.y) / 2);
            int ly = (int)((float)new_building.pos.y - ((float)new_building.size.x) / 2);

            for (int ix = 1; ix <= new_building.size.y; ix++)
            {
                for (int iy = 1; iy <= new_building.size.x; iy++)
                {
                    if (Check_Tile(lx + ix, ly + iy) != 0)
                        tile_is_free = false;
                }
            }
        }

        // Új épület pozíciójának meghatározása
        new_building.pos.x = (int)roundf(v_cursor.pos.x);
        new_building.pos.y = (int)roundf(v_cursor.pos.y);

        if (new_building.pos.x < 0 + (new_building.size.x / 2))
        {
            new_building.pos.x = 0 + (new_building.size.x / 2);
        }
        if (new_building.pos.x > (map_width - 1) - (new_building.size.x / 2))
        {
            new_building.pos.x = (map_width - 1) - (new_building.size.x / 2);
        }
        if (new_building.pos.y < 0 + (new_building.size.y / 2))
        {
            new_building.pos.y = 0 + (new_building.size.y / 2);
        }
        if (new_building.pos.y > (map_length - 1) - (new_building.size.y / 2))
        {
            new_building.pos.y = (map_length - 1) - (new_building.size.y / 2);
        }

        // Ha bal egérgomb és tile szabad
        if (mouse_left_clicked && tile_is_free)
        {
            // Épület elhelyezése
            Place_Building_OLD(new_building.building_model, new_building.category, new_building.pos.x, new_building.pos.y, new_building.size.x, new_building.size.y, new_building.facing_direction, buildings, building_limit);

            // Építendő épület alaphelyzetbe állítása
            new_building.category = nothing;
            new_building.facing_direction = north;

            game_mode = normal;

            // Kilépés az adott menüből
            Delete_Button_List("building types");
            Delete_Button_List("building categories");

            // Gomb színek visszaállítása alaphelyzetbe
            for (int i = 0; i < sizeof(buttons) / sizeof(Button); i++)
            {
                Change_Button_Text(&buttons[i], text_color_white, text_color_white);
            }
        }
    }
}

void Road_Mode_Handler()
{
    bool free_to_build = true;

    if (new_segment.exists)
    {
        new_segment.B->pos.x = (int)roundf(v_cursor.pos.x);
        new_segment.B->pos.y = (int)roundf(v_cursor.pos.y);

        if (!(new_segment.A->pos.x == new_segment.B->pos.x) && !(new_segment.A->pos.y == new_segment.B->pos.y))
        {
            if (abs(new_segment.A->pos.x - (int)roundf(v_cursor.pos.x)) > abs(new_segment.A->pos.y - (int)roundf(v_cursor.pos.y)))
            {
                new_segment.B->pos.x = (int)roundf(v_cursor.pos.x);
                new_segment.B->pos.y = new_segment.A->pos.y;
            }
            else
            {
                new_segment.B->pos.x = new_segment.A->pos.x;
                new_segment.B->pos.y = (int)roundf(v_cursor.pos.y);
            }
        }

        if (new_segment.A->pos.x == new_segment.B->pos.x && new_segment.A->pos.y == new_segment.B->pos.y)
            new_segment.length = 0;
        else if (new_segment.A->pos.x == new_segment.B->pos.x)
        {
            new_segment.length = abs(new_segment.A->pos.y - new_segment.B->pos.y);

            int y = min(new_segment.A->pos.y, new_segment.B->pos.y);
            for (; y <= max(new_segment.A->pos.y, new_segment.B->pos.y); y++)
            {
                if (Check_Tile(new_segment.A->pos.x, y) == 1)
                {
                    tiles[new_segment.A->pos.x][y].highlighted = true;
                    free_to_build = false;
                }
            }
        }
        else
        {
            new_segment.length = abs(new_segment.A->pos.x - new_segment.B->pos.x);

            int x = min(new_segment.A->pos.x, new_segment.B->pos.x);
            for (; x <= max(new_segment.A->pos.x, new_segment.B->pos.x); x++)
            {
                if (Check_Tile(x, new_segment.A->pos.y) == 1)
                {
                    tiles[x][new_segment.A->pos.y].highlighted = true;
                    free_to_build = false;
                }
            }
        }
    }

    if (mouse_left_clicked)
    {
        if (new_segment.exists && free_to_build)
        {
            if (new_segment.A->pos.x == new_segment.B->pos.x)
            {
                int start_y = min(new_segment.A->pos.y, new_segment.B->pos.y);
                int mid_y = min(new_segment.A->pos.y, new_segment.B->pos.y);
                int end_y = max(new_segment.A->pos.y, new_segment.B->pos.y);

                for (mid_y = start_y; mid_y < end_y; mid_y++)
                {
                    if (Check_Tile(new_segment.A->pos.x, mid_y) == 2 || Check_Tile(new_segment.A->pos.x, mid_y) == 3)
                    {
                        Place_Road_Segment(road_segments, road_nodes, &road_normal, new_segment.A->pos.x, start_y, new_segment.A->pos.x, mid_y);
                        start_y = mid_y;
                    }
                }
                Place_Road_Segment(road_segments, road_nodes, &road_normal, new_segment.A->pos.x, start_y, new_segment.A->pos.x, end_y);
            }
            else if (new_segment.A->pos.y == new_segment.B->pos.y)
            {
                int start_x = min(new_segment.A->pos.x, new_segment.B->pos.x);
                int mid_x = min(new_segment.A->pos.x, new_segment.B->pos.x);
                int end_x = max(new_segment.A->pos.x, new_segment.B->pos.x);

                for (mid_x = start_x; mid_x < end_x; mid_x++)
                {
                    if (Check_Tile(mid_x, new_segment.A->pos.y) == 2 || Check_Tile(mid_x, new_segment.A->pos.y) == 3)
                    {
                        Place_Road_Segment(road_segments, road_nodes, &road_normal, start_x, new_segment.A->pos.y, mid_x, new_segment.A->pos.y);
                        start_x = mid_x;
                    }
                }
                Place_Road_Segment(road_segments, road_nodes, &road_normal, start_x, new_segment.A->pos.y, end_x, new_segment.A->pos.y);
            }

            new_segment.exists = false;
        }
        else if (!new_segment.exists && free_to_build)
        {
            new_segment.exists = true;
            new_segment.A = &new_segment_A;
            new_segment.B = &new_segment_B;
            new_segment.A->pos.x = (int)roundf(v_cursor.pos.x);
            new_segment.A->pos.y = (int)roundf(v_cursor.pos.y);
            new_segment.road_type = &road_normal;
        }
    }
}

void Bulldoze_Mode_Handler()
{
    if (v_cursor.pos.x > 0 && v_cursor.pos.y > 0 && v_cursor.pos.x < map_width && v_cursor.pos.y < map_length)
    {
        int tile_is_occupied = Check_Tile((int)roundf(v_cursor.pos.x), (int)roundf(v_cursor.pos.y));
        if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(1) && tile_is_occupied != 0)
        {
            switch (tile_is_occupied)
            {
                // 1 : Épület
            case 1:
                Bulldoze_Building_OLD(v_cursor, buildings);
                break;
                // 2: út Node
            case 2:
                Delete_Road_Node((int)roundf(v_cursor.pos.x), (int)roundf(v_cursor.pos.y), road_segments, road_nodes);
                break;
                // 3: út Segment
            case 3:
                Delete_Road_Segment(tiles[(int)roundf(v_cursor.pos.x)][(int)roundf(v_cursor.pos.y)].occupied_by_road_segment, road_nodes);
                break;
            }
        }
    }

}

void Simulation()
{
    mouse_left_clicked = false;

    // Szimuláció
    for (int i = 0; i < vehicle_limit; i++)
    {
        if (vehicles[i].exists == true)
        {
            Vehicle_Cruise(&vehicles[i], road_nodes);
        }

    }
    for (int i = 0; i < sizeof(buildings) / sizeof(Building); i++)
    {
        if (buildings[i].category == factory && (Check_Tile(buildings[i].entry_point.x, buildings[i].entry_point.y) == 3 || Check_Tile(buildings[i].entry_point.x, buildings[i].entry_point.y) == 2))
        {
            if (buildings[i].order_cooldown < 0)
            {
                Building_Produce(&buildings[i]);
                buildings[i].order_cooldown = 10000;
            }
            else
            {
                buildings[i].order_cooldown -= Get_Elapsed_Time();
            }
        }
    }

    // Majd mehetne simulation-be saját függvényként
    for (int i = 0; i < building_limit; i++)
    {
        if (buildings[i].exists == true && buildings[i].category == factory)
        {
            Material* order = Get_Order(&buildings[i]);

            if (order != NULL)
            {
                int spawn_pos_x = 110;
                int spawn_pos_y = 1;
                int new_vehicle_index = Place_Vehicle(vehicles, &test_vehicle, spawn_pos_x, spawn_pos_y, road_segments, road_nodes);

                if (new_vehicle_index != -1)
                {
                    vehicles[new_vehicle_index].destination_node = &road_nodes[buildings[i].entry_point.x][buildings[i].entry_point.y];
                    Find_Path(&vehicles[new_vehicle_index], road_nodes);
                }
            }
        }
    }
}

void Render_Scene()
{
    // Buffer kiürítése
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    // Fő mátrix push
    glPushMatrix();
    // Kamera pozícionálása
    Position_Camera(&camera);

    // Fény pozícionálása
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos_default);

    // Domb kirajzolás
    glPushMatrix();
    glTranslatef(map_width / 2 - 0.5, map_length / 2 - 0.5, 0);
    Draw_Model(&igs_hills);
    glPopMatrix();

    // Ciklus a Tile-ok kirajzolására
    for (int i = player_zone_start_x; i < player_zone_start_x + player_zone_width; i++)
    {
        for (int j = player_zone_start_y; j < player_zone_start_y + player_zone_length; j++)
        {
            Draw_Tile(tiles[i][j], tex_grass);
        }
    }
    // 3D kurzor kirajzolása
    Calculate3DCursorLocation(cursor.pos.x, cursor.pos.y, &v_cursor);
    if (game_mode == build || game_mode == road || game_mode == bulldoze)
        Restrict_3D_Cursor_To_Player_Area(&v_cursor);
    if (debug)
        Draw_3D_Cursor(v_cursor);

    // Ég kirajzolása
    Draw_Skybox(tex_sky);

    // Objektum Kirajzolása
    Draw_Building(test_building);
    Draw_Vehicle(&test_vehicle);
    for (int i = 0; i < building_limit; i++)
    {
        if (buildings[i].exists != 0)
        {
            Draw_Building(buildings[i]);
        }
    }
    if (game_mode == build && new_building.category != nothing)
    {
        Draw_Building(new_building);
    }
    for (int i = 0; i < vehicle_limit; i++)
    {
        if (vehicles[i].exists == true)
        {
            Draw_Vehicle(&vehicles[i]);
        }

    }

    // Utak kirajzolása
    for (int i = 0; i < segment_limit; i++)
    {
        if (road_segments[i].exists)
        {
            Draw_Road_Segment(road_segments[i]);
        }
    }
    if (game_mode == road)
    {
        if (new_segment.exists)
        {
            if (new_segment.length > 0)
            {
                Draw_Road_Segment(new_segment);
                if (new_segment.A->pos.x == new_segment.B->pos.x)
                {
                    glPushMatrix();
                    glTranslatef((float)new_segment.A->pos.x, (float)min(new_segment.A->pos.y, new_segment.B->pos.y), 0.0f);
                    glRotatef(90, 0, 0, 1);
                    Draw_Model(&igs_road_dead_end);
                    glPopMatrix();

                    glPushMatrix();
                    glTranslatef((float)new_segment.A->pos.x, (float)max(new_segment.A->pos.y, new_segment.B->pos.y), 0.0f);
                    glRotatef(-90, 0, 0, 1);
                    Draw_Model(&igs_road_dead_end);
                    glPopMatrix();
                }
                else if (new_segment.A->pos.y == new_segment.B->pos.y)
                {
                    glPushMatrix();
                    glTranslatef((float)min(new_segment.A->pos.x, new_segment.B->pos.x), (float)new_segment.A->pos.y, 0.0f);
                    Draw_Model(&igs_road_dead_end);
                    glPopMatrix();

                    glPushMatrix();
                    glTranslatef((float)max(new_segment.A->pos.x, new_segment.B->pos.x), (float)new_segment.A->pos.y, 0.0f);
                    glRotatef(180, 0, 0, 1);
                    Draw_Model(&igs_road_dead_end);
                    glPopMatrix();
                }
            }
            else
            {
                glPushMatrix();
                glTranslatef(roundf(v_cursor.pos.x), roundf(v_cursor.pos.y), 0);
                Draw_Model(&igs_road_straight);
                glPopMatrix();
            }
        }
        else
        {
            glPushMatrix();
            glTranslatef(roundf(v_cursor.pos.x), roundf(v_cursor.pos.y), 0);
            Draw_Model(&igs_road_straight);
            glPopMatrix();
        }
    }

    // Node-k kirajzoláas
    for (int i = 0; i < map_width; i++)
    {
        for (int j = 0; j < map_length; j++)
        {
            if (road_nodes[i][j].exists)
            {
                Draw_Road_Node(&road_nodes[i][j]);
            }
        }
    }
    // Fá és kamera közti távolságok kiszámítása
    for (int i = 0; i < tree_limit; i++)
    {
        if (trees[i].exsists == true)
        {
            trees[i].distance_from_camera = Distance(camera.POV.x, trees[i].pos.x, camera.POV.y, trees[i].pos.y);
        }
    }
    // Fák sorbarendezése kamerától vett távolság alapján
    for (int i = 0; i < tree_limit - 1; i++)
    {
        int min_index = i;
        for (int j = i + 1; j < tree_limit - 1; j++)
        {
            if (trees[j].distance_from_camera < trees[min_index].distance_from_camera)
            {
                min_index = j;
            }

            Scenery_Object tree_swap_placeholder = trees[j];
            trees[j] = trees[min_index];
            trees[min_index] = tree_swap_placeholder;
        }
    }
    // Fák kirajzolása
    glDisable(GL_LIGHTING);
    for (int i = 0; i < tree_limit; i++)
    {
        if (trees[i].exsists == true)
        {
            glPushMatrix();
            glTranslatef(trees[i].pos.x, trees[i].pos.y, trees[i].pos.z);
            if (trees[i].rotate == -1)
                glRotatef((float)(camera.angle_h * (180 / PI) - 180), 0, 0, 1);
            else
                glRotatef(trees[i].rotate, 0, 0, 1);

            Draw_Model(trees[i].model);
            glPopMatrix();
        }
    }
    glEnable(GL_LIGHTING);

    // Rácsháló kirajzolása
    //Draw_Full_Grid(map_width, map_length);
    if (game_mode == build || game_mode == road)
        Draw_Local_Grid(v_cursor);

    // Tile kijelölés rajzolása
    if (debug == true)
    {
        for (int i = 0; i < building_limit; i++)
        {
            if (buildings[i].exists)
            {
                tiles[buildings[i].entry_point.x][buildings[i].entry_point.y].highlighted = true;
            }
        }
        for (int i = 0; i < map_width; i++)
        {
            for (int j = 0; j < map_length; j++)
            {
                if (tiles[i][j].occupied != 0 || tiles[i][j].occupied_by_building != NULL || tiles[i][j].occupied_by_node != NULL || tiles[i][j].occupied_by_road_segment != NULL)
                    tiles[i][j].highlighted = true;

                if (tiles[i][j].highlighted == true)
                {
                    Draw_Highlight(tiles[i][j]);
                    tiles[i][j].highlighted = false;
                }
            }
        }
    }
    else
    {
        for (int i = 0; i < map_width; i++)
        {
            for (int j = 0; j < map_length; j++)
            {
                if (tiles[i][j].highlighted == true)
                {
                    Draw_Highlight(tiles[i][j]);
                    tiles[i][j].highlighted = false;
                }
            }
        }
    }

    // Interface kirajzolása
    Render_Interface();

    glPopMatrix();
    // Képernyõ frissítés
    SDL_GL_SwapWindow(gWindow);
}

void Render_Interface()
{
    // 2D-s projekció beállítása
    Set_2D_Projection();
    // Fények, textúra kikapcsolása
    glDisable(GL_LIGHTING);
    glBindTexture(GL_TEXTURE_2D, 0);

    // ÚJ GOMBOK TESZT
    for (int i = 0; i < sizeof(buttons) / sizeof(Button); i++)
    {
        if (buttons[i].exsists)
        {
            bool highlight = false;

            if (cursor.pos.x > buttons[i].position.x - 5 &&
                cursor.pos.x < buttons[i].position.x + buttons[i].size.x &&
                cursor.pos.y > buttons[i].position.y - 20 &&
                cursor.pos.y < buttons[i].position.y + 5)
            {
                highlight = true;
            }

            Render_Button(&buttons[i], highlight);
        }
    }

    // Debug infó kiírása
    if (debug == true)
    {
        // 3D kurzor koordinátáinak kiírása
        sprintf(text, "Kurzor X = %d", (int)roundf(v_cursor.pos.x));
        Render_Bitmap_String(SCREEN_WIDTH - 150, 40, 0, GLUT_BITMAP_HELVETICA_18, text, 1, 1, 1);
        sprintf(text, "Kurzor Y = %d", (int)roundf(v_cursor.pos.y));
        Render_Bitmap_String(SCREEN_WIDTH - 150, 60, 0, GLUT_BITMAP_HELVETICA_18, text, 1, 1, 1);

        // FPS kiszámítása és kiírása
        static float fps;
        static float last_time = 0;
        float current_time = GetTickCount64() * 0.001f;
        fps++;
        if (current_time - last_time > 1)
        {
            last_time = current_time;
            sprintf(text_fps, "FPS: %d", (int)fps);
            fps = 0;
        }
        Render_Bitmap_String(SCREEN_WIDTH - 150, 20, 0, GLUT_BITMAP_HELVETICA_18, text_fps, 1, 1, 0);
    }

    // Fények visszakapcsolása
    glEnable(GL_LIGHTING);
    // 3D-s projekció visszaállítása
    Restore_3D_Projection();
}

void Set_2D_Projection()
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    glMatrixMode(GL_MODELVIEW);
}

void Restore_3D_Projection()
{
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
}

void Reshape()
{
    //Mátrix újratöltése
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //Ablak méretének bekérése, perspektíva, viewport újra beállítása átméretezés után
    SDL_GetWindowSize(gWindow, &SCREEN_WIDTH, &SCREEN_HEIGHT);
    gluPerspective(camera.field_of_view, (GLdouble)SCREEN_WIDTH / (GLdouble)SCREEN_HEIGHT, 0.1, 500.0);
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void Close()
{
    // Ablak bezárása
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    SDL_Quit();
}
