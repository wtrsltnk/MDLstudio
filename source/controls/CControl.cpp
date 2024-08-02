#include "CControl.h"

/////////////////////////////////////
// Constructors / Destructors      //
/////////////////////////////////////

CControl::CControl(
    HINSTANCE hInstance,
    HWND hParent,
    int nX, int nY,
    int nWidth, int nHeight, int id)
{
    this->m_hParent = hParent;
    this->m_hInstance = hInstance;

    this->m_nX = nX;
    this->m_nY = nY;
    this->m_nWidth = nWidth;
    this->m_nHeight = nHeight;
    this->control_id = id;
}

CControl::CControl(
    HINSTANCE hInstance,
    HWND hParent)
{
    this->m_hParent = hParent;
    this->m_hInstance = hInstance;

    this->m_nX = 0;
    this->m_nY = 0;
    this->m_nWidth = 100;
    this->m_nHeight = 100;
}

CControl::~CControl(void)
{
    DestroyWindow(m_hWnd);
}

/////////////////////////////////////
// Memberfuncties                  //
/////////////////////////////////////

void CControl::resize(
    int nX,
    int nY,
    int nWidth,
    int nHeight)
{
    this->m_nX = nX;
    this->m_nY = nY;
    this->m_nWidth = nWidth;
    this->m_nHeight = nHeight;
    MoveWindow(m_hWnd, nX, nY, nWidth, nHeight, TRUE);
}

int CControl::getID() const
{
    return GetDlgCtrlID(m_hWnd);
}

void CControl::setFont(
    HFONT hFont)
{
    SendMessage(m_hWnd, WM_SETFONT, (WPARAM)hFont, (LPARAM)TRUE);
}

HFONT CControl::getFont()
{
    return (HFONT)SendMessage(m_hWnd, WM_GETFONT, 0, 0);
}

void CControl::setText(
    const std::string &text)
{
    std::string strTemp;
    for (size_t i = 0; i < text.size(); i++)
    {
        if (text[i] == '\n')
        {
            strTemp += '\r';
        }

        strTemp += text[i];
    }

    SendMessage(m_hWnd, WM_SETTEXT, 0, (LPARAM)strTemp.c_str());
}

void CControl::addText(
    const std::string &text)
{
    char oldText[1024];
    std::string strTemp;

    int nText = SendMessage(m_hWnd, WM_GETTEXTLENGTH, 0, 0);
    SendMessage(m_hWnd, WM_GETTEXT, nText + 1, (LPARAM)oldText);

    for (int i = 0; i < nText; i++)
        strTemp += oldText[i];

    for (size_t j = 0; j < text.size(); j++)
    {
        if (text[j] == '\n')
        {
            strTemp += '\r';
        }

        strTemp += text[j];
    }

    SendMessage(m_hWnd, WM_SETTEXT, 0, (LPARAM)strTemp.c_str());
}

std::string CControl::getText()
{
    int nlenght;
    char *result;

    result = new char;

    nlenght = SendMessage(m_hWnd, WM_GETTEXTLENGTH, 0, 0);
    SendMessage(m_hWnd, WM_GETTEXT, (WPARAM)nlenght + 1, (LPARAM)(LPCTSTR)result);

    return result;
}

void CControl::setFocus()
{
    SetFocus(m_hWnd);
}

void CControl::hide(
    bool hide)
{
    if (hide)
    {
        ShowWindow(m_hWnd, SW_HIDE);
    }
    else
    {
        ShowWindow(m_hWnd, SW_SHOW);
    }
}
