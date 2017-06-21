#ifndef CINFODLG_H
#define CINFODLG_H

#include <windows.h>
#include "..\resource\resource.h"

class CInfoDlg
{
private:
	HWND				m_hOwner;
	HWND				m_hWnd;
	HINSTANCE			m_hInstance;
public:
	CInfoDlg( HINSTANCE, HWND );
	~CInfoDlg( void );
	static BOOL DlgProc( HWND, UINT, WPARAM, LPARAM );

	void show( void );
};

#endif
