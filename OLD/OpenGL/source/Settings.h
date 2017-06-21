#ifndef SETTINGS_H
#define SETTINGS_H

typedef struct
{
	float				lightcolor[3];
	float				bgcolor[3];
	bool				hidepanel;
	bool				show_bones;
	bool				show_atach;
	bool				show_hitbox;
	float				zoom;
} Settings;

extern Settings settings;

void InitSettings (void);
int LoadSettings (char *filename);
int SaveSettings (char *filename);


#endif // INCLUDED_VIEWERSETTINGS