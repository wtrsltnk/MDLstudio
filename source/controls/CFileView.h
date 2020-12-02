#ifndef CFILEVIEW_H
#define CFILEVIEW_H

#include "CControl.h"

class CFileView : public CControl
{
private:
    DWORD style;

public:
    CFileView(HINSTANCE, HWND, int, int, int, int, int);
    CFileView(HINSTANCE, HWND);

    void create(void);
    void create(DWORD);
    void create(DWORD, DWORD);
    void reset(void);
    void setStyleEx(DWORD);
    void setStyle(DWORD);
    DWORD getStyle(void);

    void initFiles(const std::string &strFilePath);

    void setImageList(UINT, int, int, int);

    void addColumn(const std::string &, int, int);
    void setColumnWidth(int, int);
    int getColumnWidth(int);

    void addItem(const std::string &, int, int);
    void addSubItem(const std::string &, int, int);

    int getSelectedIndex(void);
    std::string getItemText(int);
    std::string getSubItemText(int, int);
};
#endif
