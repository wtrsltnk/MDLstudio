#ifndef CCMANAGER_H
#define CCMANAGER_H

#include "..\resource\resource.h"
#include "COpenGl.h"
#include "Settings.h"
#include "controls\GUI.h"
#include <windows.h>

#define VIEW_MODEL 0
#define VIEW_MATERIAL 1

#define TREETYPE_REF 0
#define TREETYPE_REF_SUB 1
#define TREETYPE_SEQ 2
#define TREETYPE_SKIN 3
#define TREETYPE_CONT 4
#define TREETYPE_BONE 5
#define TREETYPE_MAT 6
#define TREETYPE_HIT 7

class CCManager
{
private:
    HINSTANCE m_hInstance;
    HWND m_hParent;

    int m_nWidth, m_nHeight;
    int m_nController;
    int m_nView;

    CTreeView *m_pPanel;
    CButton *m_pHide;
    CButton *m_pStartStop;
    CLabel *m_pLblController;
    CSlider *m_pController;
    CLabel *m_pLblAnimation;
    CSlider *m_pSpeed;
    CSlider *m_pFrames;
    COpenGl *m_p3DView;
    CStatusBar *m_pStatus;

    HTREEITEM m_hReference;
    HTREEITEM m_hControllers;
    HTREEITEM m_hSkins;
    HTREEITEM m_hSequence;
    HTREEITEM m_hMaterial;
    HTREEITEM m_hBones;
    HTREEITEM m_hHitboxes;

    StudioModel *m_pModel;

    DWORD dwTime, dwLastTime;

public:
    CCManager(HINSTANCE hInstance, HWND hParent);
    virtual ~CCManager(void);

    void resize(WPARAM wParam, LPARAM lParam);
    void togglePanel(void);
    bool toggleAnimatie(void);

    void ItemSelected(WPARAM wParam, LPARAM lParam);
    void SliderScoll(HWND hWnd, WPARAM wParam, LPARAM lParam);

    bool initGui(StudioModel *pModel);
    void clearGui(void);
    void drawOpenGL(void);

    void setView(int view);
    void setStatus(const char *text, ...);
    void centerview(void);
    void vmodelview(void);

    int getIndex(const std::string &str);
    int getTreeSelectedType();
    std::string getTreeSelectedText();
    std::string getTreeParentText();
    void statusPan(void);
};
#endif
