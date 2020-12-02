#ifndef SETTINGS_H
#define SETTINGS_H

#include <windows.h>

#define RENDERMODE_VERTEX			0
#define RENDERMODE_WIREFRAME		1
#define RENDERMODE_FLAT				2
#define RENDERMODE_SMOOTH			3
#define RENDERMODE_TEXTURED			4

#define VIEWMODE_LEFT				0
#define VIEWMODE_RIGHT				1
#define VIEWMODE_TOP				2
#define VIEWMODE_BOTTOM				3
#define VIEWMODE_FRONT				4
#define VIEWMODE_BACK				5
#define VIEWMODE_PERSPECTIVE		6

typedef struct
{
	float				lightcolor[3];
	float				bgcolor[3];
	bool				hidepanel;
	bool				show_bones;
	bool				show_atach;
	bool				show_hitbox;
	bool				show_browse;
	bool				show_polys;
	bool				show_fps;
	bool				show_grid;
	int					rendermode;
	int					viewmode;
	bool				animate;
	float				speed;
	WINDOWPLACEMENT		browse;
	WINDOWPLACEMENT		me;
} Settings;

extern Settings settings;

void InitSettings ();

int LoadSettings (
    const char *filename);

int SaveSettings (
    const char *filename);


#endif // INCLUDED_VIEWERSETTINGS
