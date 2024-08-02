#include "CMainFrame.h"

#include "Settings.h"
#include <stdlib.h>

StudioModel CMainFrame::m_Model;

/////////////////////////////////////
// Constructors / Destructors      //
/////////////////////////////////////
CMainFrame::CMainFrame(
    LPSTR lpCmdLine)
{
    memset(m_pAppPath, 0, MAX_PATH);
    memset(m_pSettings, 0, MAX_PATH);

    GetAppPath(GetCommandLine(), m_pAppPath);
    strcpy(m_pSettings, m_pAppPath);
    strcat(m_pSettings, "\\settings.set");

    this->m_dwCreationFlags = 0L;
    this->m_dwWindowStyle = WS_OVERLAPPEDWINDOW;
    this->m_dwExWindowStyle = WS_EX_WINDOWEDGE;
    this->m_dwCreationFlags = SW_SHOW;
    this->m_PosX = CW_USEDEFAULT;
    this->m_PosY = CW_USEDEFAULT;
    this->m_dwCreationWidth = CW_USEDEFAULT;
    this->m_dwCreationHeight = CW_USEDEFAULT;
    this->m_hbrWindowColor = (HBRUSH)(COLOR_WINDOW);
    this->m_hIcon = LoadIcon(m_hInstance, MAKEINTRESOURCE(IDI_ENERGY));
    this->m_strWindowTitle = "MDL studio";
    this->m_strAppVersion = "v 0.2.0";
    this->m_hMenu = LoadMenu(m_hInstance, MAKEINTRESOURCE(IDR_MENU));

    this->lpCmdLine = GetCommandLine();
    this->lpParameters = GetParameters(lpCmdLine);

    this->loaded = false;
    this->browsed = false;
}

CMainFrame::~CMainFrame()
{
    if (this->lpParameters != NULL)
    {
        delete[] this->lpParameters;
    }
}

/////////////////////////////////////
// Memberfuncties                  //
/////////////////////////////////////

LRESULT CMainFrame::MsgProc(
    HWND hWnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam)
{
    RECT pl;
    std::string result;

    switch (uMsg)
    {
        case WM_CREATE:
            m_pInfo = new CInfoDlg(m_hInstance, m_hWnd);
            m_pMaterial = new CMaterialDlg(m_hInstance, m_hWnd);
            m_pSequence = new CSequenceDlg(m_hInstance, m_hWnd);
            if (LoadSettings(m_pSettings) == 0)
            {
                InitSettings();
                GetWindowPlacement(m_hWnd, &settings.me);
            }

            CheckMenuRadioItem(this->m_hMenu,
                               IDM_VIEW_LEFT, IDM_VIEW_PERSPECTIVE,
                               IDM_VIEW_LEFT + settings.viewmode, 0);
            CheckMenuRadioItem(this->m_hMenu,
                               IDM_RENDERMODES_VERTEX, IDM_RENDERMODES_TEXTURED,
                               IDM_RENDERMODES_VERTEX + settings.rendermode, 0);

            if (settings.viewmode == VIEWMODE_PERSPECTIVE)
            {
                EnableMenuItem(this->m_hMenu, IDM_VIEW_CENTER, MF_ENABLED);
                EnableMenuItem(this->m_hMenu, IDM_VIEW_VMODEL, MF_ENABLED);
            }
            else
            {
                EnableMenuItem(this->m_hMenu, IDM_VIEW_CENTER, MF_GRAYED);
                EnableMenuItem(this->m_hMenu, IDM_VIEW_VMODEL, MF_GRAYED);
            }
            if (settings.show_bones)
                CheckMenuItem(this->m_hMenu, IDM_INSTELLINGEN_BONES, MF_CHECKED);
            if (settings.show_hitbox)
                CheckMenuItem(this->m_hMenu, IDM_INSTELLINGEN_HITBOXES, MF_CHECKED);
            if (settings.show_atach)
                CheckMenuItem(this->m_hMenu, IDM_INSTELLINGEN_ATACHMENTS, MF_CHECKED);
            if (settings.show_polys)
                CheckMenuItem(this->m_hMenu, IDM_VIEW_SHOWINFO, MF_CHECKED);
            if (settings.show_fps)
                CheckMenuItem(this->m_hMenu, IDM_VIEW_SHOWFPS, MF_CHECKED);
            if (settings.show_grid)
                CheckMenuItem(this->m_hMenu, IDM_VIEW_GRID, MF_CHECKED);

            m_pControls = new CCManager(m_hInstance, m_hWnd);
            m_pControls->clearGui();

            pl = settings.me.rcNormalPosition;
            MoveWindow(m_hWnd, pl.left, pl.top, pl.right - pl.left, pl.bottom - pl.top, true);
            if (settings.show_browse)
            {
                m_pBrowse = new CBrowseDlg(m_hInstance, m_hWnd);
                m_pBrowse->show();
                settings.show_browse = true;
            }
            m_pControls->setStatus("Settings loaded");

            if (this->lpParameters)
            {
                load(this->lpParameters);
            }
            else
            {
                setTitle("");
            }
            CreateMenus();
            break;

        case WM_CLOSE:
            GetWindowPlacement(m_hWnd, &settings.me);
            SaveSettings(m_pSettings);
            break;

        case WM_SIZE:
            m_pControls->resize(wParam, lParam);

            GetWindowPlacement(m_hWnd, &settings.me);
            break;

        case WM_PAINT:
            SetTimer(m_hWnd, ID_RENDER, TIMER_INTERVAL, (TIMERPROC)NULL);
            break;

        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
                case ID_HIDE:
                    switch (HIWORD(wParam))
                    {
                        case BN_CLICKED:
                            m_pControls->togglePanel();
                            break;
                    }
                    break;

                case ID_STARTSTOP:
                    switch (HIWORD(wParam))
                    {
                        case BN_CLICKED:
                            m_pControls->toggleAnimatie();
                            break;
                    }
                    break;

                case IDM_FILE_OPEN:
                    open();
                    break;

                case IDM_FILE_SAVE:
                    save();
                    break;

                case IDM_FILE_SAVEAS:
                    saveas();
                    break;

                case IDM_FILE_SLUITEN:
                    m_Model.FreeModel();
                    this->loaded = false;
                    m_pControls->clearGui();
                    setTitle("");
                    break;

                case IDM_FILE_BROWSE:
                    if (!settings.show_browse)
                    {
                        m_pBrowse = new CBrowseDlg(m_hInstance, m_hWnd);
                        m_pBrowse->show();
                        settings.show_browse = true;
                    }
                    break;

                case IDM_FILE_AFSLUITEN:
                    SendMessage(m_hWnd, WM_CLOSE, 0, 0);
                    break;

                case IDM_VIEW_CENTER:
                    m_pControls->centerview();
                    break;

                case IDM_VIEW_VMODEL:
                    m_pControls->vmodelview();
                    break;

                case IDM_VIEW_SHOWPOLYS:
                    settings.show_polys = !settings.show_polys;
                    CheckMenuItem(this->m_hMenu, IDM_VIEW_SHOWPOLYS, settings.show_polys ? MF_CHECKED : MF_UNCHECKED);
                    break;

                case IDM_VIEW_SHOWFPS:
                    settings.show_fps = !settings.show_fps;
                    CheckMenuItem(this->m_hMenu, IDM_VIEW_SHOWFPS, settings.show_fps ? MF_CHECKED : MF_UNCHECKED);
                    break;

                case IDM_VIEW_BONES:
                    settings.show_bones = !settings.show_bones;
                    CheckMenuItem(this->m_hMenu, IDM_INSTELLINGEN_BONES, settings.show_bones ? MF_CHECKED : MF_UNCHECKED);
                    break;

                case IDM_VIEW_HITBOXES:
                    settings.show_hitbox = !settings.show_hitbox;
                    CheckMenuItem(this->m_hMenu, IDM_INSTELLINGEN_HITBOXES, settings.show_hitbox ? MF_CHECKED : MF_UNCHECKED);
                    break;

                case IDM_VIEW_ATACHMENTS:
                    settings.show_atach = !settings.show_atach;
                    CheckMenuItem(this->m_hMenu, IDM_INSTELLINGEN_ATACHMENTS, settings.show_atach ? MF_CHECKED : MF_UNCHECKED);
                    break;

                case IDM_VIEW_GRID:
                    settings.show_grid = !settings.show_grid;
                    CheckMenuItem(this->m_hMenu, IDM_VIEW_GRID, settings.show_grid ? MF_CHECKED : MF_UNCHECKED);
                    break;

                case IDM_VIEW_LEFT:
                case IDM_VIEW_RIGHT:
                case IDM_VIEW_TOP:
                case IDM_VIEW_BOTTOM:
                case IDM_VIEW_FRONT:
                case IDM_VIEW_BACK:
                case IDM_VIEW_PERSPECTIVE:
                    settings.viewmode = LOWORD(wParam) - IDM_VIEW_LEFT;
                    CheckMenuRadioItem(this->m_hMenu,
                                       IDM_VIEW_LEFT, IDM_VIEW_PERSPECTIVE,
                                       LOWORD(wParam), 0);
                    if (LOWORD(wParam) == IDM_VIEW_PERSPECTIVE)
                    {
                        EnableMenuItem(this->m_hMenu, IDM_VIEW_CENTER, MF_ENABLED);
                        EnableMenuItem(this->m_hMenu, IDM_VIEW_VMODEL, MF_ENABLED);
                    }
                    else
                    {
                        EnableMenuItem(this->m_hMenu, IDM_VIEW_CENTER, MF_GRAYED);
                        EnableMenuItem(this->m_hMenu, IDM_VIEW_VMODEL, MF_GRAYED);
                    }
                    break;

                case IDM_SETTINGS_LIGHTCOLOR:
                    m_pCommonDlg = new CCommonDialogs(m_hInstance, m_hWnd);

                    m_pCommonDlg->color(settings.lightcolor);

                    delete m_pCommonDlg;
                    m_pCommonDlg = NULL;

                    SetTimer(m_hWnd, ID_RENDER, TIMER_INTERVAL, (TIMERPROC)NULL);
                    break;

                case IDM_SETTINGS_BGCOLOR:
                    m_pCommonDlg = new CCommonDialogs(m_hInstance, m_hWnd);

                    m_pCommonDlg->color(settings.bgcolor);

                    delete m_pCommonDlg;
                    m_pCommonDlg = NULL;

                    SetTimer(m_hWnd, ID_RENDER, TIMER_INTERVAL, (TIMERPROC)NULL);
                    break;
                case IDM_RENDERMODES_VERTEX:
                case IDM_RENDERMODES_WIREFRAME:
                case IDM_RENDERMODES_FLAT:
                case IDM_RENDERMODES_SMOOTH:
                case IDM_RENDERMODES_TEXTURED:
                    settings.rendermode = LOWORD(wParam) - IDM_RENDERMODES_VERTEX;
                    CheckMenuRadioItem(this->m_hMenu,
                                       IDM_RENDERMODES_VERTEX, IDM_RENDERMODES_TEXTURED,
                                       LOWORD(wParam), 0);
                    break;

                case IDM_HELP_INFO:
                    m_pInfo->show();
                    break;

                    //////////////////////////////////////////////////////////////
                    // SUBMENU ITEMS
                    //////////////////////////////////////////////////////////////
                case IDM_REF_EXPORT:
                    m_pCommonDlg = new CCommonDialogs(m_hInstance, m_hWnd);
                    result = m_pCommonDlg->save("Export to SMD", "SMD files (*.smd)\0*.smd", "smd");
                    if (!result.empty())
                    {
                        if (m_Model.SMD_GenerateReferences(
                                m_pControls->getIndex(m_pControls->getTreeSelectedText()),
                                const_cast<char *>(m_pControls->getTreeParentText().c_str()),
                                const_cast<char *>(result.c_str())))
                        {
                            MessageBox(m_hWnd, "Export succesfull", "Exporting a Reference", MB_OK | MB_ICONINFORMATION);
                        }
                        else
                        {
                            MessageBox(m_hWnd, "Export failed", "Exporting a Reference", MB_OK | MB_ICONEXCLAMATION);
                        }
                    }

                    delete m_pCommonDlg;
                    m_pCommonDlg = NULL;
                    break;

                case IDM_SEQ_EXPORT:
                    m_pCommonDlg = new CCommonDialogs(m_hInstance, m_hWnd);
                    result = m_pCommonDlg->save("Export to SMD", "SMD files (*.smd)\0*.smd", "smd");
                    if (!result.empty())
                    {
                        if (m_Model.SMD_ExportSequence(
                                const_cast<char *>(m_pControls->getTreeSelectedText().c_str()),
                                const_cast<char *>(result.c_str())))
                        {
                            MessageBox(m_hWnd, "Export succesfull", "Exporting a Sequence", MB_OK | MB_ICONINFORMATION);
                        }
                        else
                        {
                            MessageBox(m_hWnd, "Export failed", "Exporting a Sequence", MB_OK | MB_ICONEXCLAMATION);
                        }
                    }

                    delete m_pCommonDlg;
                    m_pCommonDlg = NULL;
                    break;

                case IDM_SEQ_INFO:
                    m_pSequence->show(
                        m_Model.GetSequence(const_cast<char *>(m_pControls->getTreeSelectedText().c_str())),
                        m_Model.GetEvent(const_cast<char *>(m_pControls->getTreeSelectedText().c_str())));
                    break;

                case IDM_MAT_EXPORT:
                    m_pCommonDlg = new CCommonDialogs(m_hInstance, m_hWnd);
                    result = m_pCommonDlg->save("Export to Bitmap", "Bitmap (*.bmp)\0*.bmp", "bmp");
                    if (!result.empty())
                    {
                        if (m_Model.BMP_ExportTexture(
                                const_cast<char *>(m_pControls->getTreeSelectedText().c_str()),
                                const_cast<char *>(result.c_str())))
                        {
                            MessageBox(m_hWnd, "Export succesfull", "Exporting a Material", MB_OK | MB_ICONINFORMATION);
                        }
                        else
                        {
                            MessageBox(m_hWnd, "Export failed", "Exporting a Material", MB_OK | MB_ICONEXCLAMATION);
                        }
                    }

                    delete m_pCommonDlg;
                    m_pCommonDlg = NULL;
                    break;

                case IDM_MAT_IMPORT:
                    m_pCommonDlg = new CCommonDialogs(m_hInstance, m_hWnd);
                    result = m_pCommonDlg->open("Import to Bitmap", "Bitmap (*.bmp)\0*.bmp", "bmp");
                    if (!result.empty())
                    {
                        if (m_Model.BMP_ImportTexture(
                                const_cast<char *>(m_pControls->getTreeSelectedText().c_str()),
                                const_cast<char *>(result.c_str())))
                        {
                            MessageBox(m_hWnd, "Import succesfull", "Importing a Material", MB_OK | MB_ICONINFORMATION);
                        }
                        else
                        {
                            MessageBox(m_hWnd, "Import failed", "Importing a Material", MB_OK | MB_ICONEXCLAMATION);
                        }
                    }

                    delete m_pCommonDlg;
                    m_pCommonDlg = NULL;
                    break;

                case IDM_MAT_INFO:
                    m_pMaterial->show(m_Model.GetMaterial(const_cast<char *>(m_pControls->getTreeSelectedText().c_str())));
                    break;

                    //////////////////////////////////////////////////////////////
                    // EINDE SUBMENU ITEMS
                    //////////////////////////////////////////////////////////////
            }
            break;

        case WM_NOTIFY:
            switch (wParam)
            {
                case ID_PANEL:
                    switch (((NMHDR *)lParam)->code)
                    {
                        case TVN_SELCHANGED:
                            KillTimer(m_hWnd, ID_RENDER);
                            m_pControls->ItemSelected(wParam, lParam);
                            SetTimer(m_hWnd, ID_RENDER, TIMER_INTERVAL, (TIMERPROC)NULL);
                            break;

                        case NM_RCLICK:
                            POINT point;
                            GetCursorPos(&point);

                            switch (m_pControls->getTreeSelectedType())
                            {
                                case TREETYPE_REF_SUB:
                                    TrackPopupMenu(m_hReferences, 0, point.x, point.y, 0, m_hWnd, 0);
                                    break;
                                case TREETYPE_SEQ:
                                    TrackPopupMenu(m_hSequences, 0, point.x, point.y, 0, m_hWnd, 0);
                                    break;
                                case TREETYPE_SKIN:
                                    // TrackPopupMenu( m_hSkins, 0, point.x, point.y, 0, m_hWnd, 0 );
                                    break;
                                case TREETYPE_CONT:
                                    // TrackPopupMenu( m_hControllers, 0, point.x, point.y, 0, m_hWnd, 0 );
                                    break;
                                case TREETYPE_MAT:
                                    TrackPopupMenu(m_hMaterials, 0, point.x, point.y, 0, m_hWnd, 0);
                                    break;
                                case TREETYPE_BONE:
                                    // TrackPopupMenu( m_hBones, 0, point.x, point.y, 0, m_hWnd, 0 );
                                    break;
                                case TREETYPE_HIT:
                                    // TrackPopupMenu( m_hHitboxes, 0, point.x, point.y, 0, m_hWnd, 0 );
                                    break;
                                default:
                                    break;
                            }
                    }
                    break;
            }
            break;

        case WM_TIMER:
            switch (wParam)
            {
                case ID_RENDER:
                    if (loaded)
                    {
                        m_pControls->drawOpenGL();
                    }
                    else
                    {
                        m_pControls->clearGui();
                        KillTimer(m_hWnd, ID_RENDER);
                    }
                    break;
            }
            break;

        case WM_HSCROLL:
            if (loaded)
                m_pControls->SliderScoll(hWnd, wParam, lParam);
            break;

        case WM_LOADBROWSEFILE:
            load((LPTSTR)wParam);
            break;

        case WM_CLOSEBROWSE:
            settings.show_browse = false;
            delete m_pBrowse;
            m_pBrowse = NULL;
            break;
    }
    return CWin::MsgProc(hWnd, uMsg, wParam, lParam);
}

char *CMainFrame::GetParameters(char *cmdline)
{
    char *result;
    int cmdlen;

    cmdlen = strlen(cmdline);

    if (cmdline[cmdlen - 1] == '\"')
    {
        for (int i = cmdlen - 2; i >= 0; i--)
        {
            if (cmdline[i] == '\"')
            {
                if (i == 0)
                    return NULL;
                result = new char[cmdlen - i];
                for (int j = 1; j < cmdlen - i - 1; j++)
                {
                    result[j - 1] = cmdline[j + i];
                }
                result[cmdlen - i - 1] = '\0';
                return result;
            }
        }
    }
    return NULL;
}

void CMainFrame::GetAppPath(char *cmdline, char *dest)
{
    int cmdlen;
    int i, j, k;

    cmdlen = strlen(cmdline);

    if (cmdline[0] == '\"')
    {
        for (i = 1; i < cmdlen; i++)
        {
            if (cmdline[i] == '\"')
            {
                for (j = 0; j < i; j++)
                {
                    dest[j] = cmdline[j + 1];
                }
                for (k = j; k > 0; k--)
                {
                    if (dest[k] == '\\')
                    {
                        dest[k] = '\0';
                        return;
                    }
                }
            }
        }
    }
}

void CMainFrame::open()
{
    std::string result;

    m_pCommonDlg = new CCommonDialogs(m_hInstance, m_hWnd);
    result = m_pCommonDlg->open("Open HL-Model file", "MDL file(*.mdl)\0*.mdl", "mdl");
    if (!result.empty())
    {
        load(const_cast<char *>(result.c_str()));
    }

    delete m_pCommonDlg;
    m_pCommonDlg = NULL;
}

void CMainFrame::saveas()
{
    std::string result;

    m_pCommonDlg = new CCommonDialogs(m_hInstance, m_hWnd);
    result = m_pCommonDlg->save("Save HL-Model file", "MDL file(*.mdl)\0*.mdl", "mdl");
    if (!result.empty())
    {
        m_Model.SaveModel(const_cast<char *>(result.c_str()));
    }

    delete m_pCommonDlg;
    m_pCommonDlg = NULL;
}

void CMainFrame::save()
{
    if (loaded)
    {
        m_Model.SaveModel(m_pFileName);
    }
}

void CMainFrame::load(char *filename)
{
    KillTimer(m_hWnd, ID_RENDER);
    if (this->loaded)
    {
        m_pControls->setStatus("Clearing memory...");
        m_Model.FreeModel();
    }

    this->loaded = false;
    try
    {
        m_pControls->setStatus("Loading model...");
        m_Model.Init(filename);
    }
    catch (char *error)
    {
        MessageBox(m_hWnd, error, "ERROR:", MB_OK);
        m_pControls->setStatus("ERROR: %s", error);
        m_pControls->clearGui();
        SetTimer(m_hWnd, ID_RENDER, TIMER_INTERVAL, (TIMERPROC)NULL);
        return;
    }
    this->loaded = true;
    strcpy(m_pFileName, filename);

    m_pControls->setStatus("Initializing GUI...");
    m_pControls->clearGui();
    m_pControls->initGui(&m_Model);

    setTitle(filename);
    m_pControls->setView(VIEW_MODEL);
    SetTimer(m_hWnd, ID_RENDER, TIMER_INTERVAL, (TIMERPROC)NULL);
    m_pControls->setStatus("%s loaded", filename);
}

void CMainFrame::setTitle(char *filename)
{
    char *title;

    title = new char[strlen(filename) + strlen(this->m_strWindowTitle) + 3 + strlen(this->m_strAppVersion) + 3];
    if (strcmp(filename, "") != 0)
    {
        strcpy(title, filename);
        strcat(title, " - ");
        strcat(title, this->m_strWindowTitle);
    }
    else
    {
        strcpy(title, this->m_strWindowTitle);
    }
    strcat(title, " - ");
    strcat(title, this->m_strAppVersion);
    SendMessage(m_hWnd, WM_SETTEXT, 0, (LPARAM)(LPTSTR)title);
}

void CMainFrame::CreateMenus()
{
    m_hReferences = CreatePopupMenu();
    AppendMenu(m_hReferences, MF_STRING, IDM_REF_EXPORT, "Export");

    m_hSequences = CreatePopupMenu();
    AppendMenu(m_hSequences, MF_STRING, IDM_SEQ_EXPORT, "Export...");
    AppendMenu(m_hSequences, MF_STRING, IDM_SEQ_INFO, "Edit...");
    /*
            m_hSkins = CreatePopupMenu();
            AppendMenu( m_hSkins, MF_STRING, IDM_SKIN_EXPORT, "Export" );
            AppendMenu( m_hSkins, MF_STRING, IDM_SKIN_IMPORT, "Import" );

            m_hControllers = CreatePopupMenu();
            AppendMenu( m_hControllers, MF_STRING, IDM_CON_EXPORT, "Export" );
            AppendMenu( m_hControllers, MF_STRING, IDM_CON_IMPORT, "Import" );

            m_hBones = CreatePopupMenu();
            AppendMenu( m_hBones, MF_STRING, IDM_BONE_EXPORT, "Export" );
            AppendMenu( m_hBones, MF_STRING, IDM_BONE_IMPORT, "Import" );

            m_hHitboxes = CreatePopupMenu();
            AppendMenu( m_hHitboxes, MF_STRING, IDM_HIT_EXPORT, "Export" );
            AppendMenu( m_hHitboxes, MF_STRING, IDM_HIT_IMPORT, "Import" );
    */
    m_hMaterials = CreatePopupMenu();
    AppendMenu(m_hMaterials, MF_STRING, IDM_MAT_EXPORT, "Export...");
    AppendMenu(m_hMaterials, MF_STRING, IDM_MAT_IMPORT, "Import...");
    AppendMenu(m_hMaterials, MF_STRING, IDM_MAT_INFO, "Edit...");
}
