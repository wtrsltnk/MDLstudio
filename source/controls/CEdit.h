#ifndef CEDIT_H
#define CEDIT_H

#include "CControl.h"

class CEdit : public CControl
{
public:
    CEdit(HINSTANCE, HWND, int, int, int, int, int);
    CEdit(HINSTANCE, HWND);
    void create(void);
    void create(DWORD);
    void create(DWORD, DWORD);

private:
};
#endif
