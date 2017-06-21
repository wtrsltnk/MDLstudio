#ifndef CSTATUSBAR_H
#define CSTATUSBAR_H

#include <windows.h>
#include <string>
#include <commctrl.h>
#include "CControl.h"
using namespace std;

#define MAX_NUM_PARTS 25

class CStatusBar : public CControl
{
private:
	int				parts[ MAX_NUM_PARTS ];
	int				numparts;
public:
	CStatusBar( HINSTANCE, HWND, int );
	~CStatusBar( void );

	void create( void );
	void resize( WPARAM, LPARAM );

	void addItem( string, int );
	void addOneItem( string );

	void setItemText( string, int );
};
#endif