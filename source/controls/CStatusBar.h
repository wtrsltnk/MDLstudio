#ifndef CSTATUSBAR_H
#define CSTATUSBAR_H

#include "CControl.h"

#define MAX_NUM_PARTS 25

class CStatusBar : public CControl
{
public:
    CStatusBar(HINSTANCE, HWND, int);
    ~CStatusBar();

    void create();
    void resize(WPARAM, LPARAM);

    void addItem(std::string, int);
    void addOneItem(std::string);

    void setItemText(std::string, int);

private:
    int parts[MAX_NUM_PARTS];
    int numparts;
};
#endif
