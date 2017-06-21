#include "CFolderView.h"

/////////////////////////////////////
// Constructors / Destructors      //
/////////////////////////////////////
CFolderView::CFolderView( HINSTANCE hInstance, 
					 HWND hParent,
					 int nX, int nY , 
					 int nWidth ,int nHeight, int id ) : CControl( hInstance, 
					 hParent,
					 nX, nY , 
					 nWidth ,nHeight, id )
{
	this->hParentItem = TVGN_ROOT;
}

CFolderView::CFolderView( HINSTANCE hInstance, HWND hParent ) : CControl( hInstance, hParent )
{
}

/////////////////////////////////////
// Memberfuncties                  //
/////////////////////////////////////

void CFolderView::create( void )
{
	m_hWnd = CreateWindowEx( WS_EX_WINDOWEDGE,
		 WC_TREEVIEW, 
		 "Caption", 
		 WS_CHILD | WS_VISIBLE,
		 m_nX, 
		 m_nY, 
		 m_nWidth, 
		 m_nHeight, 
		 m_hParent, 
		 (HMENU)control_id, 
		 NULL, 
		 NULL );
	initMycomputer();
}

void CFolderView::create( DWORD ex_style )
{
	m_hWnd = CreateWindowEx( ex_style,
		 WC_TREEVIEW, 
		 "Caption", 
		 WS_CHILD | WS_VISIBLE, 
		 m_nX, 
		 m_nY, 
		 m_nWidth, 
		 m_nHeight, 
		 m_hParent, 
		 (HMENU)control_id, 
		 NULL, 
		 NULL );
	initMycomputer();
}

void CFolderView::create( DWORD ex_style, DWORD styles )
{
	m_hWnd = CreateWindowEx( ex_style,
		 WC_TREEVIEW, 
		 "Caption", 
		 WS_CHILD | WS_VISIBLE | styles,
		 m_nX, 
		 m_nY, 
		 m_nWidth, 
		 m_nHeight, 
		 m_hParent, 
		 (HMENU)control_id, 
		 NULL, 
		 NULL );
	initMycomputer();
}

void CFolderView::initMycomputer( void )
{
	initIcons();
	hParentItem = addItem("My computer" , TVGN_ROOT, 0, 0 );
	initDrives();
	expand( hParentItem );
}

void CFolderView::initIcons( void )
{
	HIMAGELIST himl = ImageList_Create( 16, 16, ILC_COLORDDB, 7, 0 );

	ImageList_Add( himl, LoadBitmap( m_hInstance, MAKEINTRESOURCE( IDB_EXPLORER ) ), NULL );

	SendMessage( m_hWnd, TVM_SETIMAGELIST, TVSIL_NORMAL , ( LPARAM ) ( HIMAGELIST ) himl );
}

void CFolderView::initDrives( void )
{
	int nPos = 0;
	HTREEITEM hItem;
    UINT nCount = 0;
	int iImage, iSelImage;
    string strDrive = "?:\\";

    DWORD dwDriveList = GetLogicalDrives ();

	string cTmp;
	
    while (dwDriveList)
	{
        if (dwDriveList & 1)
		{
			cTmp = strDrive;
            strDrive[0] = 0x41 + nPos;

			UINT nType = GetDriveType ((LPCTSTR) strDrive.c_str());
			UINT nDrive = (UINT) strDrive[0] - 0x41;

			switch (nType)
			{

			case DRIVE_REMOVABLE:
				iImage = iSelImage = 6;
				if ( strcmp(strDrive.c_str(), "A:\\") == 0 )
					iImage = iSelImage = 3;
				break;

			case DRIVE_FIXED:
				iImage = iSelImage = 4;
				break;

			case DRIVE_REMOTE:
				iImage = iSelImage = 4;
				break;

			case DRIVE_CDROM:
				iImage = iSelImage = 5;
				break;

			case DRIVE_RAMDISK:
				iImage = iSelImage = 5;
				break;

			default:
				iImage = 1;
				iSelImage = 2;
				break;
			}
            hItem = addItem(strDrive , hParentItem, iImage, iSelImage );
			addDummyNode( hItem );;
			if ( hItem )
                nCount++;
        }
        dwDriveList >>= 1;
        nPos++;
    }
}

string CFolderView::initPath( WPARAM wParam, LPARAM lParam )
{
	HANDLE hFind;
	WIN32_FIND_DATA fd;
	LPNMTREEVIEW pnmtv;
	HTREEITEM hItem ;
	string strPathName, strFileSpec, strFileName;

	pnmtv = (LPNMTREEVIEW)lParam;
	hItem = pnmtv->itemNew.hItem;
	strPathName = getItemPath( pnmtv->itemNew.hItem );

	if ( hItem == hParentItem )
		return getItemPath( getSelection() );

	clearChilderen(hItem);
	strFileSpec = strPathName;
	if (strFileSpec[strlen(strFileSpec.c_str())-1] != '\\')
		strFileSpec += '\\';
	strFileSpec += "*.*";

	if ((hFind = FindFirstFile ((LPCTSTR) strFileSpec.c_str(), &fd)) == INVALID_HANDLE_VALUE)
		return getItemPath( getSelection() );

    do 
	{
        if ( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) 
		{
			strFileName = (LPCTSTR) &fd.cFileName;
            if ((strFileName != ".") && (strFileName != "..") && (fd.dwFileAttributes != 22))
			{
                HTREEITEM hChild =
                    addItem ((LPCTSTR) &fd.cFileName,
					hItem, 1, 2);

                string strNewPathName;
				strNewPathName = strPathName;
                if (strNewPathName[strlen(strFileSpec.c_str())] != '\\')
                    strNewPathName += '\\';

                strNewPathName += (LPCTSTR) &fd.cFileName;
                setButtonState (hChild, strNewPathName);
				
	        }
		}

    } while (FindNextFile (hFind, &fd));

    FindClose (hFind);
	return getItemPath( getSelection() );
}

void CFolderView::clearChilderen( HTREEITEM hItem )
{
    HTREEITEM hChild = getChild( hItem );

    while (hChild != NULL) 
	{
		SendMessage( m_hWnd, TVM_DELETEITEM, 0, (LPARAM) hChild );
		hChild = getChild( hItem );
    }
}

void CFolderView::setButtonState( HTREEITEM hItem, string strPathName )
{
	if (hasSubdirectory (strPathName))
        addDummyNode (hItem);
}

bool CFolderView::hasSubdirectory(string strPathName)
{
    HANDLE hFind;
    WIN32_FIND_DATA fd;
    bool bResult = false;
    string strFileSpec, strFileName;

	strFileSpec = strPathName;
    if ( strFileSpec[ strlen(strFileSpec.c_str()) ] != '\\' )
        strFileSpec += '\\';
    strFileSpec += "*.*";

    if ((hFind = FindFirstFile((LPCTSTR) strFileSpec.c_str(), &fd)) != INVALID_HANDLE_VALUE)
	{
		do 
		{
			if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				strFileName = (LPCTSTR) &fd.cFileName;
				if ((strFileName != ".") && (strFileName != ".."))
					bResult = true;
			}
		} while (::FindNextFile (hFind, &fd) && !bResult);
		FindClose (hFind);
	}
    return bResult;

}

bool CFolderView::isDriveNode( HTREEITEM hItem )
{
	return (getParent (hItem) == NULL) ? true : false;
}

void CFolderView::addDummyNode( HTREEITEM hItem )
{
	addItem( "searching...", hItem, 1, 2 );
}

string CFolderView::getItemPath( HTREEITEM hItem )
{
	HTREEITEM hHigh;
	string result, temp;

	hHigh = hItem;
	while ( hHigh != hParentItem && hHigh != NULL )
	{
		temp = getItemText( hHigh ).c_str();
		if ( temp.c_str() != NULL )
		{
			if (temp[strlen(temp.c_str())-1] != '\\')
				temp += "\\";
			result = temp + result;
		}
		hHigh = getParent( hHigh );
	}
	return result;
}

string CFolderView::getSelItemPath( void )
{
	HTREEITEM hHigh;
	string result, temp;

	hHigh = getSelection();
	while ( hHigh != hParentItem && hHigh != NULL )
	{
		temp = getItemText( hHigh ).c_str();
		if ( temp.c_str() != NULL )
			result = temp + result;
		hHigh = getParent( hHigh );
	}
	return result;
}

void CFolderView::setImageList( UINT wBMID, int cx, int cy, int iNoImg )
{
	HIMAGELIST himl = ImageList_Create( cx, cy, ILC_COLORDDB, iNoImg, 0 );

	ImageList_Add( himl, LoadBitmap( m_hInstance, MAKEINTRESOURCE( wBMID ) ), NULL );
	
	SendMessage( m_hWnd, TVM_SETIMAGELIST, TVSIL_NORMAL , ( LPARAM ) ( HIMAGELIST ) himl );
}

void CFolderView::reset( void )
{
	SendMessage( m_hWnd, TVM_DELETEITEM, 0, (LPARAM) TVI_ROOT );
}

HTREEITEM CFolderView::addItem( string str, HTREEITEM hParent, int iImage, int iSelImage )
{
	TV_INSERTSTRUCT tvins;
	TV_ITEM tvi;

	tvi.pszText			= const_cast<char *> ( str.c_str() );
	tvi.iImage			= iImage;
	tvi.iSelectedImage	= iSelImage;
	tvi.mask			= TVIF_TEXT |
		TVIF_IMAGE |
		TVIF_SELECTEDIMAGE;

	tvins.hParent		= hParent;
	tvins.hInsertAfter	= TVI_LAST;
	tvins.item			= tvi;

	return TreeView_InsertItem( m_hWnd, &tvins );
}

HTREEITEM CFolderView::getSelection( void )
{
	return TreeView_GetSelection( m_hWnd );
}

TVITEM CFolderView::getItem( HTREEITEM hitem )
{
	TVITEM		item;
	char		text[255];

	item.hItem		=	hitem;
	item.cchTextMax	=	255;
	item.pszText	=	text;
	item.mask		=	TVIF_TEXT | 
		TVIF_CHILDREN | 
		TVIF_IMAGE | 
		TVIF_PARAM | 
		TVIF_SELECTEDIMAGE | 
		TVIF_STATE | 
		TVIF_TEXT;

	SendMessage( m_hWnd, TVM_GETITEM, 0, (LPARAM)&item );
	return item;
}

string CFolderView::getItemText( HTREEITEM hItem )
{
	TVITEM item;
	char text[ 255 ];
	item.hItem		=	hItem;
	item.cchTextMax	=	255;
	item.pszText	=	text;
	item.mask		=	TVIF_TEXT;

	if(SendMessage( m_hWnd, TVM_GETITEM, 0, (LPARAM)&item) )
		return text;
	return "";
}

HTREEITEM CFolderView::getParent( HTREEITEM hitem )
{
	return TreeView_GetParent( m_hWnd, hitem );
}

HTREEITEM CFolderView::getChild( HTREEITEM hitem )
{
	return TreeView_GetChild( m_hWnd, hitem );
}

void CFolderView::expand( HTREEITEM hItem )
{
	TreeView_Expand( m_hWnd, hItem, TVE_EXPAND  );
}