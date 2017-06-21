#ifndef COPENGL_H
#define COPENGL_H

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glaux.h>

#include "..\hlsdk\mathlib.h"
#include "..\hlsdk\studio.h"
#include "..\hlsdk\studio_render.h"

#include "controls\CSlider.h"

#define RENDERMODE_VERTEX			0
#define RENDERMODE_WIREFRAME		1
#define RENDERMODE_FLAT				2
#define RENDERMODE_SMOOTH			3
#define RENDERMODE_TEXTURE			4
#define RENDERMODES					5

#define VIEWTYPE_CENTER				0
#define VIEWTYPE_PLAYER				1
#define VIEWTYPE_MATERIAL			2
#define VIEWTYPE_NORMAL				3

class COpenGl
{
public:

    COpenGl( HINSTANCE, HWND, int, int, int, int );

    ~COpenGl();

	void create( void );
	void setPixelFormat( void );
    void purge( void );
	void resize( int, int, int, int );

	void load( char * ) throw();
	void unload( void );
	void draw( void );
	void drawmaterial( void );
	void drawtext( char *text );
	void handlekeys( void );

	void rotate( float, float, float );
	void move( float, float, float );

	void setKey( UINT, bool );
	void setupRenderMode( void );
	void setRenderMode( int );
	int getRenderMode( void );
	void resetView( int type );
	void sequenceFFW( void );
	void sequenceRW( void );
	void setSequence( int );
	void setBodygroup( int, int );
	void setAnimate( bool animate );
	void toggleRenderMode( void );
	void changeAnimspeed( float );
	void setAnimspeed( float );
	void setSelHitbox( int index );
	void setSelBone( int index );
	void setSlider( CSlider *pSlider );
	void setFrame( int frame );
	void setMaterial( int material );
	void setSkin( int skin );
	float setMouth( float value );
	float setController( int index, float value );

	long getPolycount( void );

	StudioModel	getModel () const { return tempmodel; }
	
	LRESULT MsgProc( HWND, UINT, WPARAM, LPARAM );

private:
	static StudioModel tempmodel;

    void reset();

	HWND								m_hWnd, m_hParent;
	HINSTANCE							m_hInstance;
    HDC									m_hDC;
    HGLRC								m_hRC;
	int									m_nX, m_nY, m_nWidth, m_nHeight;

	float								oldx, oldy;
	float								oldpx, oldpy, oldpz;
	float								panx, pany, panz;
	float								oldrx, oldry, oldrz;
	float								rotx, roty, rotz;

	bool								m_bKeys[255];
	int									m_nRendermode;
	bool								m_bShowMdl;
	bool								m_bAnimate;
	float								m_fAnimSpeed;
	int									m_nView;
	int									m_nMaterial;
};

#endif