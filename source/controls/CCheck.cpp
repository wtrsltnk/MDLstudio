#include "CCheck.h"

/////////////////////////////////////
// Constructors / Destructors      //
/////////////////////////////////////
CCheck::CCheck(
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

CCheck::CCheck(
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

void CCheck::create()
{
    m_hWnd = CreateWindowEx(
        WS_EX_WINDOWEDGE,
        "BUTTON",
        "Caption",
        WS_CHILD | WS_VISIBLE |
            BS_NOTIFY | BS_AUTOCHECKBOX,
        m_nX,
        m_nY,
        m_nWidth,
        m_nHeight,
        m_hParent,
        (HMENU)control_id,
        NULL,
        NULL);
}

void CCheck::create(
    DWORD ex_style)
{
    m_hWnd = CreateWindowEx(
        ex_style,
        "BUTTON",
        "Caption",
        WS_CHILD | WS_VISIBLE |
            BS_NOTIFY | BS_AUTOCHECKBOX,
        m_nX,
        m_nY,
        m_nWidth,
        m_nHeight,
        m_hParent,
        (HMENU)control_id,
        NULL,
        NULL);
}

void CCheck::create(
    DWORD ex_style,
    DWORD styles)
{
    m_hWnd = CreateWindowEx(
        ex_style,
        "BUTTON",
        "Caption",
        WS_CHILD | WS_VISIBLE |
            BS_NOTIFY | BS_AUTOCHECKBOX | styles,
        m_nX,
        m_nY,
        m_nWidth,
        m_nHeight,
        m_hParent,
        (HMENU)control_id,
        NULL,
        NULL);
}

int CCheck::getValue()
{
    return SendMessage(m_hWnd, BM_GETCHECK, 0, 0);
}

void CCheck::setValue(
    int value)
{
    SendMessage(m_hWnd, BM_SETCHECK, (WPARAM)value, 0);
}
