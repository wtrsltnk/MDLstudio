#ifndef CBUTTON_H
#define CBUTTON_H

#include "CControl.h"

class CButton : public CControl
{
public:
    CButton(HINSTANCE, HWND, int, int, int, int, int);
    CButton(HINSTANCE, HWND);

    void create();
    void create(DWORD);
    void create(DWORD, DWORD);

private:
};
#endif
