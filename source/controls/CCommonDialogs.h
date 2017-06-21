#ifndef COPENSAVEDLG_H
#define COPENSAVEDLG_H

#include <windows.h>
#include <shlobj.h>
#include <string>
using namespace std;

class CCommonDialogs
{
private:
	HWND				m_hOwner;
	HWND				m_hWnd;
	HINSTANCE			m_hInstance;

public:
	CCommonDialogs( HINSTANCE, HWND );
	~CCommonDialogs( void );

	char *open( char *, char *, char * );
	char *save( char *, char *, char * );
	char *folder( char *title );
	bool color( float color[3] );
};

#endif
