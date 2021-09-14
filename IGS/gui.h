#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

#include "utility.h"

typedef struct Button {
	char name[50];
	char text[50];
	vec2i position;
	vec2i size;
	float text_color[4];
	float text_color_hover[4];
	float background_color[4];
	float background_color_hover[4];
	bool exsists;
}Button;

Button buttons[100];

void Create_Button(char name[50], int pos_x, int pos_y, int size_x, int size_y, float* text_color, float* background_color, float* text_color_hover, float* background_color_hover);
void Delete_Button(Button* button);
void Render_Button(Button* button, bool highlight);
Button* Clicked_Button(int cursor_x, int cursor_y);

#endif // GUI_H_INCLUDED
