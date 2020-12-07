#ifndef CCOMBO_H
#define CCOMBO_H

#include <windows.h>
#include <commctrl.h>
#include <string>
#include "CControl.h"

class CCombo : public CControl
{
private:
public:
	CCombo( HINSTANCE, HWND, int, int, int, int, int );
	CCombo( HINSTANCE, HWND );

	void create( void );
	void create( DWORD );
	void create( DWORD, DWORD );

	void addItem( string );
	bool remItem( int );
	string getItem( int );

	string getSelected( void );
};
#endif
