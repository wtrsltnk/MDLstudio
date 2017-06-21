#include "CFrame.h"

/////////////////////////////////////
// Constructors / Destructors      //
/////////////////////////////////////
CFrame::CFrame( HINSTANCE hInstance, 
					 HWND hParent,
					 int nX, int nY , 
					 int nWidth ,int nHeight, int id, int optMax ) : CControl( hInstance, 
					 hParent,
					 nX, nY , 
					 nWidth ,nHeight, id )
{
	this->optMax = optMax;
	this->optCount = 0;
	m_hOptions = new HWND[ optMax ];
}

CFrame::CFrame( HINSTANCE hInstance, HWND hParent ) : CControl( hInstance, hParent )
{
}

/////////////////////////////////////
// Memberfuncties                  //
/////////////////////////////////////

void CFrame::create( void )
{
	m_hWnd = CreateWindowEx( WS_EX_WINDOWEDGE,
		 "BUTTON", 
		 "Caption", 
		 WS_CHILD | WS_VISIBLE |
		 BS_NOTIFY | BS_GROUPBOX, 
		 m_nX, 
		 m_nY, 
		 m_nWidth, 
		 m_nHeight, 
		 m_hParent, 
		 (HMENU)control_id, 
		 NULL, 
		 NULL );
}

void CFrame::create( DWORD ex_style )
{
	m_hWnd = CreateWindowEx( ex_style,
		 "BUTTON", 
		 "Caption", 
		 WS_CHILD | WS_VISIBLE |
		 BS_NOTIFY | BS_GROUPBOX, 
		 m_nX, 
		 m_nY, 
		 m_nWidth, 
		 m_nHeight, 
		 m_hParent, 
		 (HMENU)control_id, 
		 NULL, 
		 NULL );
}

void CFrame::create( DWORD ex_style, DWORD styles )
{
	m_hWnd = CreateWindowEx( ex_style,
		 "BUTTON", 
		 "Caption", 
		 WS_CHILD | WS_VISIBLE |
		 BS_NOTIFY | BS_GROUPBOX | styles, 
		 m_nX, 
		 m_nY, 
		 m_nWidth, 
		 m_nHeight, 
		 m_hParent, 
		 (HMENU)control_id, 
		 NULL, 
		 NULL );
}

void CFrame::addOption( string text, int nX, int nY, int nWidth, int nHeight )
{
	if ( optCount < 10 )
	{
		m_hOptions[ optCount++ ] = CreateWindowEx( WS_EX_WINDOWEDGE,
			 "BUTTON", 
			 text.c_str(), 
			 WS_CHILD | WS_VISIBLE |
			 BS_NOTIFY | BS_AUTORADIOBUTTON, 
			 nX, 
			 nY, 
			 nWidth, 
			 nHeight, 
			 m_hWnd, 
			 NULL, 
			 NULL, 
			 NULL );
	}
}

string CFrame::getOptionText( int index )
{
	if ( index > optCount )
		return "index out of bounds";

	int nlenght;
	char *result;

	result = new char;

	nlenght = SendMessage( m_hOptions[ index - 1 ], WM_GETTEXTLENGTH, 0, 0 );
	SendMessage( m_hOptions[ index - 1 ], WM_GETTEXT, ( WPARAM ) nlenght+1, ( LPARAM )( LPCTSTR ) result );
	
	return result;
}

long CFrame::setOptionText( int index, string text )
{
	if ( index > optCount )
		return 0;

	return  SendMessage( m_hOptions[ index - 1 ], WM_SETTEXT, 0, ( LPARAM ) ( LPTSTR ) text.c_str() );
}

int CFrame::getOptionValue( int index )
{
	return SendMessage( m_hOptions[ index - 1 ], BM_GETCHECK, 0, 0 );
}

void CFrame::setOptionValue( int index, int state )
{
	SendMessage( m_hOptions[ index - 1 ], BM_SETCHECK, ( WPARAM )state, 0 );
}

int CFrame::getSelOption( void )
{
	for ( int i = 0; i < optCount; i++ )
	{
		if ( getOptionValue( i - 1 ) == BST_CHECKED )
			return i;
	}
	return -1;
}