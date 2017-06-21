#include "CLabel.h"

/////////////////////////////////////
// Constructors / Destructors      //
/////////////////////////////////////
CLabel::CLabel( HINSTANCE hInstance, 
					 HWND hParent,
					 int nX, int nY , 
					 int nWidth ,int nHeight, int id ) : CControl( hInstance, 
					 hParent,
					 nX, nY , 
					 nWidth ,nHeight, id )
{
}

CLabel::CLabel( HINSTANCE hInstance, HWND hParent ) : CControl( hInstance, hParent )
{
}

/////////////////////////////////////
// Memberfuncties                  //
/////////////////////////////////////

void CLabel::create( void )
{
	m_hWnd = CreateWindowEx( WS_EX_WINDOWEDGE,
		 "STATIC", 
		 "Caption", 
		 WS_CHILD | WS_VISIBLE |
		 SS_NOPREFIX, 
		 m_nX, 
		 m_nY, 
		 m_nWidth, 
		 m_nHeight, 
		 m_hParent, 
		 (HMENU)control_id, 
		 NULL, 
		 NULL );
}

void CLabel::create( DWORD ex_style )
{
	m_hWnd = CreateWindowEx( ex_style,
		 "STATIC", 
		 "Caption", 
		 WS_CHILD | WS_VISIBLE |
		 SS_NOPREFIX, 
		 m_nX, 
		 m_nY, 
		 m_nWidth, 
		 m_nHeight, 
		 m_hParent, 
		 (HMENU)control_id, 
		 NULL, 
		 NULL );
}

void CLabel::create( DWORD ex_style, DWORD styles )
{
	m_hWnd = CreateWindowEx( ex_style,
		 "STATIC", 
		 "Caption", 
		 WS_CHILD | WS_VISIBLE |
		 SS_NOPREFIX | styles,
		 m_nX, 
		 m_nY, 
		 m_nWidth, 
		 m_nHeight, 
		 m_hParent, 
		 (HMENU)control_id, 
		 NULL, 
		 NULL );
}
