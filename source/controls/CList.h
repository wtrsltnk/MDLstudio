#ifndef CLIST_H
#define CLIST_H

#include "CControl.h"

class CList : public CControl
{
public:
    CList(HINSTANCE, HWND, int, int, int, int, int);
    CList(HINSTANCE, HWND);

    void create(void);
    void create(DWORD);
    void create(DWORD, DWORD);

    void addItem(std::string);
    bool remItem(int);
    std::string getItem(int);

private:
};
#endif
