#ifndef CTREEVIEW_H
#define CTREEVIEW_H

#include "CControl.h"

class CTreeView : public CControl
{
public:
    CTreeView(HINSTANCE, HWND, int, int, int, int, int);
    CTreeView(HINSTANCE, HWND);

    void create();
    void create(DWORD);
    void create(DWORD, DWORD);
    void reset();

    void setImageList(UINT, int, int, int);
    HTREEITEM addItem(std::string, HTREEITEM, int);

    HTREEITEM getSelection();
    TVITEM getItem(HTREEITEM);
    std::string getItemText(HTREEITEM);
    HTREEITEM getParent(HTREEITEM);
    HTREEITEM getChild(HTREEITEM);

    void expand(HTREEITEM);

private:
};

#endif
