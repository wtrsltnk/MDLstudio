#ifndef CFILEVIEW_H
#define CFILEVIEW_H

#include <windows.h>
#include <commctrl.h>
#include <string>
#include "CControl.h"
using namespace std;

class CFileView : public CControl
{
private:
	DWORD				style;
public:
	CFileView( HINSTANCE, HWND, int, int, int, int, int );
	CFileView( HINSTANCE, HWND );

	void create( void );
	void create( DWORD );
	void create( DWORD, DWORD );
	void reset( void );
	void setStyleEx( DWORD );
	void setStyle( DWORD );
	DWORD getStyle( void );

	void initFiles( string strFilePath );

	void setImageList( UINT, int, int, int );

	void addColumn( string, int, int );
	void setColumnWidth( int, int );
	int getColumnWidth( int );

	void addItem( string, int, int );
	void addSubItem( string, int, int );

	int getSelectedIndex( void );
	string getItemText( int );
	string getSubItemText( int, int );
};
#endif
