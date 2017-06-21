#include "COpenSaveDlg.h"

/////////////////////////////////////
// Constructors / Destructors      //
/////////////////////////////////////

COpenSaveDlg::COpenSaveDlg( HINSTANCE hInstance, HWND hWnd )
{
	this->m_hOwner		= hWnd;
	this->m_hInstance	= hInstance;
}

COpenSaveDlg::~COpenSaveDlg( void )
{
}

/////////////////////////////////////
// Memberfuncties                  //
/////////////////////////////////////

char *COpenSaveDlg::open( char *title, char *szFilter, char *szDefExt )
{
	char *result;
	OPENFILENAME ofn = {0};
	char szFile[MAX_PATH+1] =	{0};
	
	//specificeer de dialog box zo volledig mogelijk
	ofn.Flags             = OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT;
	ofn.hwndOwner         = m_hOwner;
	ofn.lpstrCustomFilter = 0;
	ofn.lpstrFile         = szFile;
	ofn.lpstrFileTitle    = 0;
	ofn.lpstrFilter       = szFilter;
	ofn.lpstrDefExt		  = szDefExt;
	ofn.lpstrInitialDir   = 0;
	ofn.lpstrTitle        = title;
	ofn.lStructSize       = sizeof( OPENFILENAME );
	ofn.nMaxFile          = MAX_PATH;


	//open de dialoog
	if( GetOpenFileName( &ofn ) != 0 )
	{
		result = new char[strlen( ofn.lpstrFile )+1];
		for ( int i = 0; i < strlen( ofn.lpstrFile ); i++ )
		{
			result[i] = ofn.lpstrFile[i];
			if ( ofn.lpstrFile[i] == '\\' )
				result[i] = '/';
		}
        result[strlen( ofn.lpstrFile )]='\0';
		return result;
	}
	//als het openen van de dialoog niet gelukt is, geeft een lege string terug
	return "";
}

char *COpenSaveDlg::save( char *title, char *szFilter, char *szDefExt )
{
	char *result;
	OPENFILENAME ofn = {0};
	char szFile[MAX_PATH+1] =	{0};
	
	//specificeer de dialog box zo volledig mogelijk
	ofn.Flags             = OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT;
	ofn.hwndOwner         = m_hOwner;
	ofn.lpstrCustomFilter = 0;
	ofn.lpstrFile         = szFile;
	ofn.lpstrFileTitle    = 0;
	ofn.lpstrFilter       = szFilter;
	ofn.lpstrDefExt		  = szDefExt;
	ofn.lpstrInitialDir   = 0;
	ofn.lpstrTitle        = title;
	ofn.lStructSize       = sizeof( OPENFILENAME );
	ofn.nMaxFile          = MAX_PATH;


	//open de dialoog
	if( GetSaveFileName( &ofn ) != 0 )
	{
		result = new char[strlen( ofn.lpstrFile )+1];
		for ( int i = 0; i < strlen( ofn.lpstrFile ); i++ )
		{
			result[i] = ofn.lpstrFile[i];
			if ( ofn.lpstrFile[i] == '\\' )
				result[i] = '/';
		}
        result[strlen( ofn.lpstrFile )]='\0';
		return result;
	}
	//als het openen van de dialoog niet gelukt is, geeft een lege string terug
	return "";
}
