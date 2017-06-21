#ifndef COPENSAVEDLG_H
#define COPENSAVEDLG_H

#include <windows.h>
#include <string>
using namespace std;

class COpenSaveDlg
{
private:
	HWND				m_hOwner;
	HWND				m_hWnd;
	HINSTANCE			m_hInstance;

public:
	COpenSaveDlg( HINSTANCE, HWND );
	~COpenSaveDlg( void );

	char *open( char *, char *, char * );
	char *save( char *, char *, char * );
};

#endif
