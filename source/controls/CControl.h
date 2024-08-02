#ifndef CCONTROL_H
#define CCONTROL_H

#include <windows.h>

#include <commctrl.h>
#include <string>

#define STYLE_FLAT WS_EX_WINDOWEDGE
#define STYLE_3D WS_EX_CLIENTEDGE
#define STYLE_BORDER WS_BORDER

class CControl
{
public:
    CControl(HINSTANCE, HWND, int, int, int, int, int);
    CControl(HINSTANCE, HWND);
    ~CControl(void);
    void resize(int, int, int, int);

    int getID() const;

    void setFont(HFONT);
    HFONT getFont();

    void setText(
        const std::string &text);

    void addText(
        const std::string &text);

    std::string getText();

    HWND getHandle() const { return m_hWnd; }
    void setFocus();
    void hide(bool hide);

protected:
    HINSTANCE m_hInstance;
    HWND m_hWnd, m_hParent;
    int m_nX, m_nY, m_nWidth, m_nHeight;
    int control_id;
};
#endif
