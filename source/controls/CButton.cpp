#include "CButton.h"

/////////////////////////////////////
// Constructors / Destructors      //
/////////////////////////////////////
CButton::CButton(
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

{
}

/////////////////////////////////////
// Memberfuncties                  //
/////////////////////////////////////

void CButton::create(void)
{
    m_hWnd = CreateWindowEx(
        WS_EX_WINDOWEDGE,
        "BUTTON",
        "Caption",
        WS_CHILD | WS_VISIBLE |
            BS_NOTIFY | BS_PUSHBUTTON,
        m_nX,
        m_nY,
        m_nWidth,
        m_nHeight,
        m_hParent,
        (HMENU)control_id,
        NULL,
        NULL);
}

{
    m_hWnd = CreateWindowEx(
        ex_style,
        "BUTTON",
        "Caption",
        WS_CHILD | WS_VISIBLE |
            BS_NOTIFY | BS_PUSHBUTTON,
        m_nX,
        m_nY,
        m_nWidth,
        m_nHeight,
        m_hParent,
        (HMENU)control_id,
        NULL,
        NULL);
}

{
    m_hWnd = CreateWindowEx(
        ex_style,
        "BUTTON",
        "Caption",
        WS_CHILD | WS_VISIBLE |
            BS_NOTIFY | BS_PUSHBUTTON | styles,
        m_nX,
        m_nY,
        m_nWidth,
        m_nHeight,
        m_hParent,
        (HMENU)control_id,
        NULL,
        NULL);
}
