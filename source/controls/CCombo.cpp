#include "CCombo.h"

/////////////////////////////////////
// Constructors / Destructors      //
/////////////////////////////////////
CCombo::CCombo(
    HINSTANCE hInstance,
    HWND hParent,
    int nX, int nY,
    int nWidth, int nHeight, int id)
    : CControl(
          hInstance,
          hParent,
          nX, nY,
          nWidth, nHeight, id)
{
}

CCombo::CCombo(
    HINSTANCE hInstance,
    HWND hParent)
    : CControl(
          hInstance,
          hParent)
{
}

/////////////////////////////////////
// Memberfuncties                  //
/////////////////////////////////////

void CCombo::create(void)
{
    m_hWnd = CreateWindowEx(
        WS_EX_WINDOWEDGE,
        "COMBOBOX",
        "Caption",
        WS_CHILD | WS_VISIBLE,
        m_nX,
        m_nY,
        m_nWidth,
        m_nHeight,
        m_hParent,
        (HMENU)control_id,
        NULL,
        NULL);
}

void CCombo::create(
    DWORD ex_style)
{
    m_hWnd = CreateWindowEx(
        ex_style,
        "COMBOBOX",
        "Caption",
        WS_CHILD | WS_VISIBLE,
        m_nX,
        m_nY,
        m_nWidth,
        m_nHeight,
        m_hParent,
        (HMENU)control_id,
        NULL,
        NULL);
}

void CCombo::create(
    DWORD ex_style,
    DWORD styles)
{
    m_hWnd = CreateWindowEx(
        ex_style,
        "COMBOBOX",
        "Caption",
        WS_CHILD | WS_VISIBLE | styles,
        m_nX,
        m_nY,
        m_nWidth,
        m_nHeight,
        m_hParent,
        (HMENU)control_id,
        NULL,
        NULL);
}

void CCombo::addItem(
    std::string text)
{
    SendMessage(m_hWnd, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)text.c_str());
}

bool CCombo::remItem(int index)
{
    if (SendMessage(m_hWnd, CB_DELETESTRING, (WPARAM)index - 1, 0) == 0)
        return true;
    return false;
}

std::string CCombo::getItem(
    int index)
{
    char szBuffer[255];

    SendMessage(m_hWnd, CB_GETLBTEXT, (WPARAM)index, (LPARAM)(LPCSTR)szBuffer);
    return szBuffer;
}

std::string CCombo::getSelected()
{
    return getItem(SendMessage(m_hWnd, CB_GETCURSEL, 0, 0));
}
