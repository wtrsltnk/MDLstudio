#ifndef CEDIT_H
#define CEDIT_H

#include <windows.h>
#include <commctrl.h>
#include <string>
#include "CControl.h"

class CEdit : public CControl
{
private:
public:
	CEdit( HINSTANCE, HWND, int, int, int, int, int );
	CEdit( HINSTANCE, HWND );
	void create( void );
	void create( DWORD );
	void create( DWORD, DWORD );
};
#endif
