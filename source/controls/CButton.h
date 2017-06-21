#ifndef CBUTTON_H
#define CBUTTON_H

#include <windows.h>
#include <commctrl.h>
#include <string>
#include "CControl.h"
using namespace std;

class CButton: public CControl
{
private:
public:
	CButton( HINSTANCE, HWND, int, int, int, int, int );
	CButton( HINSTANCE, HWND );

	void create( void );
	void create( DWORD );
	void create( DWORD, DWORD );
};
#endif
