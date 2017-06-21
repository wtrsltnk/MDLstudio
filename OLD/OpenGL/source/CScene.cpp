#include "CScene.h"

vec3_t		g_vright;		// needs to be set to viewer's right in order for chrome to work
float		g_lambert = 1.5;

StudioModel CScene::tempmodel;

CScene::CScene( void )
{
}

CScene::~CScene( void )
{
}

void CScene::draw( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glPushMatrix();


	glCullFace( GL_FRONT );
	glEnable( GL_DEPTH_TEST );


    glPopMatrix();

	/*
	glDepthFunc (GL_LEQUAL);
	glDepthRange (0, 10.0);
	glDepthMask( 1 );
*/
	tempmodel.SetBlending( 0, 0.0 );
	tempmodel.SetBlending( 1, 0.0 );

	static float prev;
	float curr = GetTickCount( ) / 1000.0f;
	tempmodel.AdvanceFrame( curr - prev );
	prev = curr;

	tempmodel.DrawModel( );
}

void CScene::init( char *modelname )
{
	tempmodel.Init( modelname );
	tempmodel.SetSequence( 0 );

	tempmodel.SetController( 0, 0.0 );
	tempmodel.SetController( 1, 0.0 );
	tempmodel.SetController( 2, 0.0 );
	tempmodel.SetController( 3, 0.0 );
	tempmodel.SetMouth( 0 );
}

void CScene::nextSequence( void )
{
	tempmodel.SetSequence( tempmodel.GetSequence()+1 );
}

void CScene::prevSequence( void )
{
	tempmodel.SetSequence( tempmodel.GetSequence()-1 );
}
