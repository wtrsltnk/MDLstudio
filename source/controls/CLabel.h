#ifndef CLABEL_H
#define CLABEL_H

#include <windows.h>
#include <commctrl.h>
#include <string>
#include "CControl.h"
using namespace std;

class CLabel : public CControl
{
private:
public:
	CLabel( HINSTANCE, HWND, int, int, int, int, int );
	CLabel( HINSTANCE, HWND );
	void create( void );
	void create( DWORD );
	void create( DWORD, DWORD );
};
#endif
