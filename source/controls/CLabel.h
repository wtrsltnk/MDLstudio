#ifndef CLABEL_H
#define CLABEL_H

#include "CControl.h"

class CLabel : public CControl
{
private:
public:
    CLabel(HINSTANCE, HWND, int, int, int, int, int);
    CLabel(HINSTANCE, HWND);
    void create(void);
    void create(DWORD);
    void create(DWORD, DWORD);
};
#endif
