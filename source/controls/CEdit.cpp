#include "CEdit.h"

/////////////////////////////////////
// Constructors / Destructors      //
/////////////////////////////////////
CEdit::CEdit( HINSTANCE hInstance, 
					 HWND hParent,
					 int nX, int nY , 
					 int nWidth ,int nHeight, int id ) : CControl( hInstance, 
					 hParent,
					 nX, nY , 
					 nWidth ,nHeight, id )
{
}

CEdit::CEdit( HINSTANCE hInstance, HWND hParent ) : CControl( hInstance, hParent )
{
}

/////////////////////////////////////
// Memberfuncties                  //
/////////////////////////////////////

void CEdit::create( void )
{
	m_hWnd = CreateWindowEx( WS_EX_WINDOWEDGE,
		 "EDIT", 
		 "Caption", 
		 WS_CHILD | WS_VISIBLE, 
		 m_nX, 
		 m_nY, 
		 m_nWidth, 
		 m_nHeight, 
		 m_hParent, 
		 (HMENU)control_id, 
		 NULL, 
		 NULL );
}

void CEdit::create( DWORD ex_style )
{
	m_hWnd = CreateWindowEx( ex_style,
		 "EDIT", 
		 "Caption", 
		 WS_CHILD | WS_VISIBLE, 
		 m_nX, 
		 m_nY, 
		 m_nWidth, 
		 m_nHeight, 
		 m_hParent, 
		 (HMENU)control_id, 
		 NULL, 
		 NULL );
}

void CEdit::create( DWORD ex_style, DWORD styles )
{
	m_hWnd = CreateWindowEx( ex_style,
		 "EDIT", 
		 "Caption", 
		 WS_CHILD | WS_VISIBLE | styles, 
		 m_nX, 
		 m_nY, 
		 m_nWidth, 
		 m_nHeight, 
		 m_hParent, 
		 (HMENU)control_id, 
		 NULL, 
		 NULL );
}
