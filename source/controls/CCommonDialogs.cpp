#include "CCommonDialogs.h"

/////////////////////////////////////
// Constructors / Destructors      //
/////////////////////////////////////

CCommonDialogs::CCommonDialogs( HINSTANCE hInstance, HWND hWnd )
{
	this->m_hOwner		= hWnd;
	this->m_hInstance	= hInstance;
}

CCommonDialogs::~CCommonDialogs( void )
{
}

/////////////////////////////////////
// Memberfuncties                  //
/////////////////////////////////////

char *CCommonDialogs::open( char *title, char *szFilter, char *szDefExt )
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
	return NULL;
}

char *CCommonDialogs::save( char *title, char *szFilter, char *szDefExt )
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
	return NULL;
}

char *CCommonDialogs::folder( char *title )
{
	BROWSEINFO bi;
    ITEMIDLIST *pidl;
    char mydir[MAX_PATH];
    char *result;

    bi.hwndOwner = m_hOwner;
    bi.pidlRoot = 0;
    bi.pszDisplayName = mydir;
    bi.lpszTitle = title;
    bi.ulFlags = BIF_RETURNONLYFSDIRS;
    bi.lpfn = 0;
    bi.lParam = 0;

    //Call the directory browse dialog and assign it to a ITEMIDLIST
    pidl=SHBrowseForFolder(&bi);

    //Return the selected path
    if ( SHGetPathFromIDList(pidl, mydir) )
	{
		result = new char[strlen( mydir )+1];
		for ( int i = 0; i < strlen( mydir ); i++ )
		{
			result[i] = mydir[i];
			if ( mydir[i] == '\\' )
				result[i] = '/';
		}
        result[strlen( mydir )]='\0';
		return result;
	}
	
	return NULL;
}

bool CCommonDialogs::color( float color[3] )
{
	CHOOSECOLOR cc;  
	static COLORREF custColors[16];
	
	cc.lStructSize = sizeof (CHOOSECOLOR);
	cc.hwndOwner = m_hOwner;
	cc.rgbResult = RGB (color[0], color[1], color[2]);
	cc.lpCustColors = custColors;
	cc.Flags = CC_ANYCOLOR | CC_FULLOPEN | CC_RGBINIT;

	if (ChooseColor (&cc))
	{
		color[0] = (float) GetRValue (cc.rgbResult) / 255;
		color[1] = (float) GetGValue (cc.rgbResult) / 255;
		color[2] = (float) GetBValue (cc.rgbResult) / 255;
		return true;
	}
	return false;
}
