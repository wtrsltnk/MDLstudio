#ifndef CSEQUENCEDLG_H
#define CSEQUENCEDLG_H

#include <windows.h>
#include "..\resource\resource.h"

#include "..\hlsdk\mathlib.h"
#include "..\hlsdk\studio.h"
#include "..\hlsdk\studio_render.h"

class CSequenceDlg
{
private:
	static HWND			m_hOwner;
	HWND				m_hWnd;
	HINSTANCE			m_hInstance;

	static mstudioseqdesc_t	*m_pSeq;
	static mstudioevent_t	*m_pEvents;
public:
	CSequenceDlg( HINSTANCE, HWND);
	~CSequenceDlg( void );
	static BOOL DlgProc( HWND, UINT, WPARAM, LPARAM );

	void show( mstudioseqdesc_t *, mstudioevent_t *  );
};

#endif
