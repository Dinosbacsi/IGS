#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

#include "utility.h"
#include "building.h"
#include "material.h"

typedef struct Button {
	char name[50];
	char text[50];
	char group[50];
	vec2i position;
	vec2i size;
	float text_color[4];
	float text_color_hover[4];
	float background_color[4];
	float background_color_hover[4];
	bool exsists;
}Button;

// Gombok
Button buttons[100];
// Gomb színek
static float text_color_white[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
static float text_color_yellow[4] = { 1.0f, 1.0f, 0.0f, 1.0f };

static float bg_color[4] = { 0.0f, 0.0f, 0.0f, 0.5f };
static float bg_color_hover[4] = { 0.0f, 0.0f, 0.0f, 0.9f };

typedef struct Panel {
	char name[50];
	char title[150];
	vec2i position;
	vec2i size;
	float background_color[4];
	bool exsists;
	bool visible;

	Building* building;
}Panel;

// Panelek
Panel panels[20];

void Create_Button(char name[50], char group[50], int pos_x, int pos_y, int size_x, int size_y, float* text_color, float* background_color, float* text_color_hover, float* background_color_hover);
void Delete_Button(Button* button);
void Change_Button_Text(Button* button, float* text_color, float* text_color_hover);
void Render_Button(Button* button, bool highlight);
Button* Clicked_Button(int cursor_x, int cursor_y);

void Create_Button_List(char** list, int list_size, char* group_name, int pos_x, int pos_y);
void Add_To_Button_List(char* button_name, char* group_name, int pos_x, int pos_y);
void Delete_Button_List(char group_name[50]);

void Create_Panel(char name[50], char title[150], int pos_x, int pos_y, int size_x, int size_y);
Panel* Get_Panel_By_Name(char panel_name[50]);
void Set_Building_For_Panel(Panel* panel, Building* building);
void Delete_Panel(Panel* panel);
void Delete_Panel_By_Name(char panel_name_to_delete[50]);
Panel* Clicked_Panel(int cursor_x, int cursor_y);
void Render_Panel(Panel* panel);

#endif // GUI_H_INCLUDED
