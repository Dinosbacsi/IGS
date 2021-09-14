#include "gui.h"

void Create_Button(char name[50], int pos_x, int pos_y, int size_x, int size_y, float* text_color, float* background_color, float* text_color_hover, float* background_color_hover)
{
	for (int i = 0; i < sizeof(buttons) / sizeof(Button) && !buttons[i].exsists; i++)
	{
		sprintf(buttons[i].name, name);
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

		printf("Gomb letrehozva! \n");

		break;
	}

	return;
}

void Delete_Button(Button* button)
{
	sprintf(button->name, "");
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
