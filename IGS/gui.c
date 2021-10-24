#include "gui.h"

void Create_Button(char name[50], char group[50], int pos_x, int pos_y, int size_x, int size_y, float* text_color, float* background_color, float* text_color_hover, float* background_color_hover)
{
	for (int i = 1; i < sizeof(buttons) / sizeof(Button); i++)
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
	glVertex3i(button->position.x - 5, button->position.y - 20, 1);
	glVertex3i(button->position.x - 5, button->position.y + 5, 1);
	glVertex3i(button->position.x + button->size.x, button->position.y + 5, 1);
	glVertex3i(button->position.x + button->size.x, button->position.y - 20, 1);
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
	glRasterPos3i(button->position.x, button->position.y, 1);
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

Button* Get_Button_By_Name(char name[50])
{
	Button* button = NULL;

	for (int i = 0; i < sizeof(buttons) / sizeof(Button); i++)
	{
		if (!strcmp(buttons[i].name, name))
		{
			button = &buttons[i];
		}
	}

	return button;
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

Button* Get_Button_List_Element_By_Index(char* group_name, int index)
{
	int button_index = 0;
	Button* found_button = NULL;

	for (int i = 0; i < sizeof(buttons) / sizeof(Button); i++)
	{
		if (!strcmp(buttons[i].group, group_name))
		{
			if (button_index == index)
			{
				found_button = &buttons[i];
				return found_button;
			}

			button_index++;
		}
	}

	return found_button;
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

void Create_Panel(char name[50], char title[150], int pos_x, int pos_y, int size_x, int size_y)
{
	for (int i = 1; i < sizeof(panels) / sizeof(Panel); i++)
	{
		if (!panels[i].exsists)
		{
			sprintf(panels[i].name, name);
			sprintf(panels[i].title, title);

			panels[i].position.x = pos_x;
			panels[i].position.y = pos_y;
			panels[i].size.x = size_x;
			panels[i].size.y = size_y;

			for (int j = 0; j < 4; j++)
			{
				panels[i].background_color[j] = bg_color[j];
			}

			panels[i].exsists = true;
			panels[i].visible = true;

			return;
		}
	}
}

Panel* Get_Panel_By_Name(char panel_name[50])
{
	Panel* panel = NULL;

	for (int i = 0; i < sizeof(panels) / sizeof(Panel); i++)
	{
		if (!strcmp(panels[i].name, panel_name))
		{
			panel = &panels[i];
		}
	}

	return panel;
}

void Set_Building_For_Panel(Panel* panel, Building* building)
{
	panel->building = building;
}

void Delete_Panel(Panel* panel)
{
	panel->exsists = false;

	sprintf(panel->name, "");
	sprintf(panel->title, "");
	panel->position.x = 0;
	panel->position.y = 0;
	panel->size.x = 0;
	panel->size.y = 0;

	panel->visible = false;
	panel->building = NULL;
}

void Delete_Panel_By_Name(char panel_name_to_delete[50])
{
	for (int i = 0; i < sizeof(panels) / sizeof(Panel); i++)
	{
		if (!strcmp(panels[i].name, panel_name_to_delete))
		{
			Delete_Panel(&panels[i]);
		}
	}
}

Panel* Clicked_Panel(int cursor_x, int cursor_y)
{
	for (int i = 0; i < sizeof(panels) / sizeof(Panel); i++)
	{
		if (cursor_x > panels[i].position.x &&
			cursor_x < panels[i].position.x + panels[i].size.x &&
			cursor_y > panels[i].position.y &&
			cursor_y < panels[i].position.y + panels[i].size.y)
		{
			return &panels[i];
		}
	}

	return NULL;
}

void Render_Panel(Panel* panel)
{
	// Háttér kirajzolása
	glColor4f(panel->background_color[0], panel->background_color[1], panel->background_color[2], panel->background_color[3]);
	glLoadIdentity();

	glBegin(GL_QUADS);
	glVertex3i(panel->position.x, panel->position.y, 0);
	glVertex3i(panel->position.x + panel->size.x, panel->position.y, 0);
	glVertex3i(panel->position.x + panel->size.x, panel->position.y + panel->size.y, 0);
	glVertex3i(panel->position.x, panel->position.y + panel->size.y, 0);
	glEnd();

	// Panel címsorának kiíratása
	Render_Bitmap_String(panel->position.x + 2, panel->position.y + 18, 1, GLUT_BITMAP_HELVETICA_18, panel->title, text_color_yellow[0], text_color_yellow[1], text_color_yellow[2]);

	// Tartalom kiíratása
	int line_break = 0;
	Building* building = panel->building;

	if (building != NULL)
	{
		char category[50] = "Category: ";
		Render_Bitmap_String(panel->position.x + 2, panel->position.y + 42 + (line_break * 20), 0, GLUT_BITMAP_HELVETICA_18, strcat(category, Building_Type_String(building->category)), text_color_white[0], text_color_white[1], text_color_white[2]);
		line_break++;

		if (building->category == factory ||building->category == warehouse)
		{
			if (building->category == factory || building->category == warehouse)
			{
				// Termelés infó
				if (building->category == factory)
				{
					Render_Bitmap_String(panel->position.x + 2, panel->position.y + 42 + (line_break * 20), 0, GLUT_BITMAP_HELVETICA_18, "Produces:", text_color_white[0], text_color_white[1], text_color_white[2]);
					line_break++;

					int material_types_listed = 0;
					Delete_Button_List("building_produces");
					for (int i = 0; i < sizeof(material_types) / sizeof(Material); i++)
					{
						if (material_types[i].category == finished)
						{
							Add_To_Button_List(material_types[i].name, "building_produces", panel->position.x + 95, panel->position.y + (line_break * 20));

							if (&material_types[i] == building->produces)
							{
								Button* this_button = Get_Button_List_Element_By_Index("building_produces", material_types_listed);
								if (this_button != NULL)
								{
									Change_Button_Text(this_button, text_color_yellow, text_color_yellow);
								}
							}

							material_types_listed++;
						}
					}
					line_break += material_types_listed;
				}

				// Beszállító/vevõ épületek választó
				Render_Bitmap_String(panel->position.x + 2, panel->position.y + 42 + (line_break * 20), 0, GLUT_BITMAP_HELVETICA_18, "Src. from:", text_color_white[0], text_color_white[1], text_color_white[2]);
				Button* source_from_button = Get_Button_List_Element_By_Index("building_source_from", 0);
				if (source_from_button == NULL)
				{
					Create_Button("Not selected", "building_source_from", panel->position.x + 95, panel->position.y + 42 + (line_break * 20), 205, 50, text_color_white, bg_color, text_color_white, bg_color_hover);
				}

				if (source_from_button != NULL)
				{
					if(building->source_from != NULL)
						sprintf(source_from_button->name, building->source_from->name);
					else
						sprintf(source_from_button->name, "Not selected");
				}
				line_break++;

				Render_Bitmap_String(panel->position.x + 2, panel->position.y + 42 + (line_break * 20), 0, GLUT_BITMAP_HELVETICA_18, "Src. to:", text_color_white[0], text_color_white[1], text_color_white[2]);
				Button* source_to_button = Get_Button_List_Element_By_Index("building_source_to", 0);
				if (source_to_button == NULL)
				{
					Create_Button("Not selected", "building_source_to", panel->position.x + 95, panel->position.y + 42 + (line_break * 20), 205, 50, text_color_white, bg_color, text_color_white, bg_color_hover);
				}

				if (source_to_button != NULL)
				{
					if (building->deliver_to != NULL)
						sprintf(source_to_button->name, building->deliver_to->name);
					else
						sprintf(source_to_button->name, "Not selected");
				}
				line_break++;

			}

			// Raktár infó
			Render_Bitmap_String(panel->position.x + 2, panel->position.y + 42 + (line_break * 20), 0, GLUT_BITMAP_HELVETICA_18, "Storage:", text_color_white[0], text_color_white[1], text_color_white[2]);
			for (int i = 0; i < building->storage_capacity; i++)
			{
				char material_name_in_storage[50] = "[empty]";

				if (building->storage[i] != NULL)
				{
					sprintf(material_name_in_storage, building->storage[i]->name);
				}

				Render_Bitmap_String(panel->position.x + 80, panel->position.y + 42 + (line_break * 20), 0, GLUT_BITMAP_HELVETICA_18, material_name_in_storage, text_color_white[0], text_color_white[1], text_color_white[2]);
				line_break++;
			}
		}
	}

	panel->size.y = 40 + (line_break * 20);

	// Szín visszaállítása
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}
