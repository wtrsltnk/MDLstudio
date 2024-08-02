#ifndef CCHECK_H
#define CCHECK_H

#include "CControl.h"

class CCheck : public CControl
{
public:
    CCheck(HINSTANCE, HWND, int, int, int, int, int);
    CCheck(HINSTANCE, HWND);

    void create();
    void create(DWORD);
    void create(DWORD, DWORD);

    void setValue(int);
    int getValue();

private:
};
#endif
