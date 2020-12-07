#ifndef COPENSAVEDLG_H
#define COPENSAVEDLG_H

#include <shlobj.h>
#include <windows.h>

class CCommonDialogs
{
public:
    CCommonDialogs(
        HINSTANCE,
        HWND);

    ~CCommonDialogs();

    char *open(
        const char *title,
        const char *szFilter,
        const char *szDefExt);

    char *save(
        const char *title,
        const char *szFilter,
        const char *szDefExt);

    char *folder(
        const char *title);

    bool color(
        float color[3]);

private:
    HWND m_hOwner;
    HWND m_hWnd;
    HINSTANCE m_hInstance;
};

#endif
