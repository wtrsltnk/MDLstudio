#ifndef COPENSAVEDLG_H
#define COPENSAVEDLG_H

#include <windows.h>

class COpenSaveDlg
{
public:
    COpenSaveDlg(HINSTANCE, HWND);
    ~COpenSaveDlg();

    char *open(char *, char *, char *);
    char *save(char *, char *, char *);

private:
    HWND m_hOwner;
    HWND m_hWnd;
    HINSTANCE m_hInstance;
};

#endif
