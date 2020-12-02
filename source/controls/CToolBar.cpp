#include "CToolBar.h"

/////////////////////////////////////
// Constructors / Destructors      //
/////////////////////////////////////

CToolBar::CToolBar(
    HINSTANCE hInstance,
    HWND hParent,
    int id)
    : CControl(
          hInstance,
          hParent,
          0, 0,
          0, 0,
          id)
{
    //bewaar de instance en de parent window handle
    this->m_hInstance = hInstance;
    this->m_hParent = hParent;

    this->iNumButtons = 0;
}

/////////////////////////////////////
// Memberfuncties                  //
/////////////////////////////////////

void CToolBar::create(
    UINT wBMID,
    int iNumBut,
    int iButWidth,
    int iButHeight)
{
    m_hWnd = CreateToolbarEx(
        m_hParent,
        WS_CHILD | WS_VISIBLE |
            TBSTYLE_FLAT | TBSTYLE_LIST | TBSTYLE_TOOLTIPS,
        1002,
        1,
        m_hInstance,
        wBMID,
        tbb,
        0,
        iButWidth,
        iButHeight,
        iButWidth,
        iButHeight,
        sizeof(TBBUTTON));

    SendMessage(m_hWnd, TB_AUTOSIZE, 0, 0);
}

/*
Resize functie
*/
void CToolBar::resize(WPARAM wParam, LPARAM lParam)
{
    SendMessage(m_hWnd, WM_SIZE, wParam, lParam);
}

bool CToolBar::addBitmaps(int nButtons, UINT nID)
{
    TBADDBITMAP lptbab;

    lptbab.hInst = m_hInstance;
    lptbab.nID = nID;

    SendMessage(m_hWnd, TB_ADDBITMAP, (WPARAM)nButtons, (LPARAM)(LPTBADDBITMAP)&lptbab);
    return true;
}

/*
Add toolbar button
*/
void CToolBar::addButton(
    const std::string &caption,
    int idCommand,
    int iBitmap)
{
    TBBUTTON tb;

    tb.fsState = TBSTATE_ENABLED;
    tb.fsStyle = TBSTYLE_GROUP;
    tb.idCommand = idCommand;
    tb.dwData = 0L;
    tb.iString = SendMessage(m_hWnd, TB_ADDSTRING, 0, (LPARAM)(LPSTR)caption.c_str());
    tb.iBitmap = iBitmap;
    SendMessage(m_hWnd, TB_INSERTBUTTON, (WPARAM)(UINT)iNumButtons++, (LPARAM)(LPTBBUTTON)&tb);
}

/*
Remove Button
*/
bool CToolBar::remButton(
    int iButNo)
{
    if (iButNo >= iNumButtons)
        return false;

    return true;
}

/*
Add seperator
*/
void CToolBar::addSeperator()
{
    TBBUTTON tb;

    tb.fsState = TBSTATE_ENABLED;
    tb.fsStyle = TBSTYLE_SEP;
    tb.idCommand = 0;
    tb.dwData = 0L;
    tb.iString = SendMessage(m_hWnd, TB_ADDSTRING, 0, (LPARAM)(LPSTR) "");

    SendMessage(m_hWnd, TB_INSERTBUTTON, (WPARAM)(UINT)iNumButtons++, (LPARAM)(LPTBBUTTON)&tb);
}
