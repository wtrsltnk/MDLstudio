#ifndef CBROWSEDLG_H
#define CBROWSEDLG_H

#include <windows.h>
#include "..\resource\resource.h"
#include "controls\GUI.h"
#include "Settings.h"

class CBrowseDlg
{
private:
	HWND						m_hWnd;
	static HWND					m_hOwner;
	static HINSTANCE			m_hInstance;

	static CFolderView			*m_pFolder;
	static CFileView			*m_pFile;
public:
	CBrowseDlg( HINSTANCE, HWND );
	~CBrowseDlg( void );
	static BOOL DlgProc( HWND, UINT, WPARAM, LPARAM );

	void show( void );
};

#endif
