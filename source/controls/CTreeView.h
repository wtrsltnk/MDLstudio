#ifndef CTREEVIEW_H
#define CTREEVIEW_H

#include "CControl.h"

#include <commctrl.h>

class CTreeView : public CControl
{
private:
public:
    CTreeView(HINSTANCE, HWND, int, int, int, int, int);
    CTreeView(HINSTANCE, HWND);

    void create(void);
    void create(DWORD);
    void create(DWORD, DWORD);
    void reset(void);

    void setImageList(UINT, int, int, int);
    HTREEITEM addItem(const std::string &, HTREEITEM, int);

    HTREEITEM getSelection(void);
    TVITEM getItem(HTREEITEM);
    std::string getItemText(HTREEITEM);
    HTREEITEM getParent(HTREEITEM);
    HTREEITEM getChild(HTREEITEM);

    void expand(HTREEITEM);
};
#endif
