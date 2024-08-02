#ifndef CFRAME_H
#define CFRAME_H

#include "CControl.h"

class CFrame : public CControl
{
public:
    CFrame(HINSTANCE, HWND, int, int, int, int, int, int);
    CFrame(HINSTANCE, HWND);
    void create(void);
    void create(DWORD);
    void create(DWORD, DWORD);

    void addOption(std::string, int, int, int, int);

    std::string getOptionText(int);
    long setOptionText(int, std::string);

    int getOptionValue(int);
    void setOptionValue(int, int);

    int getSelOption(void);

private:
    HWND *m_hOptions;
    int optMax;
    int optCount;
};
#endif
