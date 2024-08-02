#ifndef CMAINFRAME_H
#define CMAINFRAME_H

#include "CBrowseDlg.h"
#include "CCManager.h"
#include "CInfoDlg.h"
#include "CMaterialDlg.h"
#include "CSequenceDlg.h"
#include "CWin.h"

#include "..\hlsdk\studio_render.h"

#define TIMER_INTERVAL 1
typedef unsigned char byte;

class CMainFrame : public CWin
{
public:
    CMainFrame();
    ~CMainFrame();

    LRESULT MsgProc(HWND, UINT, WPARAM, LPARAM);

    char *GetParameters(char *cmdline);
    void GetAppPath(char *lpCmdLine, char *dest);

    void open(void);
    void save(void);
    void saveas(void);
    void load(char *filename);
    void setTitle(char *filename);

    void CreateMenus();

private:
    CCManager *m_pControls;
    CCommonDialogs *m_pCommonDlg;
    CInfoDlg *m_pInfo;
    CMaterialDlg *m_pMaterial;
    CSequenceDlg *m_pSequence;
    CBrowseDlg *m_pBrowse;
    static StudioModel m_Model;

    char m_pAppPath[MAX_PATH];
    char m_pSettings[MAX_PATH];
    char m_pFileName[MAX_PATH];
    LPTSTR lpCmdLine;
    LPTSTR lpParameters;

    bool loaded;
    bool browsed;

    HMENU m_hReferences;
    HMENU m_hSequences;
    HMENU m_hSkins;
    HMENU m_hControllers;
    HMENU m_hBones;
    HMENU m_hMaterials;
    HMENU m_hHitboxes;
};

#endif
