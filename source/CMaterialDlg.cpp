#include "CMaterialDlg.h"
#include <stdio.h>


mstudiotexture_t *CMaterialDlg::m_pTexture = NULL;
HWND CMaterialDlg::m_hOwner = NULL;
/////////////////////////////////////
// Constructors / Destructors      //
/////////////////////////////////////

CMaterialDlg::CMaterialDlg( HINSTANCE hInstance, HWND hWnd )
{
	this->m_hOwner		= hWnd;
	this->m_hInstance	= hInstance;
}

CMaterialDlg::~CMaterialDlg( void )
{
}

/////////////////////////////////////
// Memberfuncties                  //
/////////////////////////////////////

BOOL CMaterialDlg::DlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	char str[255];
	switch( uMsg )
	{
	case WM_INITDIALOG:
		sprintf( str, "%i", m_pTexture->height );
		SetDlgItemText( hDlg, IDC_MAT_HEIGHT, str );
		sprintf( str, "%i", m_pTexture->width );
		SetDlgItemText( hDlg, IDC_MAT_WIDTH, str );
		sprintf( str, "%s", m_pTexture->name );
		SetDlgItemText( hDlg, IDC_MAT_NAME, str );
		if ( m_pTexture->flags & STUDIO_NF_CHROME )
			SendMessage( GetDlgItem( hDlg, IDC_MAT_CHROME ), BM_SETCHECK, BST_CHECKED, 0 );
		if ( m_pTexture->flags & STUDIO_NF_ADDITIVE )
			SendMessage( GetDlgItem( hDlg, IDC_MAT_ADDITIVE ), BM_SETCHECK, BST_CHECKED, 0 );
		if ( m_pTexture->flags & STUDIO_NF_TRANSPARANT )
			SendMessage( GetDlgItem( hDlg, IDC_MAT_TRANSPARANT ), BM_SETCHECK, BST_CHECKED, 0 );
		break;

	case WM_COMMAND:
		switch ( LOWORD(wParam) )
		{
		case IDOK:
			m_pTexture->flags = 0;
			if ( SendMessage( GetDlgItem( hDlg, IDC_MAT_CHROME ), BM_GETCHECK, 0, 0 ) )
				m_pTexture->flags = m_pTexture->flags | STUDIO_NF_CHROME;
			if ( SendMessage( GetDlgItem( hDlg, IDC_MAT_ADDITIVE ), BM_GETCHECK, 0, 0 ) )
				m_pTexture->flags = m_pTexture->flags | STUDIO_NF_ADDITIVE;
			if ( SendMessage( GetDlgItem( hDlg, IDC_MAT_TRANSPARANT ), BM_GETCHECK, 0, 0 ) )
				m_pTexture->flags = m_pTexture->flags | STUDIO_NF_TRANSPARANT;
			EndDialog( hDlg, IDOK );
			break;

		case IDCANCEL:
			EndDialog( hDlg, IDCANCEL );
			break;

		case IDC_MAT_EXPORT:
			SendMessage( m_hOwner, WM_COMMAND, MAKELONG(IDM_MAT_EXPORT, 0), 0 );
			break;

		case IDC_MAT_IMPORT:
			SendMessage( m_hOwner, WM_COMMAND, MAKELONG(IDM_MAT_IMPORT, 0), 0 );
			break;
		}
		break;

	case WM_CLOSE:
		EndDialog( hDlg, ID_CLOSE );
		break;
	}
	return FALSE;
}

void CMaterialDlg::show( mstudiotexture_t *pTexture )
{
	this->m_pTexture = pTexture;
	DialogBox( m_hInstance, MAKEINTRESOURCE( IDD_MATERIAL ), m_hOwner, (DLGPROC)DlgProc );	
}
