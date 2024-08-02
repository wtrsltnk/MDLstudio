#ifndef CSLIDER_H
#define CSLIDER_H

#include "CControl.h"

class CSlider : public CControl
{
public:
    CSlider(HINSTANCE, HWND, int, int, int, int, int);
    CSlider(HINSTANCE, HWND);

    void create(void);
    void create(DWORD);
    void create(DWORD, DWORD);
    void init(int min, int max);
    void reset();

    void setSel(int sel);
    int getSel();

private:
};
#endif
