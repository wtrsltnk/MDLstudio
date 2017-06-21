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
	settings.show_bones = false;
	settings.show_hitbox = false;
	settings.show_atach = false;

	settings.zoom = 1.0f;
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
