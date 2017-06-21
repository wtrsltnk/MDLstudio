#ifndef CREPORTVIEW_H
#define CREPORTVIEW_H

#include <windows.h>
#include <commctrl.h>
#include <string>
#include "CControl.h"
using namespace std;

class CReportView : public CControl
{
private:
public:
	CReportView( HINSTANCE, HWND, int, int, int, int, int );
	CReportView( HINSTANCE, HWND );

	void create( void );
	void create( DWORD );
	void create( DWORD, DWORD );
	void reset( void );
	void setExStyle( DWORD );

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
