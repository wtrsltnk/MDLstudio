#ifndef CTOOLBAR_H
#define CTOOLBAR_H

#include <windows.h>
#include <commctrl.h>
#include <string>
#include "CControl.h"
using namespace std;

#define MAX_BUTTONS 10

class CToolBar : public CControl
{
private:
	TBBUTTON		tbb[ MAX_BUTTONS ];
	int				iNumButtons;
public:
	CToolBar( HINSTANCE, HWND, int );

	void create( UINT, int, int, int );
	void resize( WPARAM, LPARAM );

	bool addBitmaps( int, UINT );

	void addButton( char *, int, int );
	bool remButton( int );
	void addSeperator( void );
};
#endif
