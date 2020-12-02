#include "CBrowseDlg.h"

CFolderView *CBrowseDlg::m_pFolder = NULL;
CFileView *CBrowseDlg::m_pFile = NULL;
HINSTANCE CBrowseDlg::m_hInstance = NULL;
HWND CBrowseDlg::m_hOwner = NULL;

/////////////////////////////////////
// Constructors / Destructors      //
/////////////////////////////////////

CBrowseDlg::CBrowseDlg(
    HINSTANCE hInstance,
    HWND hWnd)
{
    this->m_hOwner = hWnd;
    this->m_hInstance = hInstance;
}

CBrowseDlg::~CBrowseDlg(void)
{
}

/////////////////////////////////////
// Memberfuncties                  //
/////////////////////////////////////

BOOL CBrowseDlg::DlgProc(
    HWND hDlg,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam)
{
    RECT pl;

    switch (uMsg)
    {
        case WM_INITDIALOG:
        {
            m_pFile = new CFileView(m_hInstance, hDlg, 100, 0, 100, 100, ID_FILE);
            m_pFile->create(WS_EX_STATICEDGE, LVS_LIST | LVS_SHOWSELALWAYS | LVS_SORTASCENDING);

            m_pFolder = new CFolderView(m_hInstance, hDlg, 0, 0, 100, 100, ID_FOLDER);
            m_pFolder->create(WS_EX_STATICEDGE, TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_FULLROWSELECT | TVS_SHOWSELALWAYS);

            pl = settings.browse.rcNormalPosition;
            MoveWindow(hDlg, pl.left, pl.top, pl.right - pl.left, pl.bottom - pl.top, true);
            break;
        }
        case WM_SIZE:
        {
            int nWidth, nHeight;
            nWidth = LOWORD(lParam);
            nHeight = HIWORD(lParam);
            m_pFolder->resize(0, 0, nWidth / 3, nHeight);
            m_pFile->resize(nWidth / 3 + 5, 0, (nWidth / 3) * 2 - 5, nHeight);

            GetWindowPlacement(hDlg, &settings.browse);
            break;
        }
        case WM_MOVE:
        {
            GetWindowPlacement(hDlg, &settings.browse);
            break;
        }
        case WM_NOTIFY:
        {
            switch (((LPNMHDR)lParam)->idFrom)
            {
                case ID_FOLDER:
                {
                    switch (((LPNMHDR)lParam)->code)
                    {
                        case TVN_SELCHANGED:
                        {
                            m_pFile->initFiles(m_pFolder->getItemPath(m_pFolder->getSelection()));
                            break;
                        }
                        case TVN_ITEMEXPANDED:
                        {
                            m_pFolder->initPath(wParam, lParam);
                            break;
                        }
                    }
                    break;
                }
                case ID_FILE:
                {
                    switch (((LPNMHDR)lParam)->code)
                    {
                        case LVN_ITEMACTIVATE:
                        {
                            std::string strFilePath, strFileName;
                            strFilePath = m_pFolder->getItemPath(m_pFolder->getSelection());
                            strFileName = m_pFile->getItemText(m_pFile->getSelectedIndex());
                            if (strFilePath[strFilePath.length() - 1] != '\\')
                                strFilePath += '/';

                            char result[MAX_PATH];
                            strcpy_s(result, MAX_PATH, strFilePath.c_str());
                            strcat_s(result, MAX_PATH, strFileName.c_str());
                            SendMessage(m_hOwner, WM_LOADBROWSEFILE, (WPARAM)(LPTSTR)result, 0);
                            SetFocus(m_hOwner);
                            break;
                        }
                    }
                    break;
                }
            }
            break;
        }
        case WM_CLOSE:
        {
            GetWindowPlacement(hDlg, &settings.browse);
            SendMessage(m_hOwner, WM_CLOSEBROWSE, 0, 0);
            EndDialog(hDlg, ID_CLOSE);
            break;
        }
    }
    return FALSE;
}

void CBrowseDlg::show()
{
    m_hWnd = CreateDialog(m_hInstance, MAKEINTRESOURCE(IDD_BROWSE), m_hOwner, (DLGPROC)DlgProc);
    ShowWindow(m_hWnd, SW_SHOW);
}
