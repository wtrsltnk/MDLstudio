#include "CSlider.h"

/////////////////////////////////////
// Constructors / Destructors      //
/////////////////////////////////////
CSlider::CSlider( HINSTANCE hInstance, 
					 HWND hParent,
					 int nX, int nY , 
					 int nWidth ,int nHeight, int id ) : CControl( hInstance, 
					 hParent,
					 nX, nY , 
					 nWidth ,nHeight, id )
{
}

CSlider::CSlider( HINSTANCE hInstance, HWND hParent ) : CControl( hInstance, hParent )
{
}

/////////////////////////////////////
// Memberfuncties                  //
/////////////////////////////////////

void CSlider::create( void )
{
	m_hWnd = CreateWindowEx( WS_EX_WINDOWEDGE,
		 TRACKBAR_CLASS, 
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

void CSlider::create( DWORD ex_style )
{
	m_hWnd = CreateWindowEx( ex_style,
		 TRACKBAR_CLASS, 
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

void CSlider::create( DWORD ex_style, DWORD styles )
{
	m_hWnd = CreateWindowEx( ex_style,
		 TRACKBAR_CLASS, 
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

void CSlider::init( int min, int max )
{
	SendMessage(m_hWnd, TBM_SETRANGE, 
        (WPARAM) TRUE,                   // redraw flag 
        (LPARAM) MAKELONG(min, max));  // min. & max. positions 

    SendMessage(m_hWnd, TBM_SETPAGESIZE, 
        0, (LPARAM) 4); 

    SendMessage(m_hWnd, TBM_SETPOS, 
        (WPARAM) TRUE,
        (LPARAM) 0); 
}

void CSlider::reset( void )
{
	SendMessage( m_hWnd, TVM_DELETEITEM, 0, (LPARAM) TVI_ROOT );
}

void CSlider::setSel( int sel )
{
	SendMessage( m_hWnd, TBM_SETPOS, (WPARAM) (BOOL) TRUE, (LPARAM) (LONG)sel );
}

int CSlider::getSel( void )
{
	return SendMessage( m_hWnd, TBM_GETPOS, (WPARAM)0, (LPARAM)0 );
}