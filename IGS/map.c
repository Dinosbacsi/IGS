#include "map.h"

void Draw_Skybox(GLuint sky_texture)
{
    // Textúra beállítása
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBindTexture(GL_TEXTURE_2D, sky_texture);
    // Köd kikapcsolás
    glDisable(GL_FOG);

    // Kirajzolás
    glPushMatrix();
    glTranslatef(player_zone_start_x + player_zone_width / 2, player_zone_start_y + player_zone_length / 2, 0);
    // Észak
    glBegin(GL_QUADS);
    glTexCoord2f(0.5f, 0.33f);
    glVertex3d(250, 250, 250);

    glTexCoord2f(0.5f, 0.66f);
    glVertex3d(250, 250, -250);

    glTexCoord2f(0.75f, 0.66f);
    glVertex3d(250, -250, -250);

    glTexCoord2f(0.75f, 0.33f);
    glVertex3d(250, -250, 250);
    glEnd();

    // Dél
    glBegin(GL_QUADS);
    glTexCoord2f(0.001f, 0.33f);
    glVertex3d(-250, -250, 250);

    glTexCoord2f(0.001f, 0.66f);
    glVertex3d(-250, -250, -250);

    glTexCoord2f(0.25f, 0.66f);
    glVertex3d(-250, 250, -250);

    glTexCoord2f(0.25f, 0.33f);
    glVertex3d(-250, 250, 250);
    glEnd();

    // Kelet
    glBegin(GL_QUADS);
    glTexCoord2f(0.25f, 0.33f);
    glVertex3d(-250, 250, 250);

    glTexCoord2f(0.25f, 0.66f);
    glVertex3d(-250, 250, -250);

    glTexCoord2f(0.5f, 0.66f);
    glVertex3d(250, 250, -250);

    glTexCoord2f(0.5f, 0.33f);
    glVertex3d(250, 250, 250);
    glEnd();

    // Nyugat
    glBegin(GL_QUADS);
    glTexCoord2f(0.75f, 0.33f);
    glVertex3d(250, -250, 250);

    glTexCoord2f(0.75f, 0.66f);
    glVertex3d(250, -250, -250);

    glTexCoord2f(0.999f, 0.66f);
    glVertex3d(-250, -250, -250);

    glTexCoord2f(0.999f, 0.33f);
    glVertex3d(-250, -250, 250);
    glEnd();

    // Fel
    glBegin(GL_QUADS);
    glTexCoord2f(0.25f, 0.0f);
    glVertex3d(250, -250, 250);

    glTexCoord2f(0.25f, 0.33f);
    glVertex3d(250, 250, 250);

    glTexCoord2f(0.5f, 0.33f);
    glVertex3d(-250, 250, 250);

    glTexCoord2f(0.5f, 0.0f);
    glVertex3d(-250, -250, 250);
    glEnd();

    glPopMatrix();

    // Textúra mód, köd visszaállítása
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glEnable(GL_FOG);
}

/*
======================================================================================
    Tile kezelõ függvények
*/
//bool Check_Tile(int x, int y, struct Tile tiles[map_width][map_length])
int Check_Tile(int x, int y)
{
    if (tiles[x][y].occupied_by_building != NULL)
    {
        tiles[x][y].highlighted = true;
        return 1;
    }
    else if (tiles[x][y].occupied_by_node != NULL)
    {
        tiles[x][y].highlighted = true;
        return 2;
    }
    else if (tiles[x][y].occupied_by_road_segment != NULL)
    {
        tiles[x][y].highlighted = true;
        return 3;
    }

    return 0;
}

void Draw_Tile(struct Tile tile, GLuint tile_texture)
{
    // Tile textúra beállítása
    glBindTexture(GL_TEXTURE_2D, tile_texture);

    glPushMatrix();
    // Mátrix mozgatása a Tile pozíciójára
    glTranslatef(tile.pos.x, tile.pos.y, 0);
    // Tile kirajzolása
    glBegin(GL_QUADS);
    // Normálvektor beállítása
    glNormal3f(0.0f, 0.0f, 1.0f);
    // Lap kirajzolása
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, 0.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(0.5f, 0.5f, 0.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-0.5f, 0.5f, 0.0f);
    glEnd();
    glPopMatrix();
}

void Draw_Highlight(struct Tile tile)
{
    // Szín beállítása
    glDisable(GL_LIGHTING);
    //glEnable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D, 0);
    glColor4f(1.0f, 0.0f, 0.0f, 0.5f);

    glPushMatrix();
    // Mátrix mozgatása a Tile pozíciójára
    glTranslatef(tile.pos.x, tile.pos.y, 0.0);

    // Kirajzolás
    glBegin(GL_QUADS);
    glVertex3f(-0.5f, -0.5f, 1.0f);
    glVertex3f(0.5f, -0.5f, 1.0f);
    glVertex3f(0.5f, 0.5f, 1.0f);
    glVertex3f(-0.5f, 0.5f, 1.0f);
    glEnd();

    glPopMatrix();

    // Fény, színek visszaállítása
    glEnable(GL_LIGHTING);
    //glDisable(GL_BLEND);
    glColor3f(1.0f, 1.0f, 1.0f);
}
