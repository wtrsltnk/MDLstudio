#ifndef CCHECK_H
#define CCHECK_H

#include <windows.h>
#include <commctrl.h>
#include <string>
#include "CControl.h"

class CCheck: public CControl
{
private:
public:
	CCheck( HINSTANCE, HWND, int, int, int, int, int );
	CCheck( HINSTANCE, HWND );

	void create( void );
	void create( DWORD );
	void create( DWORD, DWORD );

	void setValue( int );
	int getValue( void );
};
#endif
