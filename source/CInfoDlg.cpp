#include "CInfoDlg.h"

/////////////////////////////////////
// Constructors / Destructors      //
/////////////////////////////////////

CInfoDlg::CInfoDlg( HINSTANCE hInstance, HWND hWnd )
{
	this->m_hOwner		= hWnd;
	this->m_hInstance	= hInstance;
}

CInfoDlg::~CInfoDlg( void )
{
}

/////////////////////////////////////
// Memberfuncties                  //
/////////////////////////////////////

BOOL CInfoDlg::DlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
	case WM_LBUTTONUP:
		EndDialog( hDlg, IDCANCEL );
		break;
	}
	return FALSE;
}

void CInfoDlg::show( void )
{
	m_hWnd = CreateDialog( m_hInstance, MAKEINTRESOURCE( IDD_INFO ), m_hOwner, (DLGPROC)DlgProc );	
	ShowWindow( m_hWnd, SW_SHOW );
}
