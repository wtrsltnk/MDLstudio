#include "CTreeView.h"

/////////////////////////////////////
// Constructors / Destructors      //
/////////////////////////////////////
CTreeView::CTreeView(
    HINSTANCE hInstance,
    HWND hParent,
    int nX, int nY,
    int nWidth, int nHeight,
    int id)
    : CControl(
          hInstance,
          hParent,
          nX, nY,
          nWidth, nHeight,
          id)
{
}

CTreeView::CTreeView(
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

void CTreeView::create()
{
    m_hWnd = CreateWindowEx(
        WS_EX_WINDOWEDGE,
        WC_TREEVIEW,
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

void CTreeView::create(DWORD ex_style)
{
    m_hWnd = CreateWindowEx(
        ex_style,
        WC_TREEVIEW,
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

void CTreeView::create(
    DWORD ex_style,
    DWORD styles)
{
    m_hWnd = CreateWindowEx(
        ex_style,
        WC_TREEVIEW,
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

void CTreeView::setImageList(
    UINT wBMID,
    int cx,
    int cy,
    int iNoImg)
{
    HIMAGELIST himl = ImageList_Create(cx, cy, ILC_COLORDDB, iNoImg, 0);
    ImageList_Add(himl, LoadBitmap(m_hInstance, MAKEINTRESOURCE(wBMID)), NULL);
    SendMessage(m_hWnd, TVM_SETIMAGELIST, TVSIL_NORMAL, (LPARAM)(HIMAGELIST)himl);
}

void CTreeView::reset(void)
{
    SendMessage(m_hWnd, TVM_DELETEITEM, 0, (LPARAM)TVI_ROOT);
}

HTREEITEM CTreeView::addItem(
    const std::string &str,
    HTREEITEM hParent,
    int iImage)
{
    TV_INSERTSTRUCT tvins;
    TV_ITEM tvi;

    tvi.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
    tvi.pszText = const_cast<char *>(str.c_str());
    tvi.iImage = iImage;
    tvi.iSelectedImage = iImage + 1;

    tvins.hParent = hParent;
    tvins.hInsertAfter = TVI_LAST;
    tvins.item = tvi;

    return TreeView_InsertItem(m_hWnd, &tvins);
}

HTREEITEM CTreeView::getSelection()
{
    return TreeView_GetSelection(m_hWnd);
}

TVITEM CTreeView::getItem(
    HTREEITEM hitem)
{
    TVITEM item;
    char text[255];

    item.hItem = hitem;
    item.cchTextMax = 255;
    item.pszText = text;
    item.mask = TVIF_TEXT |
                TVIF_CHILDREN |
                TVIF_IMAGE |
                TVIF_PARAM |
                TVIF_SELECTEDIMAGE |
                TVIF_STATE |
                TVIF_TEXT;

    SendMessage(m_hWnd, TVM_GETITEM, 0, (LPARAM)&item);
    return item;
}

std::string CTreeView::getItemText(
    HTREEITEM hItem)
{
    TVITEM item;
    char text[255];
    item.hItem = hItem;
    item.cchTextMax = 255;
    item.pszText = text;
    item.mask = TVIF_TEXT;

    if (SendMessage(m_hWnd, TVM_GETITEM, 0, (LPARAM)&item))
        return text;
    return "";
}

HTREEITEM CTreeView::getParent(
    HTREEITEM hitem)
{
    return TreeView_GetParent(m_hWnd, hitem);
}

HTREEITEM CTreeView::getChild(
    HTREEITEM hitem)
{
    return TreeView_GetChild(m_hWnd, hitem);
}

void CTreeView::expand(
    HTREEITEM hItem)
{
    TreeView_Expand(m_hWnd, hItem, TVE_EXPAND);
}
