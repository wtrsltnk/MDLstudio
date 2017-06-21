#include "CButton.h"
//#include "CCheck.h"
//#include "CCombo.h"
//#include "CEdit.h"
//#include "CFrame.h"
#include "CLabel.h"
//#include "CList.h"
//#include "CReportView.h"
#include "CStatusBar.h"
#include "CToolBar.h"
#include "CTreeView.h"
#include "CCommonDialogs.h"
#include "CSlider.h"
#include "CFolderView.h"
#include "CFileView.h"

// define control id's
#define ID_FOLDER				0
#define ID_FILE					1

// define window messages for browse dialog
#define WM_LOADBROWSEFILE		WM_USER+2303
#define WM_CLOSEBROWSE			WM_USER+2304