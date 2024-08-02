#ifndef COPENSAVEDLG_H
#define COPENSAVEDLG_H

#include <windows.h>

#include <shlobj.h>
#include <string>

class CCommonDialogs
{
public:
    CCommonDialogs(HINSTANCE, HWND);
    ~CCommonDialogs();

    std::string open(const char *, const char *, const char *);
    std::string save(const char *, const char *, const char *);
    std::string folder(const char *title);
    bool color(float color[3]);

private:
    HWND m_hOwner;
    HWND m_hWnd;
    HINSTANCE m_hInstance;
};

#endif
