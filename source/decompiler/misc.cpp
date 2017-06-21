/*
==============================================================

	Half-Life MDL Decompiler
	2002, Kratisto. Based on code from HL SDK

==============================================================
*/

#include <errno.h>
#include <ctype.h>
#include <time.h>
#include <stdarg.h>

#include "mdldec.h"

void CMDLDecompiler::LogMessage (int type, char *msg, ...)
{
	va_list argptr;
	va_start (argptr,msg);

#ifdef MDLDEC_MS3DPLUGIN

	CString s;
	char s1[1024]; //FIX 

	vsprintf (s1, msg, argptr);
	s = s + s1;
	pLogDialog->WriteLine(s);
	pLogDialog->UpdateWindow(); 
	pLogDialog->UpdateDialogControls (pLogDialog, false);

#else
	vprintf (msg,argptr);

#endif
	va_end (argptr);
}

///////////////////////////////////////////////////////////////////

void MyExtractFileBase (char *path, char *dest)
{
	char    *src;

	src = path + strlen(path) - 1;

	// back up until a \ or the start

	while (src != path && !PATHSEPARATOR(*(src-1)))
		src--;

	while (*src && *src != '.')
	{
		*dest++ = *src++;
	}
	*dest = 0;
}

void MyExtractFilePath (char *path, char *dest)
{
	char    *src;

	src = path + strlen(path) - 1;

	// back up until a \ or the start

	while (src != path && !PATHSEPARATOR(*(src-1)))
		src--;

	memcpy (dest, path, src-path);
	dest[src-path] = 0;
}

///////////////////////////////////////////////////////////////////