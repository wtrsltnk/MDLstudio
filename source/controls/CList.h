#ifndef CLIST_H
#define CLIST_H

#include "CControl.h"

class CList : public CControl
{
private:
public:
    CList(HINSTANCE, HWND, int, int, int, int, int);
    CList(HINSTANCE, HWND);

    void create(void);
    void create(DWORD);
    void create(DWORD, DWORD);

    void addItem(const std::string &);
    bool remItem(int);
    string getItem(int);
};
#endif
