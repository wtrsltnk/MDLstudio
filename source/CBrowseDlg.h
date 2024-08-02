#ifndef CBROWSEDLG_H
#define CBROWSEDLG_H

#include <windows.h>

#include "..\resource\resource.h"
#include "controls\GUI.h"

class CBrowseDlg
{
public:
    CBrowseDlg(HINSTANCE, HWND);
    ~CBrowseDlg();

    static BOOL DlgProc(HWND, UINT, WPARAM, LPARAM);

    void show();

private:
    HWND m_hWnd;
    static HWND m_hOwner;
    static HINSTANCE m_hInstance;

    static CFolderView *m_pFolder;
    static CFileView *m_pFile;
};

#endif
