#include "CReportView.h"

/////////////////////////////////////
// Constructors / Destructors      //
/////////////////////////////////////
CReportView::CReportView(
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

CReportView::CReportView(
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

void CReportView::create()
{
    m_hWnd = CreateWindowEx(
        WS_EX_WINDOWEDGE,
        WC_LISTVIEW,
        "Caption",
        WS_CHILD | WS_VISIBLE |
            LVS_REPORT,
        m_nX,
        m_nY,
        m_nWidth,
        m_nHeight,
        m_hParent,
        (HMENU)control_id,
        NULL,
        NULL);
}

void CReportView::create(
    DWORD ex_style)
{
    m_hWnd = CreateWindowEx(
        ex_style,
        WC_LISTVIEW,
        "Caption",
        WS_CHILD | WS_VISIBLE |
            LVS_REPORT,
        m_nX,
        m_nY,
        m_nWidth,
        m_nHeight,
        m_hParent,
        (HMENU)control_id,
        NULL,
        NULL);
}

void CReportView::create(
    DWORD ex_style,
    DWORD styles)
{
    m_hWnd = CreateWindowEx(
        ex_style,
        WC_LISTVIEW,
        "Caption",
        WS_CHILD | WS_VISIBLE |
            LVS_REPORT | styles,
        m_nX,
        m_nY,
        m_nWidth,
        m_nHeight,
        m_hParent,
        (HMENU)control_id,
        NULL,
        NULL);
}

void CReportView::setImageList(
    UINT wBMID,
    int cx,
    int cy,
    int iNoImg)
{
    HIMAGELIST himl = ImageList_Create(cx, cy, ILC_COLORDDB, iNoImg, 0);
    ImageList_Add(himl, LoadBitmap(m_hInstance, MAKEINTRESOURCE(wBMID)), NULL);
    SendMessage(m_hWnd, TVM_SETIMAGELIST, TVSIL_NORMAL, (LPARAM)(HIMAGELIST)himl);
}

void CReportView::setExStyle(
    DWORD style)
{
    ListView_SetExtendedListViewStyleEx(m_hWnd, NULL, style);
}

void CReportView::reset()
{
    int i = 0;
    int result;

    result = 1;
    ListView_DeleteAllItems(m_hWnd);

    while (result)
    {
        result = ListView_DeleteColumn(m_hWnd, 0);
        i++;
    }
}

void CReportView::addColumn(
    std::string str,
    int iColNo,
    int width)
{

    LV_COLUMN lvc;

    lvc.mask = LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;
    lvc.fmt = LVCFMT_LEFT;
    lvc.cchTextMax = 300;
    lvc.cx = width;
    lvc.pszText = const_cast<char *>(str.c_str());
    lvc.iSubItem = iColNo;

    ListView_InsertColumn(m_hWnd, iColNo, &lvc);
}

void CReportView::setColumnWidth(
    int iColNo,
    int nWidth)
{
    SendMessage(m_hWnd,
                LVM_SETCOLUMNWIDTH,
                (WPARAM)iColNo,
                (LPARAM)MAKELPARAM(nWidth, 0));
}

int CReportView::getColumnWidth(int iColNo)
{
    return SendMessage(m_hWnd,
                       LVM_GETCOLUMNWIDTH,
                       (WPARAM)iColNo,
                       0);
}

void CReportView::addItem(
    std::string str,
    int iRowNo,
    int iImage)
{
    LV_ITEM lvi;

    lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
    lvi.state = 0;
    lvi.stateMask = -1;
    lvi.iItem = (iRowNo);
    lvi.iSubItem = 0;
    lvi.pszText = const_cast<char *>(str.c_str());
    lvi.iImage = iImage;

    ListView_InsertItem(m_hWnd, &lvi);
}

void CReportView::addSubItem(
    std::string s,
    int iRowNo,
    int iSubItem)
{
    ListView_SetItemText(m_hWnd, iRowNo, iSubItem, const_cast<char *>(s.c_str()));
}

int CReportView::getSelectedIndex()
{
    return ListView_GetSelectionMark(m_hWnd);
}

std::string CReportView::getItemText(
    int row)
{
    char text[255];
    ListView_GetItemText(m_hWnd, row, 0, text, 255);
    return text;
}

std::string CReportView::getSubItemText(
    int row,
    int sub)
{
    char text[255];
    ListView_GetItemText(m_hWnd, row, sub, text, 255);
    return text;
    return "";
}
