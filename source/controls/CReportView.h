#ifndef CREPORTVIEW_H
#define CREPORTVIEW_H

#include "CControl.h"

class CReportView : public CControl
{
public:
    CReportView(HINSTANCE, HWND, int, int, int, int, int);
    CReportView(HINSTANCE, HWND);

    void create();
    void create(DWORD);
    void create(DWORD, DWORD);
    void reset();
    void setExStyle(DWORD);

    void setImageList(UINT, int, int, int);

    void addColumn(std::string, int, int);
    void setColumnWidth(int, int);
    int getColumnWidth(int);

    void addItem(std::string, int, int);
    void addSubItem(std::string, int, int);

    int getSelectedIndex();
    std::string getItemText(int);
    std::string getSubItemText(int, int);

private:
};
#endif
