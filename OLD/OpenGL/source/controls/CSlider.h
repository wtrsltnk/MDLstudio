#ifndef CSLIDER_H
#define CSLIDER_H

#include <windows.h>
#include <commctrl.h>
#include <string>
#include "CControl.h"
using namespace std;

class CSlider : public CControl
{
private:
public:
	CSlider( HINSTANCE, HWND, int, int, int, int, int );
	CSlider( HINSTANCE, HWND );

	void create( void );
	void create( DWORD );
	void create( DWORD, DWORD );
	void init( int min, int max );
	void reset( void );

	void setSel( int sel );
	int getSel( void );
};
#endif
