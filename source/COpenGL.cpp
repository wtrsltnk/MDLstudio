#include "COpenGL.h"
#include "Settings.h"
#include <windows.h>

vec3_t g_vright = { 100, 100, 50 };		// needs to be set to viewer's right in order for chrome to work
float		g_lambert = 1.5;

static COpenGl * g_pCWin		= NULL;

//////////////////////////////////////////////////////////////////
// Koppeling WIN32 -> Klasse
//////////////////////////////////////////////////////////////////
LRESULT CALLBACK GLProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return g_pCWin->MsgProc(hWnd, uMsg, wParam, lParam);
}


COpenGl::COpenGl( HINSTANCE hInstance, 
				 HWND hParent, 
				 int nX, int nY, 
				 int nWidth, int nHeight, 
				 int nId ) : CControl( hInstance, hParent, nX, nY , nWidth ,nHeight, nId )
{
	g_pCWin = this;

	this->control_id = nId;
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

	this->m_nRendermode = RENDERMODE_TEXTURED;
	this->m_fAnimSpeed = 1.0f;

	for ( int i = 0; i < 255; i++ )
		m_bKeys[i] = false;

	this->m_bAnimate = true;

	this->m_nMaterial = 0;

	this->frame	 = 0;
	this->time	 = 0;
	this->timebase = 0;
	this->LastReading = 0.0;
}

COpenGl::~COpenGl()
{
    purge();
}

LRESULT COpenGl::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	float x, y;
	DWORD fwKeys;
	
	switch ( uMsg )
	{
	case WM_CREATE:
		break;

	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_LBUTTONDOWN:
		oldx = LOWORD(lParam);
		oldy = HIWORD(lParam);
		oldpx = panx;
		oldpy = pany;
		oldpz = panz;
		oldrx = rotx;
		oldry = roty;
		oldrz = rotz;
		SetCursor( LoadCursor( m_hInstance, MAKEINTRESOURCE(IDC_HAND) ) );
		break;

	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
	case WM_LBUTTONUP:
		SetCursor( LoadCursor( NULL, IDC_ARROW ) );
		break;

	case WM_MOUSEMOVE:
		fwKeys = wParam;
		x = LOWORD(lParam);
		y = HIWORD(lParam);

		switch (fwKeys)
		{
		case MK_LBUTTON:
			rotz = oldrz - ( oldx - x );
			rotx = oldrx - ( oldy - y );
			SetCursor( LoadCursor( m_hInstance, MAKEINTRESOURCE(IDC_HAND) ) );
			break;
		case MK_RBUTTON:
			panz = oldpz + ( ( oldy - y ) / 5 );
			SetCursor( LoadCursor( m_hInstance, MAKEINTRESOURCE(IDC_HAND) ) );
			break;
		case MK_MBUTTON:
			panx = oldpx - ( ( oldx - x ) / 20 );
			pany = oldpy + ( ( oldy - y ) / 20 );
			SetCursor( LoadCursor( m_hInstance, MAKEINTRESOURCE(IDC_HAND) ) );
			break;
		case MK_RBUTTON|MK_SHIFT:
			panx = oldpx - ( ( oldx - x ) / 20 );
			pany = oldpy + ( ( oldy - y ) / 20 );
			SetCursor( LoadCursor( m_hInstance, MAKEINTRESOURCE(IDC_HAND) ) );
			break;
		case MK_LBUTTON|MK_SHIFT:
			roty = oldry - ( oldx - x );
			SetCursor( LoadCursor( m_hInstance, MAKEINTRESOURCE(IDC_HAND) ) );
			break;
		default:
			break;
		}
		break;
	}
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

void COpenGl::create()
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 
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

	::RegisterClassEx(&wcex);


	m_hWnd = CreateWindowEx( WS_EX_STATICEDGE, "OPENGL",
		"OpenGl Window",
		WS_VISIBLE | WS_CHILD,
		m_nX, m_nY,
		m_nWidth, m_nHeight,
		m_hParent,
		(HMENU)control_id,
		m_hInstance,
		NULL );

    m_hDC = GetDC( m_hWnd );
	setPixelFormat();

    m_hRC = wglCreateContext( m_hDC );
    wglMakeCurrent( m_hDC, m_hRC );

    gladLoadGL();

    initfont();

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







// OPENGL FUNCTIES
bool COpenGl::init( StudioModel *pModel )
{
	if ( pModel == NULL )
		return false;

	this->m_pModel = pModel;

	m_pModel->SetSelHitbox( -1 );
	m_pModel->SetSelBone( -1 );
	m_pModel->SetSequence( 0 );
	m_pModel->SetController( 0, 0.0 );
	m_pModel->SetController( 1, 0.0 );
	m_pModel->SetController( 2, 0.0 );
	m_pModel->SetController( 3, 0.0 );
	m_pModel->SetMouth( 0 );

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

	return true;
}

void COpenGl::initfont( void )
{
	font = glGenLists( 255 );

    HFONT verdana = CreateFont( 12, 0, 0, 0, FW_MEDIUM, FALSE,
                    FALSE, FALSE, DEFAULT_CHARSET, OUT_TT_PRECIS,
                    CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
                    FF_DONTCARE | DEFAULT_PITCH, "Verdana" );

    SelectObject( m_hDC, verdana );

    wglUseFontBitmaps( m_hDC, 1, 255, font );
}

void COpenGl::draw()
{
	static float prev;
	float curr;

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glClearColor( settings.bgcolor[0], settings.bgcolor[1], settings.bgcolor[2], 0.0f );

	setupRenderMode();
	setupViewMode();

	m_pModel->SetBlending( 0, 0.0 );
	m_pModel->SetBlending( 1, 0.0 );

	if ( settings.animate )
	{
		curr = GetTickCount( ) / 1000.0f;
		m_pModel->AdvanceFrame( (curr - prev)*settings.speed );
		prev = curr;
	}

	m_pModel->DrawModel();
	if ( settings.show_grid && settings.viewmode != VIEWMODE_PERSPECTIVE )
		drawgrid();
	if ( settings.show_bones )
		m_pModel->DrawBones();
	if ( settings.show_hitbox )
		m_pModel->DrawHitboxes();
	if ( settings.show_atach )
		m_pModel->DrawAtachments();
	if ( settings.show_polys )
		drawtext( 10, 10, font, "Polygons : %i", m_pModel->GetPolyCount() );
	if ( settings.show_fps )
		showFPS();

	glPopMatrix();
	
	SwapBuffers( m_hDC );
}

void COpenGl::drawmaterial()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glDisable (GL_DEPTH_TEST);
	
	glClearColor( settings.bgcolor[0], settings.bgcolor[1], settings.bgcolor[2], 1.0f );
    
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	

	glOrtho (0.0f, (float) m_nWidth*panz/10, (float) m_nHeight*panz/10, 0.0f, 1.0f, -1.0f);

	m_pModel->DrawMaterial( m_nMaterial, m_nWidth*panz/10, m_nHeight*panz/10 );

	glPopMatrix ();

	SwapBuffers( m_hDC );
}

void COpenGl::drawtext( int x, int y, unsigned font, const char *format, ...)
{
    va_list argp;
    char text[256];

    va_start( argp, format );
    vsprintf( text, format, argp );
    va_end( argp );

	for ( int i = 0; i < strlen(text); i++ )
	{
		text[i] -= 1;
	}
    glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
    glPushMatrix();

    glOrtho(0.0, (GLdouble) m_nWidth, 0.0, (GLdouble) m_nHeight, 1.0f, -1.0f);

    glDisable(GL_TEXTURE_2D);
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
    glRasterPos2i( x, y );
    glListBase( font );
    glCallLists( strlen( text ), GL_BYTE, text );

    glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
    glPopMatrix();
}

void COpenGl::drawgrid( void )
{
	int linespacing;
	int temppan;

	temppan = panz;
	if ( panz < 0 )
		temppan = -panz;

	if ( temppan < 50 )
		linespacing = 2;
	else if ( temppan >= 50 && temppan < 100 )
		linespacing = 4;
	else if ( temppan >= 100 && temppan < 220 )
		linespacing = 8;
	else if ( temppan >= 220 && temppan < 400 )
		linespacing = 16;
	else if ( temppan >= 400 && temppan < 600 )
		linespacing = 24;
	else if ( temppan >= 600 && temppan < 800 )
		linespacing = 32;
	else if ( temppan >= 800 )
		linespacing = 40;


    glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
	glLineWidth (1.0f);

	for ( int i = -50; i < 50; i++ )
	{
		if ( (i%4)==0 )
			glLineWidth (2.0f);
		else
			glLineWidth (1.0f);
		glBegin(GL_LINES);
		switch ( settings.viewmode )
		{
		case VIEWMODE_LEFT:
		case VIEWMODE_RIGHT:
			glVertex3f( -m_nWidth*(linespacing/2), -50, i*linespacing );
			glVertex3f( m_nWidth*(linespacing/2), -50, i*linespacing );
			glVertex3f( i*linespacing, -50, -m_nWidth*(linespacing/2) );
			glVertex3f( i*linespacing, -50,  m_nWidth*(linespacing/2) );
			break;
		case VIEWMODE_TOP:
		case VIEWMODE_BOTTOM:
			glVertex3f( -m_nWidth*(linespacing/2), i*linespacing, -50 );
			glVertex3f( m_nWidth*(linespacing/2), i*linespacing, -50 );
			glVertex3f( i*linespacing, -m_nWidth*(linespacing/2), -50 );
			glVertex3f(i *linespacing,  m_nWidth*(linespacing/2), -50 );
			break;
		case VIEWMODE_FRONT:
		case VIEWMODE_BACK:
			glVertex3f( -50, -m_nWidth*(linespacing/2), i*linespacing );
			glVertex3f( -50, m_nWidth*(linespacing/2), i*linespacing );
			glVertex3f( -50,i*linespacing, -m_nWidth*(linespacing/2) );
			glVertex3f( -50,i*linespacing, m_nWidth*(linespacing/2) );
			break;
		}
		glEnd();
	}
}

void COpenGl::clear( void )
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glClearColor( settings.bgcolor[0], settings.bgcolor[1], settings.bgcolor[2], 0.0f );
	SwapBuffers( m_hDC );
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

void COpenGl::centerview( void )
{
	this->rotx = -90.0f;
	this->roty = 0.0f;
	this->rotz = -90.0f;

	this->panx = 0.0f;
	this->pany = 0.0f;
	this->panz = -20.0f;
}

void COpenGl::vmodelview( void )
{
	this->rotx = -90.0f;
	this->roty = 0.0f;
	this->rotz = 100.0f;

	this->panx = 0.0f;
	this->pany = 0.0f;
	this->panz = -1.0f;
}

void COpenGl::setKey( UINT key, bool value )
{
	m_bKeys[key] = value;
}

void COpenGl::showFPS( void )
{
	frame++;
	time = GetTickCount();
	
	if (time - timebase > 1000)			
	{
		LastReading = frame * 1000.0/(time - timebase);

		timebase = time;
		frame = 0;
	}
	drawtext( 10, m_nHeight-20, font, "%4.2f fps", LastReading );
}

void COpenGl::setupRenderMode( void )
{
	switch ( settings.rendermode )
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
		glDisable (GL_DEPTH_TEST);
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
		glDisable (GL_CULL_FACE);
		glEnable (GL_DEPTH_TEST);
		glShadeModel ( GL_SMOOTH );
		break;
	}
}

void perspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
    const GLdouble pi = 3.1415926535897932384626433832795;
    GLdouble fW, fH;

    //fH = tan( (fovY / 2) / 180 * pi ) * zNear;
    fH = tan(fovY / 360 * pi) * zNear;
    fW = fH * aspect;

    glFrustum(-fW, fW, -fH, fH, zNear, zFar);
}

void COpenGl::setupViewMode( void )
{
	float ratio;
		
	ratio = (float)m_nHeight/(float)m_nWidth;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); glPushMatrix ();

	switch ( settings.viewmode )
	{
	case VIEWMODE_LEFT:
		if ( panz<0 )
			glOrtho (-panz+(panx*4), panz+(panx*4), -panz*ratio+(pany*4), panz*ratio+(pany*4), -200.0f, 200.0f);
		else
			glOrtho (panz+(panx*4), -panz+(panx*4), panz*ratio+(pany*4), -panz*ratio+(pany*4), -200.0f, 200.0f);

		glRotatef(90.0f,1,0,0);
		glRotatef(180.0f,0,0,1);
		break;


	case VIEWMODE_RIGHT:
		if ( panz<0 )
			glOrtho (-panz+(panx*4), panz+(panx*4), -panz*ratio+(pany*4), panz*ratio+(pany*4), -200.0f, 200.0f);
		else
			glOrtho (panz+(panx*4), -panz+(panx*4), panz*ratio+(pany*4), -panz*ratio+(pany*4), -200.0f, 200.0f);

		glRotatef(90.0f,1,0,0);
		glRotatef(0.0f,0,0,1);
		break;

	case VIEWMODE_TOP:
		if ( panz<0 )
			glOrtho (-panz+(panx*4), panz+(panx*4), -panz*ratio+(pany*4), panz*ratio+(pany*4), -200.0f, 200.0f);
		else
			glOrtho (panz+(panx*4), -panz+(panx*4), panz*ratio+(pany*4), -panz*ratio+(pany*4), -200.0f, 200.0f);

		glRotatef(0.0f,1,0,0);
		glRotatef(90.0f,0,0,1);
		break;


	case VIEWMODE_BOTTOM:
		if ( panz<0 )
			glOrtho (-panz+(panx*4), panz+(panx*4), -panz*ratio+(pany*4), panz*ratio+(pany*4), -200.0f, 200.0f);
		else
			glOrtho (panz+(panx*4), -panz+(panx*4), panz*ratio+(pany*4), -panz*ratio+(pany*4), -200.0f, 200.0f);

		glRotatef(180.0f,1,0,0);
		glRotatef(90.0f,0,0,1);
		break;


	case VIEWMODE_FRONT:
		if ( panz<0 )
			glOrtho (-panz+(panx*4), panz+(panx*4), -panz*ratio+(pany*4), panz*ratio+(pany*4), -200.0f, 200.0f);
		else
			glOrtho (panz+(panx*4), -panz+(panx*4), panz*ratio+(pany*4), -panz*ratio+(pany*4), -200.0f, 200.0f);

		glRotatef(90.0f,1,0,0);
		glRotatef(90.0f,0,0,1);
		break;


	case VIEWMODE_BACK:
		if ( panz<0 )
			glOrtho (-panz+(panx*4), panz+(panx*4), -panz*ratio+(pany*4), panz*ratio+(pany*4), -200.0f, 200.0f);
		else
			glOrtho (panz+(panx*4), -panz+(panx*4), panz*ratio+(pany*4), -panz*ratio+(pany*4), -200.0f, 200.0f);

		glRotatef(90.0f,1,0,0);
		glRotatef(-90.0f,0,0,1);
		break;
		

	case VIEWMODE_PERSPECTIVE:
        perspectiveGL(45.0f, (GLfloat) m_nWidth / (GLfloat) m_nHeight, 0.5f, 300.0f);
        glTranslatef(panx, pany, panz);
        glRotatef(rotx,1,0,0);
		glRotatef(roty,0,1,0);
		glRotatef(rotz,0,0,1);
		glScalef( 0.1f, 0.1f, 0.1f );
		break;
	}
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

void COpenGl::setMaterial( int material )
{
	this->m_nMaterial = material;
}
