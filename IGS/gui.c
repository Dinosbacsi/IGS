#include "gui.h"

void Create_Button(char name[50], char group[50], int pos_x, int pos_y, int size_x, int size_y, float* text_color, float* background_color, float* text_color_hover, float* background_color_hover)
{
	for (int i = 0; i < sizeof(buttons) / sizeof(Button); i++)
	{
		if (!buttons[i].exsists)
		{
			sprintf(buttons[i].name, name);
			sprintf(buttons[i].group, group);
			buttons[i].position.x = pos_x;
			buttons[i].position.y = pos_y;
			buttons[i].size.x = size_x;
			buttons[i].size.y = size_y;

			for (int j = 0; j < 4; j++)
			{
				buttons[i].text_color[j] = text_color[j];
				buttons[i].background_color[j] = background_color[j];

				buttons[i].text_color_hover[j] = text_color_hover[j];
				buttons[i].background_color_hover[j] = background_color_hover[j];
			}
			buttons[i].exsists = true;

			//printf("Gomb letrehozva! \n");

			return;
		}
	}

	return;
}

void Delete_Button(Button* button)
{
	sprintf(button->name, "");
	sprintf(button->text, "");
	sprintf(button->group, "");
	button->position.x = 0;
	button->position.y = 0;
	button->size.x = 0;
	button->size.y = 0;

	for (int j = 0; j < 4; j++)
	{
		buttons->text_color[j] = 0.0f;
		buttons->background_color[j] = 0.0f;
	}
	button->exsists = false;

	return;
}

void Change_Button_Text(Button* button, float* text_color, float* text_color_hover)
{
	for (int j = 0; j < 4; j++)
	{
		button->text_color[j] = text_color[j];
		button->text_color_hover[j] = text_color_hover[j];
	}
}

void Render_Button(Button* button, bool hightlight)
{
	// Karakter változó
	char* c;

	// HÁTTÉR
	// Háttér színének beállítása
	if (hightlight)
	{
		glColor4f(button->background_color_hover[0], button->background_color_hover[1], button->background_color_hover[2], button->background_color_hover[3]);
	}
	else
	{
		glColor4f(button->background_color[0], button->background_color[1], button->background_color[2], button->background_color[3]);
	}
	
	glLoadIdentity();
	// Kirajzolás
	glBegin(GL_QUADS);
	glVertex3i(button->position.x - 5, button->position.y - 20, 0);
	glVertex3i(button->position.x - 5, button->position.y + 5, 0);
	glVertex3i(button->position.x + button->size.x, button->position.y + 5, 0);
	glVertex3i(button->position.x + button->size.x, button->position.y - 20, 0);
	glEnd();

	// FELIRAT
	// Felirat színének beállítása
	if (hightlight)
	{
		glColor4f(button->text_color_hover[0], button->text_color_hover[1], button->text_color_hover[2], button->text_color_hover[3]);
	}
	else
	{
		glColor4f(button->text_color[0], button->text_color[1], button->text_color[2], button->text_color[3]);
	}
	
	// Pozícionálás
	glLoadIdentity();
	glRasterPos3i(button->position.x, button->position.y, 0);
	// Kiíratás
	for (c = button->name; *c != '\0'; c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
	}

	// Szín visszaállítása
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

Button* Clicked_Button(int cursor_x, int cursor_y)
{
	for (int i = 0; i < sizeof(buttons) / sizeof(Button); i++)
	{
		if (cursor_x > buttons[i].position.x - 5 &&
			cursor_x < buttons[i].position.x + buttons[i].size.x &&
			cursor_y > buttons[i].position.y - 20 &&
			cursor_y < buttons[i].position.y + 5)
		{
			return &buttons[i];
		}
	}

	return NULL;
}

void Create_Button_List(char** list, int list_size, char* group_name, int pos_x, int pos_y)
{
	Delete_Button_List(group_name);

	for (int i = 1; i < list_size; i++)
	{
		char name[50];
		strcpy(name, list[i]);

		char group[50];
		strcpy(group, group_name);

		Create_Button(name, group, pos_x, pos_y + i * 25, 205, 50, text_color_white, bg_color, text_color_white, bg_color_hover);
	}
}

void Add_To_Button_List(char* button_name, char* group_name, int pos_x, int pos_y)
{
	// Csoporthoz tartozó korábbi gombok megszámolása
	int previous_buttons = 1;

	for (int i = 0; i < sizeof(buttons) / sizeof(Button); i++)
	{
		if (!strcmp(buttons[i].group, group_name))
		{
			previous_buttons++;
		}
	}

	// Gomb létrehozása
	char name[50];
	strcpy(name, button_name);

	char group[50];
	strcpy(group, group_name);

	Create_Button(name, group, pos_x, pos_y + previous_buttons * 25, 205, 50, text_color_white, bg_color, text_color_white, bg_color_hover);
}

void Delete_Button_List(char group_name[50])
{
	for (int i = 0; i < sizeof(buttons) / sizeof(Button); i++)
	{
		if (strcmp(buttons[i].group, group_name) == 0)
		{
			Delete_Button(&buttons[i]);
		}
	}
}
