#include "COpenGL.h"
#include "Settings.h"

vec3_t		g_vright;		// needs to be set to viewer's right in order for chrome to work
float		g_lambert = 1.5;

static COpenGl * g_pCWin		= NULL;
StudioModel COpenGl::tempmodel;

//////////////////////////////////////////////////////////////////
// Koppeling WIN32 -> Klasse
//////////////////////////////////////////////////////////////////
LRESULT CALLBACK GLProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return g_pCWin->MsgProc(hWnd, uMsg, wParam, lParam);
}


COpenGl::COpenGl( HINSTANCE hInstance, HWND hParent, int nX, int nY, int nWidth, int nHeight )
{
	g_pCWin = this;
	this->m_hInstance = hInstance;
	this->m_hParent = hParent;
	this->m_nX = nX;
	this->m_nY = nY;
	this->m_nWidth = nWidth;
	this->m_nHeight = nHeight;

	this->rotx = -90.0f;
	this->roty = 0.0f;
	this->rotz = -90.0f;

	this->panx = 0.0f;
	this->pany = 0.0f;
	this->panz = -20.0f;

	this->oldx = -1.0f;
	this->oldy = -1.0f;

	this->oldrx = -1.0f;
	this->oldry = -1.0f;
	this->oldrz = -1.0f;

	this->oldpx = -1.0f;
	this->oldpy = -1.0f;
	this->oldpz = -1.0f;

	this->m_nRendermode = 4;
	this->m_fAnimSpeed = 1.0f;

	for ( int i = 0; i < 255; i++ )
		m_bKeys[i] = false;

	this->m_bShowMdl = false;
	this->m_bAnimate = true;

	this->m_nMaterial = 0;
	this->m_nView = VIEWTYPE_NORMAL;
	
	reset();
}

COpenGl::~COpenGl()
{
	tempmodel.FreeModel();
    purge();
}

LRESULT COpenGl::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	DWORD fwKeys;
	POINT  pt;

	switch ( uMsg )
	{
	case WM_CREATE:
		SetFocus( m_hWnd );
		break;

	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_LBUTTONDOWN:
		GetCursorPos( &pt );
		oldx = pt.x;
		oldy = pt.y;
		oldpx = panx;
		oldpy = pany;
		oldpz = panz;
		oldrx = rotx;
		oldry = roty;
		oldrz = rotz;
		SetFocus( m_hWnd );
		break;

	case WM_MOUSEMOVE:
		GetCursorPos( &pt );
		fwKeys = wParam;

		switch (fwKeys)
		{
		case MK_LBUTTON:
			rotz = oldrz - ( oldx - pt.x );
			rotx = oldrx - ( oldy - pt.y );
			break;
		case MK_RBUTTON:
			panz = oldpz + ( ( oldy - pt.y ) / 5 );
			break;
		case MK_MBUTTON:
			panx = oldpx - ( ( oldx - pt.x ) / 20 );
			pany = oldpy + ( ( oldy - pt.y ) / 20 );
			break;
		case MK_RBUTTON|MK_SHIFT:
			panx = oldpx - ( ( oldx - pt.x ) / 20 );
			pany = oldpy + ( ( oldy - pt.y ) / 20 );
			break;
		case MK_LBUTTON|MK_SHIFT:
			roty = oldry - ( oldx - pt.x );
			break;
		default:
			break;
		}
		break;

	case 0x020A: // WM_MOUSEWHEEL
		break;

	case WM_KEYDOWN:
		m_bKeys[wParam] = true;
		break;

	case WM_KEYUP:
		m_bKeys[wParam] = false;
		break;
	}
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

void COpenGl::create()
{
	WNDCLASSEX wcex;

	wcex.cbSize			= sizeof(WNDCLASSEX); 
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)GLProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= m_hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= "OPENGL";
	wcex.hIconSm		= NULL;

	int r = RegisterClassEx(&wcex);

	m_hWnd = CreateWindowEx( WS_EX_STATICEDGE, 
		"OPENGL",
		"OpenGl Window",
		WS_VISIBLE | WS_CHILD,
		m_nX, m_nY,
		m_nWidth, m_nHeight,
		m_hParent,
		NULL,
		m_hInstance,
		NULL );

    m_hDC = GetDC( m_hWnd );
	setPixelFormat();

    m_hRC = wglCreateContext( m_hDC );
    wglMakeCurrent( m_hDC, m_hRC );

	glEnable(GL_CULL_FACE);
}

void COpenGl::setPixelFormat()
{
    PIXELFORMATDESCRIPTOR pfd;
    int format;

    ZeroMemory( &pfd, sizeof( pfd ) );
    pfd.nSize = sizeof( pfd );
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    format = ChoosePixelFormat( m_hDC, &pfd );
    SetPixelFormat( m_hDC, format, &pfd );
}

void COpenGl::resize( int nX, int nY, int nWidth, int nHeight )
{
	this->m_nX = nX;
	this->m_nY = nY;
	this->m_nWidth = nWidth;
	this->m_nHeight = nHeight;

	MoveWindow( m_hWnd, m_nX, m_nY, m_nWidth, m_nHeight, true );

	if( m_nHeight == 0 )
		m_nHeight = 1;
	GLfloat ratio = (GLfloat)m_nWidth / (GLfloat)m_nHeight;
 
    wglMakeCurrent( m_hDC, m_hRC );   
	glViewport(0, 0, m_nWidth, m_nHeight);

    glMatrixMode(GL_PROJECTION);			
    glLoadIdentity();						

    gluPerspective(45.0f,(GLfloat)m_nWidth/(GLfloat)m_nHeight,0.1f,300.0f);
	
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void COpenGl::purge()
{
    if ( m_hRC )
    {
        wglMakeCurrent( NULL, NULL );
        wglDeleteContext( m_hRC );
    }
    if ( m_hWnd && m_hDC )
    {
        ReleaseDC( m_hWnd, m_hDC );
    }
    reset();
}

void COpenGl::reset()
{
    m_hWnd = NULL;
    m_hDC = NULL;
    m_hRC = NULL;
}

void COpenGl::load( char *modelname ) throw()
{
	if ( strcmp( modelname, "" ) != 0 )
	{
		tempmodel.FreeModel();
		tempmodel.Init( modelname );
		tempmodel.SetSelHitbox( -1 );
		tempmodel.SetSelBone( -1 );
		tempmodel.SetSequence( 0 );
		tempmodel.SetController( 0, 0.0 );
		tempmodel.SetController( 1, 0.0 );
		tempmodel.SetController( 2, 0.0 );
		tempmodel.SetController( 3, 0.0 );
		tempmodel.SetMouth( 0 );
		this->m_bShowMdl = true;
	}

	this->rotx = -90.0f;
	this->roty = 0.0f;
	this->rotz = -90.0f;

	this->panx = 0.0f;
	this->pany = 0.0f;
	this->panz = -20.0f;

	this->oldx = -1.0f;
	this->oldy = -1.0f;

	this->oldrx = -1.0f;
	this->oldry = -1.0f;
	this->oldrz = -1.0f;

	this->oldpx = -1.0f;
	this->oldpy = -1.0f;
	this->oldpz = -1.0f;

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}

void COpenGl::unload( void )
{
	tempmodel.FreeModel();
	this->m_bShowMdl = false;
}

void COpenGl::draw()
{
	static float prev;
	float curr;

	glViewport(0, 0, m_nWidth, m_nHeight);

	glClear( GL_COLOR_BUFFER_BIT );
	glClear( GL_DEPTH_BUFFER_BIT );
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0f,(GLfloat)m_nWidth/(GLfloat)m_nHeight,0.1f,300.0f);
	glPushMatrix();

	if ( m_nView == VIEWTYPE_MATERIAL )
	{
		drawmaterial();
		return;
	}

	glClearColor( settings.bgcolor[0], settings.bgcolor[1], settings.bgcolor[2], 0.0f );
    
	handlekeys();

	glTranslatef( panx, pany ,panz );
	glRotatef(rotx,1,0,0);
	glRotatef(roty,0,1,0);
	glRotatef(rotz,0,0,1);
	glScalef( 0.1f, 0.1f, 0.1f );

	setupRenderMode();

	glCullFace( GL_FRONT );
	if ( m_bShowMdl )
	{
		tempmodel.SetBlending( 0, 0.0 );
		tempmodel.SetBlending( 1, 0.0 );

		if ( m_bAnimate )
		{
			curr = GetTickCount( ) / 1000.0f;
			tempmodel.AdvanceFrame( (curr - prev)*m_fAnimSpeed );
			prev = curr;
		}

		glClear( GL_DEPTH_BUFFER_BIT );

		tempmodel.DrawModel();
		if ( settings.show_bones )
			tempmodel.DrawBones();
		if ( settings.show_hitbox )
			tempmodel.DrawHitboxes();
		if ( settings.show_atach )
			tempmodel.DrawAtachments();
	}
	glPopMatrix();
	
	SwapBuffers( m_hDC );
}

void COpenGl::drawmaterial()
{
	glClear( GL_COLOR_BUFFER_BIT );
	glClear( GL_DEPTH_BUFFER_BIT );
	glDisable (GL_DEPTH_TEST);
	glClearColor( settings.bgcolor[0], settings.bgcolor[1], settings.bgcolor[2], 0.0f );
    glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glOrtho (0.0f, (float) m_nWidth, (float) m_nHeight, 0.0f, 1.0f, -1.0f);

	if ( m_bShowMdl )
	{
		tempmodel.DrawUVMap( m_nMaterial, m_nWidth, m_nHeight );
	}
	SwapBuffers( m_hDC );
}

void COpenGl::handlekeys( void )
{
//////////////////////////////
	if ( m_bKeys[VK_UP] )
	{
		pany += 0.1f;
	}
	if ( m_bKeys[VK_DOWN] )
	{
		pany -= 0.1f;
	}
//////////////////////////////
	if ( m_bKeys[VK_RIGHT] )
	{
		panx += 0.1f;
	}
	if ( m_bKeys[VK_LEFT] )
	{
		panx -= 0.1f;
	}
//////////////////////////////
	if ( m_bKeys[0x05A] ) // Z
	{
		panz += 0.1f;
	}
	if ( m_bKeys[0x058] ) // X
	{
		panz -= 0.1f;
	}
//////////////////////////////
	if ( m_bKeys[0x044] ) // D
	{
		rotz += 0.8f;
	}
	if ( m_bKeys[0x041] ) // A
	{
		rotz -= 0.8f;
	}
//////////////////////////////
	if ( m_bKeys[0x053] ) // S
	{
		rotx += 0.8f;
	}
	if ( m_bKeys[0x057] ) // W
	{
		rotx -= 0.8f;
	}
//////////////////////////////
}

void COpenGl::rotate( float fRotX, float fRotY, float fRotZ )
{
	rotx += fRotX;
	roty += fRotY;
	rotz += fRotZ;
}

void COpenGl::move( float fTranX, float fTranY, float fTranZ )
{
	panx += fTranX;
	pany += fTranY;
	panz += fTranZ;
}

void COpenGl::setKey( UINT key, bool value )
{
	m_bKeys[key] = value;
}

void COpenGl::setupRenderMode( void )
{
	switch ( this->m_nRendermode )
	{
	case 0:
		glPointSize( 2 );

		glPolygonMode ( GL_FRONT_AND_BACK, GL_POINT );
		glDisable (GL_TEXTURE_2D);
		glEnable (GL_CULL_FACE);
		glEnable (GL_DEPTH_TEST);
		glShadeModel ( GL_SMOOTH );
		break;
	case 1:
		glLineWidth( 1 );

		glPolygonMode ( GL_FRONT_AND_BACK, GL_LINE );
		glDisable (GL_TEXTURE_2D);
		glEnable (GL_CULL_FACE);
		glEnable (GL_DEPTH_TEST);
		glShadeModel ( GL_SMOOTH );
		break;
	case 2:
		glPolygonMode ( GL_FRONT_AND_BACK, GL_FILL );
		glDisable (GL_TEXTURE_2D);
		glEnable (GL_CULL_FACE);
		glEnable (GL_DEPTH_TEST);
		glShadeModel ( GL_FLAT );
		break;
	case 3:
		glPolygonMode ( GL_FRONT_AND_BACK, GL_FILL );
		glDisable (GL_TEXTURE_2D);
		glEnable (GL_CULL_FACE);
		glEnable (GL_DEPTH_TEST);
		glShadeModel ( GL_SMOOTH );
		break;
	case 4:
		glPolygonMode ( GL_FRONT_AND_BACK, GL_FILL );
		glEnable (GL_TEXTURE_2D);
		glEnable (GL_CULL_FACE);
		glEnable (GL_DEPTH_TEST);
		glShadeModel ( GL_SMOOTH );
		break;
	}
}

void COpenGl::setRenderMode( int value )
{
	if ( value >= RENDERMODES )
	{
		this->m_nRendermode = 0;
		return;
	}
	this->m_nRendermode = value;
}

int COpenGl::getRenderMode( void )
{
	return this->m_nRendermode;
}

void COpenGl::resetView( int type )
{
	this->m_nView = type;

	switch ( type )
	{
	case VIEWTYPE_CENTER:
		this->rotx = -90.0f;
		this->roty = 0.0f;
		this->rotz = -90.0f;

		this->panx = 0.0f;
		this->pany = 0.0f;
		this->panz = -20.0f;

		this->oldx = -1.0f;
		this->oldy = -1.0f;

		this->oldrx = -1.0f;
		this->oldry = -1.0f;
		this->oldrz = -1.0f;

		this->oldpx = -1.0f;
		this->oldpy = -1.0f;
		this->oldpz = -1.0f;

		break;
	case VIEWTYPE_PLAYER:
		this->rotx = -90.0f;
		this->roty = 0.0f;
		this->rotz = 100.0f;

		this->panx = -0.2f;
		this->pany = 0.0f;
		this->panz = -0.7f;

		this->oldx = -1.0f;
		this->oldy = -1.0f;

		this->oldrx = -1.0f;
		this->oldry = -1.0f;
		this->oldrz = -1.0f;

		this->oldpx = -1.0f;
		this->oldpy = -1.0f;
		this->oldpz = -1.0f;
		break;
	}
}

void COpenGl::toggleRenderMode( void )
{
	m_nRendermode++;
	if ( m_nRendermode >= RENDERMODES )
		m_nRendermode = 0;
}

void COpenGl::sequenceFFW( void )
{
	tempmodel.SetSequence( tempmodel.GetSequence()+1 );
}

void COpenGl::sequenceRW( void )
{
	tempmodel.SetSequence( tempmodel.GetSequence()-1 );
}

void COpenGl::setSequence( int seq )
{
	tempmodel.SetSequence( seq );
}

void COpenGl::setBodygroup( int bodygr, int value )
{
 	tempmodel.SetBodygroup( bodygr, value );
}

void COpenGl::changeAnimspeed( float speed )
{
	m_fAnimSpeed += speed;
	if ( m_fAnimSpeed < 0 )
		m_fAnimSpeed = 0;
}

void COpenGl::setAnimspeed( float speed )
{
	m_fAnimSpeed = speed;
	if ( m_fAnimSpeed < 0 )
		m_fAnimSpeed = 0;
}

void COpenGl::setAnimate( bool animate )
{
	this->m_bAnimate = animate;
}

void COpenGl::setFrame( int frame )
{
	tempmodel.SetFrame( frame );
}

void COpenGl::setSelHitbox( int index )
{
	tempmodel.SetSelHitbox( index );
}

void COpenGl::setSelBone( int index )
{
	tempmodel.SetSelBone( index );
}

void COpenGl::setSlider( CSlider *pSlider )
{
	pSlider->setSel( (int)tempmodel.GetFrame() );
}

void COpenGl::setMaterial( int material )
{
	this->m_nMaterial = material;
}

void COpenGl::setSkin( int skin )
{
	tempmodel.SetSkin( skin );
}

float COpenGl::setMouth( float value )
{
	return tempmodel.SetMouth( value );
}

float COpenGl::setController( int index, float value )
{
	return tempmodel.SetController( index, value );
}

long COpenGl::getPolycount( void )
{
	return tempmodel.GetPolyCount();
}