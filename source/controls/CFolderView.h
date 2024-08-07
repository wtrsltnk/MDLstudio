#ifndef CFOLDERVIEW_H
#define CFOLDERVIEW_H

#include "CControl.h"

#include "..\..\resource\resource.h"

class CFolderView : public CControl
{
public:
    CFolderView(HINSTANCE, HWND, int, int, int, int, int);
    CFolderView(HINSTANCE, HWND);

    void create(void);
    void create(DWORD);
    void create(DWORD, DWORD);
    void reset(void);

    std::string initPath(WPARAM wParam, LPARAM lParam);

    std::string getItemPath(HTREEITEM hItem);
    std::string getSelItemPath(void);

    void setImageList(UINT, int, int, int);

    HTREEITEM getSelection(void);
    TVITEM getItem(HTREEITEM);
    std::string getItemText(HTREEITEM);
    HTREEITEM getParent(HTREEITEM);
    HTREEITEM getChild(HTREEITEM);

    void expand(HTREEITEM);

private:
    HTREEITEM hParentItem;

    HTREEITEM addItem(std::string, HTREEITEM, int iImage, int iSelImage);
    void addDummyNode(HTREEITEM hItem);

    void clearChilderen(HTREEITEM hItem);
    bool isDriveNode(HTREEITEM hItem);
    void setButtonState(HTREEITEM hItem, std::string strPathName);
    bool hasSubdirectory(std::string strPathName);

    void initIcons(void);
    void initMycomputer(void);
    void initDrives(void);
};
#endif
