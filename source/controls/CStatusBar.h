#ifndef CSTATUSBAR_H
#define CSTATUSBAR_H

#include "CControl.h"

#define MAX_NUM_PARTS 25

class CStatusBar : public CControl
{
private:
    int parts[MAX_NUM_PARTS];
    int numparts;

public:
    CStatusBar(HINSTANCE, HWND, int);
    ~CStatusBar(void);

    void create(void);
    void resize(WPARAM, LPARAM);

    void addItem(const std::string &, int);
    void addOneItem(const std::string &);

    void setItemText(const std::string &, int);
};
#endif
