// WinClass.cpp : Defines the entry point for the application.
//
#include "CMainFrame.h"

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	CMainFrame app;

	if (!app.Create())
		return 0;
	
	return app.Run();
}
