#ifndef CCOMBO_H
#define CCOMBO_H

#include "CControl.h"

class CCombo : public CControl
{
public:
    CCombo(HINSTANCE, HWND, int, int, int, int, int);
    CCombo(HINSTANCE, HWND);

    void create();
    void create(DWORD);
    void create(DWORD, DWORD);

    void addItem(std::string);
    bool remItem(int);
    std::string getItem(int);

    std::string getSelected();

private:
};
#endif
