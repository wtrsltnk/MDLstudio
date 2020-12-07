#ifndef CFRAME_H
#define CFRAME_H

#include <windows.h>
#include <commctrl.h>
#include <string>
#include "CControl.h"

class CFrame: public CControl
{
private:
	HWND		*m_hOptions;
	int			optMax;
	int			optCount;
public:
	CFrame( HINSTANCE, HWND, int, int, int, int, int, int );
	CFrame( HINSTANCE, HWND );
	void create( void );
	void create( DWORD );
	void create( DWORD, DWORD );

	void addOption( string, int, int, int, int );

	string getOptionText( int );
	long setOptionText( int, string );

	int getOptionValue( int );
	void setOptionValue( int, int );

	int getSelOption( void );
};
#endif
