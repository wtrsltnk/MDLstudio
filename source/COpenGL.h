#ifndef COPENGL_H
#define COPENGL_H

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
//#include <gl/glaux.h>

#include "..\hlsdk\mathlib.h"
#include "..\hlsdk\studio.h"
#include "..\hlsdk\studio_render.h"

#include "controls\GUI.h"
#include "Settings.h"

#define VIEWTYPE_CENTER				0
#define VIEWTYPE_PLAYER				1
#define VIEWTYPE_MATERIAL			2
#define VIEWTYPE_NORMAL				3

class COpenGl : public CControl
{
public:

    COpenGl( HINSTANCE hInstance, HWND hParent, int nX, int nY, int nWidth, int nHeight, int nId );

    ~COpenGl();

	void create( void );
	void setPixelFormat( void );
    void purge( void );
	void resize( int, int, int, int );

	bool init( StudioModel *pModel );
	void initfont( void );
	void draw( void );
	void drawmaterial( void );
	void drawtext( int x, int y, unsigned int font, const char *format, ...);
	void drawgrid( void );
	void clear( void );

	void rotate( float, float, float );
	void move( float, float, float );
	void centerview( void );
	void vmodelview( void );

	void setKey( UINT, bool );
	void showFPS( void );

	void changeAnimspeed( float );
	void setAnimspeed( float );

	void setMaterial( int material );
	int getPan(){return panz;};
	
	LRESULT MsgProc( HWND, UINT, WPARAM, LPARAM );

private:
	StudioModel							*m_pModel;
	int									control_id;

    void reset();

	HWND								m_hParent;
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
	bool								m_bAnimate;
	float								m_fAnimSpeed;

	int									m_nMaterial;

	unsigned int						font;

	float								LastReading;	
	int									frame;
	int									time;
	int									timebase;

	GLfloat								LightAmbient[4];//=		{ 0.3f, 0.3f, 0.3f, 1.0f };
	GLfloat								LightDiffuse[4];//=		{ 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat								LightPosition[4];//=	{ 1.0f, -1.0f, 1.0f, 0.0f };
	
	void setupRenderMode( void );
	void setupViewMode( void );
};

#endif
