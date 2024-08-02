#include "CList.h"

/////////////////////////////////////
// Constructors / Destructors      //
/////////////////////////////////////
CList::CList(
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

CList::CList(
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

void CList::create()
{
    m_hWnd = CreateWindowEx(
        WS_EX_WINDOWEDGE,
        "LISTBOX",
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

void CList::create(
    DWORD ex_style)
{
    m_hWnd = CreateWindowEx(
        ex_style,
        "LISTBOX",
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

void CList::create(
    DWORD ex_style,
    DWORD styles)
{
    m_hWnd = CreateWindowEx(
        ex_style,
        "LISTBOX",
        "Caption",
        WS_CHILD | WS_VISIBLE |
            LBS_NOTIFY | styles,
        m_nX,
        m_nY,
        m_nWidth,
        m_nHeight,
        m_hParent,
        (HMENU)control_id,
        NULL,
        NULL);
}

void CList::addItem(
    std::string text)
{
    SendMessage(m_hWnd, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)text.c_str());
}

bool CList::remItem(
    int index)
{
    if (SendMessage(m_hWnd, LB_DELETESTRING, (WPARAM)index - 1, 0) == 0)
        return true;
    return false;
}

std::string CList::getItem(
    int index)
{
    char szBuffer[255];

    SendMessage(m_hWnd, LB_GETTEXT, (WPARAM)index, (LPARAM)(LPCSTR)szBuffer);
    return szBuffer;
}
