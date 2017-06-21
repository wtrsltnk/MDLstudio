#ifndef CCONTROL_H
#define CCONTROL_H

#include <windows.h>
#include <commctrl.h>
#include <string>
using namespace std;

#define STYLE_FLAT			WS_EX_WINDOWEDGE
#define STYLE_3D			WS_EX_CLIENTEDGE
#define STYLE_BORDER		WS_BORDER

class CControl
{
protected:
	HINSTANCE		m_hInstance;
	HWND			m_hWnd, m_hParent;
	int				m_nX, m_nY, m_nWidth , m_nHeight;
	int				control_id;
public:
	CControl( HINSTANCE, HWND, int, int, int, int, int );
	CControl( HINSTANCE, HWND );
	~CControl( void );
	void resize( int, int, int ,int );

	const int getID();

	void setFont( HFONT );
	HFONT getFont( void );

	void setText( string );
	void addText( string );
	string getText( void );

	HWND getHandle( void ) const {return m_hWnd;}
	void setFocus( void );
	void hide( bool hide );
};
#endif
