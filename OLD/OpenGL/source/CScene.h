#ifndef CSCENE_H
#define CSCENE_H

#include <stdio.h>
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glaux.h>

#include "mathlib.h"
#include "engine\studio.h"
#include "studio_render.h"


class CScene
{
private:
	static StudioModel tempmodel;
public:
    CScene( void );
    ~CScene( void );

	void draw( void );
	void init( char * );

	void nextSequence( void );
	void prevSequence( void );
};

#endif