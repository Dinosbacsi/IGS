#include "utility.h"

/*
======================================================================================
    Kamera függvények
*/
void Position_Camera(struct Camera* cam)
{
    cam->POV.x = cam->pos.x + (cam->distance * cos(cam->angle_h)) * cos(cam->angle_v);
    cam->POV.y = cam->pos.y + (cam->distance * sin(cam->angle_h)) * cos(cam->angle_v);
    cam->POV.z = cam->distance * sin(cam->angle_v);

    gluLookAt(
        // Kamera nézõpontjának koordinátája
        //cam->pos.x + (cam->distance * cos(cam->angle_h)) * cos(cam->angle_v), cam->pos.y + (cam->distance * sin(cam->angle_h)) * cos(cam->angle_v), cam->distance * sin(cam->angle_v),
        cam->POV.x, cam->POV.y, cam->POV.z,
        // Kamera célpontjának koordinátája
        cam->pos.x, cam->pos.y, cam->pos.z,
        // "Fel" iránya
        0.0, 0.0, 1.0
    );
}

void Move_Camera_To(struct Camera* cam, float new_cam_pos_x, float new_cam_pos_y)
{
    cam->pos.x = new_cam_pos_x;
    cam->pos.y = new_cam_pos_y;
}

void Move_Camera_Relative(struct Camera* cam, enum cam_direction direction)
{
    switch (direction)
    {
    case up:
        cam->pos.x -= cam->speed * -sin(cam->angle_h) * cam->distance;
        cam->pos.y -= cam->speed * cos(cam->angle_h) * cam->distance;
        break;
    case down:
        cam->pos.x += cam->speed * -sin(cam->angle_h) * cam->distance;
        cam->pos.y += cam->speed * cos(cam->angle_h) * cam->distance;
        break;
    case left:
        cam->pos.x -= cam->speed * cos(cam->angle_h) * cam->distance;
        cam->pos.y -= cam->speed * sin(cam->angle_h) * cam->distance;
        break;
    case right:
        cam->pos.x += cam->speed * cos(cam->angle_h) * cam->distance;
        cam->pos.y += cam->speed * sin(cam->angle_h) * cam->distance;
        break;
    }


}

void Rotate_Camera(struct Camera* cam, struct Cursor* cursor, SDL_Window* gWindow, int SCREEN_WIDTH, int SCREEN_HEIGHT)
{
    // Kurzormozgatás távolságának meghatározása
    cursor->dif.x = cursor->last.x - cursor->pos.x;
    cursor->dif.y = cursor->last.y - cursor->pos.y;

    // Kamera szögének módosítása
    cam->angle_h += cursor->dif.x * cam->sensitivity;
    cam->angle_v -= cursor->dif.y * cam->sensitivity;

    // Kamera vertikális szögének szabályozása
    if (cam->angle_v > (M_PI / 2) - 0.001)
        cam->angle_v = (M_PI / 2) - 0.001;
    else if (cam->angle_v < 0)
        cam->angle_v = 0;
    // Kamera horizontális szögének szabályozása
    if (cam->angle_h > 2 * M_PI)
        cam->angle_h = 0;
    else if (cam->angle_h < 0)
        cam->angle_h = 2 * M_PI;

    // Kurzor megállítása az ablak széleinél
    if (cursor->pos.x < SCREEN_WIDTH - 20 && cursor->pos.x > 20 && cursor->pos.y < SCREEN_HEIGHT - 20 && cursor->pos.y > 20)
    {
        cursor->last.x = cursor->pos.x;
        cursor->last.y = cursor->pos.y;
    }
    else
    {
        if (cursor->pos.x >= SCREEN_WIDTH - 20)
        {
            SDL_WarpMouseInWindow(gWindow, SCREEN_WIDTH - 20, cursor->pos.y);
            cursor->last.y = cursor->pos.y - cursor->dif.y;
        }
        else if (cursor->pos.x <= 20)
        {
            SDL_WarpMouseInWindow(gWindow, 20, cursor->pos.y);
            cursor->last.y = cursor->pos.y - cursor->dif.y;
        }

        if (cursor->pos.y >= SCREEN_HEIGHT - 20)
        {
            SDL_WarpMouseInWindow(gWindow, cursor->pos.x, SCREEN_HEIGHT - 20);
            cursor->last.x = cursor->pos.x - cursor->dif.x;
        }
        else if (cursor->pos.y <= 20)
        {
            SDL_WarpMouseInWindow(gWindow, cursor->pos.x, 20);
            cursor->last.x = cursor->pos.x - cursor->dif.x;
        }
    }
}


/*
======================================================================================
    3D kurzor
*/
void Draw_3D_Cursor(struct Virtual_Cursor v_cursor)
{
    glBindTexture(GL_TEXTURE_2D, 0);
    glPushMatrix();
    glTranslatef(v_cursor.pos.x, v_cursor.pos.y, 0);
    glScalef(0.3, 0.3, 0.3);
    glBegin(GL_TRIANGLES);
    //X tengely
        //piros
    glColor3f(1.0f, 0.0f, 0.0f);
    //y felöli oldal
    glNormal3f(1, 1, 0.8);
    glVertex3f(1, 0, 0);
    glVertex3f(0, 0.2, 0);
    glVertex3f(0, 0, 0.2);
    //-y felöli oldal
    glNormal3f(1, -1, 0.8);
    glVertex3f(0, -0.2, 0);
    glVertex3f(1, 0, 0);
    glVertex3f(0, 0, 0.2);

    //Y tengely
        //zöld
    glColor3f(0.0f, 1.0f, 0.0f);
    //x felöli oldal
    glNormal3f(1, 1, 0.8);
    glVertex3f(0, 1, 0);
    glVertex3f(0, 0, 0.2);
    glVertex3f(0.2, 0, 0);
    //-x felöli oldal
    glNormal3f(-1, 1, 0.8);
    glVertex3f(0, 1, 0);
    glVertex3f(-0.2, 0, 0);
    glVertex3f(0, 0, 0.2);

    //Z tengely
        //kék
    glColor3f(0.0f, 0.0f, 1.0f);
    //-x-y felöli oldal
    glNormal3f(-1, -1, 0.2);
    glVertex3f(0, 0, 0.5);
    glVertex3f(-0.2, 0, 0);
    glVertex3f(0, -0.2, 0);
    //x-y felöli oldal
    glNormal3f(1, -1, 0.2);
    glVertex3f(0, 0, 0.5);
    glVertex3f(0, -0.2, 0);
    glVertex3f(0.2, 0, 0);
    //xy felöli oldal
    glNormal3f(1, 1, 0.2);
    glVertex3f(0, 0, 0.5);
    glVertex3f(0.2, 0, 0);
    glVertex3f(0, 0.2, 0);
    //-xy felöli oldal
    glNormal3f(-1, 1, 0.2);
    glVertex3f(0, 0, 0.5);
    glVertex3f(0, 0.2, 0);
    glVertex3f(-0.2, 0, 0);
    glEnd();
    glPopMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
}
void Calculate3DCursorLocation(int cursorX, int cursorY, Virtual_Cursor* v_cursor)
{
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;

    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    winX = (float)cursorX;
    winY = (float)viewport[3] - (float)cursorY;
    glReadPixels(winX, winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
    gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

    v_cursor->pos.x = posX;
    v_cursor->pos.y = posY;
    v_cursor->pos.z = posZ;
}
// 3D kurzor játékos területen tartása
void Restrict_3D_Cursor_To_Player_Area(Virtual_Cursor* v_cursor)
{
    if (v_cursor->pos.x < player_zone_start_x)
    {
        v_cursor->pos.x = player_zone_start_x;
    }
    else if (v_cursor->pos.x > player_zone_start_x + player_zone_width - 1)
    {
        v_cursor->pos.x = player_zone_start_x + player_zone_width - 1;
    }

    if (v_cursor->pos.y < player_zone_start_y)
    {
        v_cursor->pos.y = player_zone_start_y;
    }
    else if (v_cursor->pos.y > player_zone_start_y + player_zone_length - 1)
    {
        v_cursor->pos.y = player_zone_start_y + player_zone_length - 1;
    }
}

void Draw_Full_Grid(int w, int l)
{
    // Ciklus változók
    int i, j;

    glPushMatrix();
    glTranslatef(-0.5f, -0.5f, 0);
    for (i = 0; i < w; i++)
    {
        glBegin(GL_LINES);
        glVertex3f(i, 0, 0.001);
        glVertex3f(i, l, 0.001);
        glEnd();
    }
    for (j = 0; j < l; j++)
    {
        glBegin(GL_LINES);
        glVertex3f(0, j, 0.001);
        glVertex3f(w, j, 0.001);
        glEnd();
    }
    glPopMatrix();
}

void Draw_Local_Grid(struct Virtual_Cursor v_cursor)
{
    // Kerekített pozíció változók
    float x, y;
    x = roundf(v_cursor.pos.x);
    y = roundf(v_cursor.pos.y);

    // Szín beállítása
    glDisable(GL_LIGHTING);
    glBindTexture(GL_TEXTURE_2D, 0);
    glColor3f(1.0f, 1.0f, 0.0f);

    // Kirajzolás
    glPushMatrix();
    glTranslatef(x, y, 0.001);

    // X tengely menti vonalak
    glBegin(GL_LINES);
    glVertex3f(-2, -3.5, 0);
    glVertex3f(+2, -3.5, 0);

    glVertex3f(-3, -2.5, 0);
    glVertex3f(+3, -2.5, 0);

    glVertex3f(-4, -1.5, 0);
    glVertex3f(+4, -1.5, 0);

    glVertex3f(-4, -0.5, 0);
    glVertex3f(+4, -0.5, 0);

    glVertex3f(-4, 0.5, 0);
    glVertex3f(+4, 0.5, 0);

    glVertex3f(-4, 1.5, 0);
    glVertex3f(+4, 1.5, 0);

    glVertex3f(-3, 2.5, 0);
    glVertex3f(+3, 2.5, 0);

    glVertex3f(-2, 3.5, 0);
    glVertex3f(+2, 3.5, 0);
    glEnd();

    // Y tengely menti vonalak
    glBegin(GL_LINES);
    glVertex3f(-3.5, -2, 0);
    glVertex3f(-3.5, +2, 0);

    glVertex3f(-2.5, -3, 0);
    glVertex3f(-2.5, +3, 0);

    glVertex3f(-1.5, -4, 0);
    glVertex3f(-1.5, +4, 0);

    glVertex3f(-0.5, -4, 0);
    glVertex3f(-0.5, +4, 0);

    glVertex3f(0.5, -4, 0);
    glVertex3f(0.5, +4, 0);

    glVertex3f(1.5, -4, 0);
    glVertex3f(1.5, +4, 0);

    glVertex3f(2.5, -3, 0);
    glVertex3f(2.5, +3, 0);

    glVertex3f(3.5, -2, 0);
    glVertex3f(3.5, +2, 0);
    glEnd();
    glPopMatrix();

    //Fény, színek visszaállítása
    glEnable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 1.0f);
}

/*
======================================================================================
    3D kurzor
*/
void Render_Bitmap_String(int x, int y, int z, void* font, char* string, float r, float g, float b)
{

    char* c;

    // Szín beállítása
    glColor3f(r, g, b);

    // Pozícionálás
    glLoadIdentity();
    glRasterPos3i(x, y, z);

    // Kiíratás
    for (c = string; *c != '\0'; c++)
    {
        glutBitmapCharacter(font, *c);
    }

    // Szín visszaállítása
    glColor3f(1.0f, 1.0f, 1.0f);
}

void Render_Bitmap_String_With_Backdrop(int x, int y, int z, void* font, char* string, float r, float g, float b, float br, float bg, float bb)
{
    // Karakter változó
    char* c;

    // HÁTTÉR
    // Háttér színének beállítása
    glColor4f(br, bg, bb, 0.5f);
    glLoadIdentity();
    // Kirajzolás
    glBegin(GL_QUADS);
    glVertex3i(x - 5, y - 20, z);
    glVertex3i(x - 5, y + 5, z);
    glVertex3i(x + 205, y + 5, z);
    glVertex3i(x + 205, y - 20, z);
    glEnd();

    // FELIRAT
    // Felirat színének beállítása
    glColor4f(r, g, b, 1);
    // Pozícionálás
    glLoadIdentity();
    glRasterPos3i(x, y, z);
    // Kiíratás
    for (c = string; *c != '\0'; c++)
    {
        glutBitmapCharacter(font, *c);
    }

    // Szín visszaállítása
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

/*
======================================================================================
    Random matek
*/
float Distance(float x1, float x2, float y1, float y2)
{
    float distance = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
    return distance;
}
