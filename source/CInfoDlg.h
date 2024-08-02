#ifndef CINFODLG_H
#define CINFODLG_H

#include <windows.h>

#include "..\resource\resource.h"

class CInfoDlg
{
public:
    CInfoDlg(HINSTANCE, HWND);
    ~CInfoDlg();

    static BOOL DlgProc(HWND, UINT, WPARAM, LPARAM);

    void show();

private:
    HWND m_hOwner;
    HWND m_hWnd;
    HINSTANCE m_hInstance;
};

#endif
