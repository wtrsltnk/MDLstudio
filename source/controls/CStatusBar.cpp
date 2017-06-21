#include "CStatusBar.h"


/////////////////////////////////////
// Constructors / Destructors      //
/////////////////////////////////////

CStatusBar::CStatusBar( HINSTANCE hInstance, HWND hParent, int id ) : CControl( hInstance, 
					 hParent,
					 0, 0 , 
					 0 ,0, id )
{
	numparts = 0;
}

CStatusBar::~CStatusBar( void )
{
}

/////////////////////////////////////
// Memberfuncties                  //
/////////////////////////////////////
/*
Create statusbar
*/
void CStatusBar::create( void )
{
	m_hWnd = CreateStatusWindow( WS_CHILD | WS_VISIBLE, 
		"", 
		m_hParent, 
		1 );

	SendMessage( m_hWnd, SB_SETPARTS, numparts, ( LPARAM )parts );
	SendMessage( m_hWnd, SB_SETMINHEIGHT, 16, 0 );
}

/*
Resize status bar
*/
void CStatusBar::resize( WPARAM wParam, LPARAM lParam )
{
	SendMessage( m_hWnd, WM_SIZE, wParam, lParam );
}

/*
Set Tekst in Item
*/
void CStatusBar::setItemText( string strText, int index )
{
	SendMessage( m_hWnd, SB_SETTEXT, ( WPARAM ) index-1|0, ( LPARAM )strText.c_str() );
}

/*
Add Item
*/
void CStatusBar::addItem( string strText, int width )
{
	if ( width < 0 )
	{
		parts[ numparts++ ] = -1;

		SendMessage( m_hWnd, SB_SETPARTS, numparts, ( LPARAM )parts );
		SendMessage( m_hWnd, SB_SETTEXT, ( WPARAM )numparts-1|0    , ( LPARAM )strText.c_str() );
	}
	else
	{
		for ( int i = 0; i < numparts; i++ )
			width += parts[ i ];

		parts[ numparts++ ] = width;

		SendMessage( m_hWnd, SB_SETPARTS, numparts, ( LPARAM )parts );
		SendMessage( m_hWnd, SB_SETTEXT, ( WPARAM )numparts-1|0    , ( LPARAM )strText.c_str() );
	}
}

/*
Add One Item
*/
void CStatusBar::addOneItem( string strText )
{
		parts[ 0 ] = -1;

		SendMessage( m_hWnd, SB_SETPARTS, 1, ( LPARAM )parts );
		SendMessage( m_hWnd, SB_SETTEXT, ( WPARAM )0|0, ( LPARAM )strText.c_str() );
}
