#include "CFileView.h"

#include <commctrl.h>

/////////////////////////////////////
// Constructors / Destructors      //
/////////////////////////////////////
CFileView::CFileView(
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
    this->style = 0;
}

CFileView::CFileView(
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

void CFileView::create()
{
    m_hWnd = CreateWindowEx(
        WS_EX_WINDOWEDGE,
        WC_LISTVIEW,
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
    this->style = getStyle();
}

void CFileView::create(
    DWORD ex_style)
{
    m_hWnd = CreateWindowEx(
        ex_style,
        WC_LISTVIEW,
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
    this->style = getStyle();
}

void CFileView::create(
    DWORD ex_style,
    DWORD styles)
{
    m_hWnd = CreateWindowEx(
        ex_style,
        WC_LISTVIEW,
        "Caption",
        WS_CHILD | WS_VISIBLE |
            styles,
        m_nX,
        m_nY,
        m_nWidth,
        m_nHeight,
        m_hParent,
        (HMENU)control_id,
        NULL,
        NULL);
    this->style = getStyle();
}

void CFileView::initFiles(
    std::string strFilePath)
{
    HANDLE hFind;
    WIN32_FIND_DATA fd;
    std::string strPathName, strFileSpec, strFileName;

    strPathName = strFilePath;

    reset();
    addColumn("File", 1, 300);

    strFileSpec = strPathName;
    if (strFileSpec[strlen(strFileSpec.c_str()) - 1] != '\\')
    {
        strFileSpec += '\\';
    }

    strFileSpec += "*.mdl";

    if ((hFind = FindFirstFile((LPCTSTR)strFileSpec.c_str(), &fd)) == INVALID_HANDLE_VALUE)
    {
        return;
    }

    do
    {
        if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        {
            continue;
        }

        strFileName = (LPCTSTR)&fd.cFileName;
        if ((strFileName != ".") && (strFileName != "..") && (fd.dwFileAttributes != 22))
        {
            addItem(strFileName, 1, 0);

            std::string strNewPathName;
            strNewPathName = strPathName;
            if (strNewPathName[strlen(strNewPathName.c_str()) - 1] != '\\')
            {
                strNewPathName += '\\';
            }

            strNewPathName += (LPCTSTR)&fd.cFileName;
        }

    } while (FindNextFile(hFind, &fd));

    FindClose(hFind);
}

void CFileView::setImageList(
    UINT wBMID,
    int cx,
    int cy,
    int iNoImg)
{
    HIMAGELIST himl = ImageList_Create(cx, cy, ILC_COLORDDB, iNoImg, 0);

    ImageList_Add(himl, LoadBitmap(m_hInstance, MAKEINTRESOURCE(wBMID)), NULL);

    SendMessage(m_hWnd, TVM_SETIMAGELIST, TVSIL_NORMAL, (LPARAM)(HIMAGELIST)himl);
}

void CFileView::setStyleEx(
    DWORD style)
{
    ListView_SetExtendedListViewStyleEx(m_hWnd, NULL, style);
}

void CFileView::setStyle(
    DWORD style)
{
    SetWindowLong(m_hWnd, GWL_STYLE, this->style | style);
}

DWORD CFileView::getStyle()
{
    return GetWindowLong(m_hWnd, GWL_STYLE);
}

void CFileView::reset()
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

void CFileView::addColumn(
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

void CFileView::setColumnWidth(
    int iColNo,
    int nWidth)
{
    SendMessage(
        m_hWnd,
        LVM_SETCOLUMNWIDTH,
        (WPARAM)iColNo,
        (LPARAM)MAKELPARAM(nWidth, 0));
}

int CFileView::getColumnWidth(
    int iColNo)
{
    return SendMessage(
        m_hWnd,
        LVM_GETCOLUMNWIDTH,
        (WPARAM)iColNo,
        0);
}

void CFileView::addItem(
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

void CFileView::addSubItem(
    std::string s,
    int iRowNo,
    int iSubItem)
{
    ListView_SetItemText(m_hWnd, iRowNo, iSubItem, const_cast<char *>(s.c_str()));
}

int CFileView::getSelectedIndex()
{
    return ListView_GetSelectionMark(m_hWnd);
}

std::string CFileView::getItemText(
    int row)
{
    char text[255];

    ListView_GetItemText(m_hWnd, row, 0, text, 255);

    return text;
}

std::string CFileView::getSubItemText(
    int row,
    int sub)
{
    char text[255];

    ListView_GetItemText(m_hWnd, row, sub, text, 255);

    return text;
}
