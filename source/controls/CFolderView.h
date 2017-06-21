#ifndef CFOLDERVIEW_H
#define CFOLDERVIEW_H

#include <windows.h>
#include <commctrl.h>
#include <string>
#include "CControl.h"
using namespace std;

#include "..\..\resource\resource.h"

class CFolderView : public CControl
{
private:
	HTREEITEM		hParentItem;

	HTREEITEM addItem( string, HTREEITEM, int iImage, int iSelImage );
	void addDummyNode( HTREEITEM hItem );

	void clearChilderen( HTREEITEM hItem );
	bool isDriveNode( HTREEITEM hItem );
	void setButtonState( HTREEITEM hItem, string strPathName );
	bool hasSubdirectory( string strPathName );

	void initIcons( void );
	void initMycomputer( void );
	void initDrives( void );

public:
	CFolderView( HINSTANCE, HWND, int, int, int, int, int );
	CFolderView( HINSTANCE, HWND );

	void create( void );
	void create( DWORD );
	void create( DWORD, DWORD );
	void reset( void );

	string initPath( WPARAM wParam, LPARAM lParam );

	string getItemPath( HTREEITEM hItem );
	string getSelItemPath( void );

	void setImageList( UINT, int, int, int );

	HTREEITEM getSelection( void );
	TVITEM getItem( HTREEITEM );
	string getItemText( HTREEITEM );
	HTREEITEM getParent( HTREEITEM );
	HTREEITEM getChild( HTREEITEM );

	void expand( HTREEITEM );
};
#endif
