#include "Settings.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Settings settings;

void InitSettings (void)
{
	memset (&settings, 0, sizeof (Settings));

	settings.lightcolor[0] = 1.0f;
	settings.lightcolor[1] = 1.0f;
	settings.lightcolor[2] = 1.0f;

	settings.bgcolor[0] = 0.9f;
	settings.bgcolor[1] = 1.0f;
	settings.bgcolor[2] = 1.0f;

	settings.hidepanel = false;
	settings.show_bones = true;
	settings.show_hitbox = true;
	settings.show_atach = true;
	settings.show_browse = true;
	settings.show_polys = true;
	settings.show_grid = true;

	settings.rendermode = 4;
	settings.viewmode = 6;

	settings.animate = true;
	settings.speed = 1.0f;

	settings.browse.rcNormalPosition.left = 0;
	settings.browse.rcNormalPosition.top = 0;
	settings.browse.rcNormalPosition.right = 500;
	settings.browse.rcNormalPosition.bottom = 200;

	settings.me.rcNormalPosition.left = 0;
	settings.me.rcNormalPosition.top = 0;
	settings.me.rcNormalPosition.right = 300;
	settings.me.rcNormalPosition.bottom = 200;
}



int LoadSettings (char *filename)
{
	FILE *file = fopen (filename, "rb");

	if (!file)
		return 0;

	fread (&settings, sizeof (Settings), 1, file);
	fclose (file);

	return 1;
}



int SaveSettings (char *filename)
{
	FILE *file = fopen (filename, "wb");

	if (!file)
		return 0;

	fwrite (&settings, sizeof (Settings), 1, file);
	fclose (file);

	return 1;
}
