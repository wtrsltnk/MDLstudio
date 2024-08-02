#ifndef CTOOLBAR_H
#define CTOOLBAR_H

#include "CControl.h"

#define MAX_BUTTONS 10

class CToolBar : public CControl
{
public:
    CToolBar(HINSTANCE, HWND, int);

    void create(UINT, int, int, int);
    void resize(WPARAM, LPARAM);

    bool addBitmaps(int, UINT);

    void addButton(const char *, int, int);
    bool remButton(int);
    void addSeperator(void);

private:
    TBBUTTON tbb[MAX_BUTTONS];
    int iNumButtons;
};
#endif
