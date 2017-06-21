#ifndef CMATERIALDLG_H
#define CMATERIALDLG_H

#include <windows.h>
#include "..\resource\resource.h"

#include "..\hlsdk\mathlib.h"
#include "..\hlsdk\studio.h"
#include "..\hlsdk\studio_render.h"

class CMaterialDlg
{
private:
	static HWND			m_hOwner;
	HWND				m_hWnd;
	HINSTANCE			m_hInstance;

	static mstudiotexture_t	*m_pTexture;
public:
	CMaterialDlg( HINSTANCE, HWND);
	~CMaterialDlg( void );
	static BOOL DlgProc( HWND, UINT, WPARAM, LPARAM );

	void show( mstudiotexture_t *  );
};

#endif
