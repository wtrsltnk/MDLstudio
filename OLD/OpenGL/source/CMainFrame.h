#ifndef CMAINFRAME_H
#define CMAINFRAME_H

#include "CWin.h"
#include "COpenGL.h"
#include "..\resource\resource.h"

#include "controls\GUI.h"
#include "CInfoDlg.h"

#define TIMER_INTERVAL			20

class CMainFrame : public CWin
{
private:
	COpenGl			*m_pOpenGl;
	CStatusBar		*m_pStatus;
	CTreeView		*m_pModelView;
	CButton			*m_pHideButton;
	CButton			*m_pStart;
	CSlider			*m_pSlider;
	CSlider			*m_pSpeed;
	CSlider			*m_pController;
	CLabel			*m_pSlow;
	CLabel			*m_pFast;

	CInfoDlg		*m_pInfo;
	COpenSaveDlg	*m_pOpenSaveDlg;

	char			m_pAppPath[MAX_PATH];
	char			m_pSettings[MAX_PATH];

	int				m_iHideBut;
	int				m_iModelViewWidth;
	int				m_iGlX, m_iGlWidth;
	bool			m_bHidden;

	int				m_nWidth, m_nHeight;

	HTREEITEM		m_hReference;
	HTREEITEM		m_hControllers;
	HTREEITEM		m_hSkins;
	HTREEITEM		m_hSequence;
	HTREEITEM		m_hMaterial;
	HTREEITEM		m_hBones;
	HTREEITEM		m_hHitboxes;

	LPTSTR			lpCmdLine;
	LPTSTR			lpParameters;
	bool			bAnimate;
	int				m_nController;
public:
	CMainFrame();
	~CMainFrame();

	void load( char *filename ) throw();
	int getIndex( string str );
	char *GetParameters( char * cmdline );
	void GetAppPath( char *lpCmdLine, char *dest );

	LRESULT MsgProc(HWND, UINT, WPARAM, LPARAM);
};

#endif
