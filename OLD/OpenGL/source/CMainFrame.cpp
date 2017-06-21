#include "CMainFrame.h"
#include "Settings.h"

#include <direct.h>
#include <time.h>
#include <strstream>
using namespace std;

/////////////////////////////////////
// Constructors / Destructors      //
/////////////////////////////////////
CMainFrame::CMainFrame()
{
	GetAppPath( GetCommandLine(), m_pAppPath );
	strcpy(m_pSettings, m_pAppPath);
	strcat( m_pSettings, "\\settings.set" );

	this->m_hWnd = NULL;
	this->m_dwCreationFlags  = 0L;
	this->m_dwWindowStyle	 = WS_OVERLAPPEDWINDOW;
	this->m_dwExWindowStyle	 = WS_EX_WINDOWEDGE;
	this->m_dwCreationFlags  = SW_SHOW;
	this->m_PosX			 = CW_USEDEFAULT;	
	this->m_PosY			 = CW_USEDEFAULT;	
	this->m_dwCreationWidth  = CW_USEDEFAULT;
	this->m_dwCreationHeight = CW_USEDEFAULT;
	this->m_hbrWindowColor	 = (HBRUSH)(COLOR_WINDOW);
	this->m_hIcon			 = LoadIcon(m_hInstance, MAKEINTRESOURCE(IDI_ENERGY));
	this->m_strWindowTitle	 = "MDL studio beta";
	this->m_hMenu			 = LoadMenu( m_hInstance, MAKEINTRESOURCE(IDR_MENU) );

	this->lpCmdLine			 = GetCommandLine();
	this->lpParameters		 = GetParameters( lpCmdLine );
	this->bAnimate			 = true;

	if ( LoadSettings( m_pSettings ) == 0 )
		InitSettings();
}


CMainFrame::~CMainFrame()
{
	delete m_pOpenGl;
	delete m_pOpenSaveDlg;
}

/////////////////////////////////////
// Memberfuncties                  //
/////////////////////////////////////

LRESULT CMainFrame::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nWidth, nHeight;
	char *result;
	string seltext, partext;
	CHOOSECOLOR cc;  
	static COLORREF custColors[16];
	

	switch ( uMsg )
	{
	case WM_CREATE:
		HFONT hFont;

		m_pInfo = NULL;
		m_pOpenSaveDlg = NULL;

		m_pOpenGl = new COpenGl( m_hInstance, m_hWnd, 10, 10, 200, 200 );
		m_pOpenGl->create();

		m_pModelView = new CTreeView( m_hInstance, m_hWnd, 10, 10, 200, 200, ID_MODELVIEW );
		m_pModelView->create( WS_EX_STATICEDGE, TVS_HASLINES | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_SHOWSELALWAYS );

		m_pHideButton = new CButton( m_hInstance, m_hWnd, 10, 10, 200, 200, ID_HIDEBUTTON );
		m_pHideButton->create( WS_EX_WINDOWEDGE, BS_PUSHLIKE );
		m_pHideButton->setText( "<" );
		hFont = CreateFont(10,5,0,0,FW_LIGHT,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_OUTLINE_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,NULL,"Fixed days" );
		m_pHideButton->setFont( hFont );

		m_pSlider = new CSlider( m_hInstance, m_hWnd, 0, 0, 200, 200, ID_SLIDER );
		m_pSlider->create( NULL, TBS_AUTOTICKS | TBS_ENABLESELRANGE );

		m_pStatus = new CStatusBar( m_hInstance, m_hWnd, ID_STATUSBAR );
		m_pStatus->create();
		m_pStatus->addItem( "no faces", 200 );

		m_pController = new CSlider( m_hInstance, m_hWnd, 0, 0, 200, 200, ID_CONTROLLER );
		m_pController->create( NULL, TBS_ENABLESELRANGE );
		m_pController->init(0, 1);
		m_pController->setSel( 0 );

		m_pStart = new CButton( m_hInstance, m_hWnd, 0, 0, 200, 200, ID_START );
		m_pStart->create();
		m_pStart->setText( "Stop" );
		m_pStart->setFont( m_pModelView->getFont() );

		m_pSpeed = new CSlider( m_hInstance, m_hWnd, 0, 0, 200, 200, ID_SPEED );
		m_pSpeed->create( NULL, TBS_ENABLESELRANGE );
		m_pSpeed->init(0, 100);
		m_pSpeed->setSel( 10 );

		m_pSlow = new CLabel( m_hInstance, m_hWnd, 0, 0, 200, 200, ID_SLOW );
		m_pSlow->create();
		m_pSlow->setText( "Slow" );
		m_pSlow->setFont( m_pModelView->getFont() );

		m_pFast = new CLabel( m_hInstance, m_hWnd, 0, 0, 200, 200, ID_FAST );
		m_pFast->create();
		m_pFast->setText( "Fast" );
		m_pFast->setFont( m_pModelView->getFont() );

		if ( settings.show_bones )
			CheckMenuItem( this->m_hMenu, IDM_INSTELLINGEN_BONES, MF_CHECKED );

		if ( settings.show_hitbox )
			CheckMenuItem( this->m_hMenu, IDM_INSTELLINGEN_HITBOXES, MF_CHECKED );

		if ( settings.show_atach )
			CheckMenuItem( this->m_hMenu, IDM_INSTELLINGEN_ATACHMENTS, MF_CHECKED );

		if ( settings.zoom == 0.5f )
			CheckMenuItem( this->m_hMenu, IDM_MATERIAL_ZOOM_50, MF_CHECKED );

		if ( settings.zoom == 1.0f )
			CheckMenuItem( this->m_hMenu, IDM_MATERIAL_ZOOM_100, MF_CHECKED );

		if ( settings.zoom == 1.5f )
			CheckMenuItem( this->m_hMenu, IDM_MATERIAL_ZOOM_150, MF_CHECKED );

		if ( settings.zoom == 2.0f )
			CheckMenuItem( this->m_hMenu, IDM_MATERIAL_ZOOM_200, MF_CHECKED );

		if ( settings.zoom == 3.0f )
			CheckMenuItem( this->m_hMenu, IDM_MATERIAL_ZOOM_300, MF_CHECKED );

		if ( lpParameters != NULL )
		{	
			try
			{
				load( lpParameters );
				SetTimer(hWnd, ID_TIME, TIMER_INTERVAL, NULL);
				break;
			}
			catch ( char *error )
			{
				MessageBox( m_hWnd, error, "Error:", MB_OK );
				break;
			}
		}
		SetTimer(hWnd, ID_TIME, TIMER_INTERVAL, NULL);
		break;

	case WM_TIMER:
		switch (wParam)
		{
		case ID_TIME:
			m_pOpenGl->draw();
			if ( bAnimate )
				m_pOpenGl->setSlider( m_pSlider );
			char str[255];
			sprintf(str, "%i faces drawn", m_pOpenGl->getPolycount() );
			m_pStatus->setItemText( str, 1 );
			break;
		}
		break;

	case WM_SIZE:
		nWidth = LOWORD(lParam);
		nHeight = HIWORD(lParam);
		this->m_nWidth = nWidth;
		this->m_nHeight = nHeight;
		if ( settings.hidepanel )
		{
			m_iHideBut = 0;
			m_iModelViewWidth = 0;
			m_iGlX = 15;
			m_pHideButton->setText( ">" );
		}
		else
		{
			m_iHideBut = 203;
			m_iModelViewWidth = 215;
			m_iGlX = 220;
			m_pHideButton->setText( "<" );
		}
		m_pOpenGl->resize( m_iGlX, 0, m_nWidth-m_iGlX, m_nHeight-50 );
		m_pHideButton->resize( m_iHideBut, 0, 12, 12 );
		m_pModelView->resize( 0, 15, m_iModelViewWidth, m_nHeight-155 );
		m_pSlider->resize( 0, m_nHeight-50, m_nWidth, 30 );
		
		m_pController->resize( 22, m_nHeight-130, 22 *8, 20 );
		m_pStart->resize( 110-(220 / 6), m_nHeight-105, (220 / 3), 20 );
		m_pSpeed->resize( 22, m_nHeight-80, 22*8, 20 );
		m_pSlow->resize( 22, m_nHeight-95, 30, 15 );
		m_pFast->resize( 220 - 22 - 20, m_nHeight-95, 30, 15 );
		
		m_pStatus->resize( wParam, lParam );
		m_pOpenGl->draw();
		break;

	case WM_HSCROLL:
		if ( (HWND)lParam == m_pSlider->getHandle() )
		{
			m_pOpenGl->setFrame( m_pSlider->getSel() );
		}
		else if ( (HWND)lParam == m_pSpeed->getHandle() )
		{
			m_pOpenGl->setAnimspeed( (float)m_pSpeed->getSel() / 10 );
		}
		else if ( (HWND)lParam == m_pController->getHandle() )
		{
			studiohdr_t *hdr = m_pOpenGl->getModel().getStudioHeader ();
			if (hdr)
			{
				mstudiobonecontroller_t *pbonecontrollers = (mstudiobonecontroller_t *) ((byte *) hdr + hdr->bonecontrollerindex);
				if ( m_nController == 4 )
					m_pOpenGl->setMouth ( m_pController->getSel() );
				else
					m_pOpenGl->setController ( m_nController, m_pController->getSel() );
			}	 
		}
		break;

	case WM_DESTROY:
		SaveSettings( m_pSettings );
		break;

	case WM_COMMAND:
		switch ( LOWORD(wParam) )
		{
		case ID_HIDEBUTTON:
			switch ( HIWORD(wParam) )
			{
			case BN_CLICKED:
				settings.hidepanel = !settings.hidepanel;
				if ( settings.hidepanel )
				{
					m_iHideBut = 0;
					m_iModelViewWidth = 0;
					m_iGlX = 15;
					m_pHideButton->setText( ">" );
				}
				else
				{
					m_iHideBut = 203;
					m_iModelViewWidth = 215;
					m_iGlX = 220;
					m_pHideButton->setText( "<" );
				}
				m_pOpenGl->resize( m_iGlX, 0, m_nWidth-m_iGlX, m_nHeight-50 );
				m_pHideButton->resize( m_iHideBut, 0, 12, 12 );
				m_pModelView->resize( 0, 15, m_iModelViewWidth, m_nHeight-155 );
				m_pSlider->resize( 0, m_nHeight-50, m_nWidth, 30 );
				SetFocus( m_hWnd );
				break;
			}
			break;

		case ID_START:
			switch ( HIWORD(wParam) )
			{
			case BN_CLICKED:
				bAnimate = !bAnimate;
				if ( !bAnimate )
					m_pStart->setText( "Start" );
				else
					m_pStart->setText( "Stop" );
				m_pOpenGl->setAnimate( bAnimate );
				break;
			}
			break;

		case IDM_BESTAND_OPEN:
			m_pOpenSaveDlg = new COpenSaveDlg( m_hInstance, m_hWnd );
			result = m_pOpenSaveDlg->open( "Open HL-Model file", "MDL file(*.mdl)\0*.mdl", "mdl" );
			if ( strcmp( result, "" ) != 0 )
			{
				KillTimer( hWnd, ID_TIME );
				try
				{
					load( result );
				}
				catch ( char *error )
				{
					MessageBox( m_hWnd, error, "Error:", MB_OK );
					SetTimer(hWnd, ID_TIME, TIMER_INTERVAL, NULL);
					break;
				}
				SetTimer(hWnd, ID_TIME, TIMER_INTERVAL, NULL);
			}
			delete m_pOpenSaveDlg;
			m_pOpenSaveDlg = NULL;
			break;

		case IDM_BESTAND_SLUITEN:
			m_pOpenGl->unload();
			m_pModelView->reset();
			SendMessage( m_hWnd, WM_SETTEXT, 0, (LPARAM)(LPTSTR)m_strWindowTitle );
			break;

		case IDM_BESTAND_AFSLUITEN:
			PostQuitMessage( 0 );
			break;

		case IDM_INSTELLINGEN_LIGHTCOLOR:
			cc.lStructSize = sizeof (CHOOSECOLOR);
			cc.hwndOwner = hWnd;
			cc.rgbResult = RGB (settings.lightcolor[0]*255, settings.lightcolor[1]*255, settings.lightcolor[2]*255);
			cc.lpCustColors = custColors;
			cc.Flags = CC_ANYCOLOR | CC_FULLOPEN | CC_RGBINIT;

			if (ChooseColor (&cc))
			{
				settings.lightcolor[0] = (float) GetRValue (cc.rgbResult) / 255;
				settings.lightcolor[1] = (float) GetGValue (cc.rgbResult) / 255;
				settings.lightcolor[2] = (float) GetBValue (cc.rgbResult) / 255;
			}
			break;

		case IDM_INSTELLINGEN_BGCOLOR:
			cc.lStructSize = sizeof (CHOOSECOLOR);
			cc.hwndOwner = hWnd;
			cc.rgbResult = RGB (settings.bgcolor[0]*255, settings.bgcolor[1]*255, settings.bgcolor[2]*255);
			cc.lpCustColors = custColors;
			cc.Flags = CC_ANYCOLOR | CC_FULLOPEN | CC_RGBINIT;

			if (ChooseColor (&cc))
			{
				settings.bgcolor[0] = (float) GetRValue (cc.rgbResult) / 255;
				settings.bgcolor[1] = (float) GetGValue (cc.rgbResult) / 255;
				settings.bgcolor[2] = (float) GetBValue (cc.rgbResult) / 255;
			}
			break;

		case IDM_INSTELLINGEN_BONES:
			settings.show_bones = !settings.show_bones;
			if ( settings.show_bones )
				CheckMenuItem( this->m_hMenu, IDM_INSTELLINGEN_BONES, MF_CHECKED );
			else
				CheckMenuItem( this->m_hMenu, IDM_INSTELLINGEN_BONES, MF_UNCHECKED );
			break;

		case IDM_INSTELLINGEN_HITBOXES:
			settings .show_hitbox = !settings .show_hitbox;
			if ( settings .show_hitbox )
				CheckMenuItem( this->m_hMenu, IDM_INSTELLINGEN_HITBOXES, MF_CHECKED );
			else
				CheckMenuItem( this->m_hMenu, IDM_INSTELLINGEN_HITBOXES, MF_UNCHECKED );
			break;

		case IDM_INSTELLINGEN_ATACHMENTS:
			settings.show_atach = !settings.show_atach;
			if ( settings.show_atach )
				CheckMenuItem( this->m_hMenu, IDM_INSTELLINGEN_ATACHMENTS, MF_CHECKED );
			else
				CheckMenuItem( this->m_hMenu, IDM_INSTELLINGEN_ATACHMENTS, MF_UNCHECKED );
			break;

		case IDM_VIEW_CENTER:
			m_pOpenGl->resetView( VIEWTYPE_CENTER );
			break;

		case IDM_VIEW_PLAYER:
			m_pOpenGl->resetView( VIEWTYPE_PLAYER );
			break;

		case IDM_RENDERMODES_VERTEX:
			m_pOpenGl->setRenderMode( RENDERMODE_VERTEX );
			CheckMenuItem( this->m_hMenu, IDM_RENDERMODES_VERTEX, MF_CHECKED );
			CheckMenuItem( this->m_hMenu, IDM_RENDERMODES_WIREFRAME, MF_UNCHECKED );
			CheckMenuItem( this->m_hMenu, IDM_RENDERMODES_FLATSHADED, MF_UNCHECKED );
			CheckMenuItem( this->m_hMenu, IDM_RENDERMODES_SMOOTHSHADED, MF_UNCHECKED );
			CheckMenuItem( this->m_hMenu, IDM_RENDERMODE_TEXTURED, MF_UNCHECKED );
			break;

		case IDM_RENDERMODES_WIREFRAME:
			m_pOpenGl->setRenderMode( RENDERMODE_WIREFRAME );
			CheckMenuItem( this->m_hMenu, IDM_RENDERMODES_VERTEX, MF_UNCHECKED );
			CheckMenuItem( this->m_hMenu, IDM_RENDERMODES_WIREFRAME, MF_CHECKED );
			CheckMenuItem( this->m_hMenu, IDM_RENDERMODES_FLATSHADED, MF_UNCHECKED );
			CheckMenuItem( this->m_hMenu, IDM_RENDERMODES_SMOOTHSHADED, MF_UNCHECKED );
			CheckMenuItem( this->m_hMenu, IDM_RENDERMODE_TEXTURED, MF_UNCHECKED );
			break;

		case IDM_RENDERMODES_FLATSHADED:
			m_pOpenGl->setRenderMode( RENDERMODE_FLAT );
			CheckMenuItem( this->m_hMenu, IDM_RENDERMODES_VERTEX, MF_UNCHECKED );
			CheckMenuItem( this->m_hMenu, IDM_RENDERMODES_WIREFRAME, MF_UNCHECKED );
			CheckMenuItem( this->m_hMenu, IDM_RENDERMODES_FLATSHADED, MF_CHECKED );
			CheckMenuItem( this->m_hMenu, IDM_RENDERMODES_SMOOTHSHADED, MF_UNCHECKED );
			CheckMenuItem( this->m_hMenu, IDM_RENDERMODE_TEXTURED, MF_UNCHECKED );
			break;

		case IDM_RENDERMODES_SMOOTHSHADED:
			m_pOpenGl->setRenderMode( RENDERMODE_SMOOTH );
			CheckMenuItem( this->m_hMenu, IDM_RENDERMODES_VERTEX, MF_UNCHECKED );
			CheckMenuItem( this->m_hMenu, IDM_RENDERMODES_WIREFRAME, MF_UNCHECKED );
			CheckMenuItem( this->m_hMenu, IDM_RENDERMODES_FLATSHADED, MF_UNCHECKED );
			CheckMenuItem( this->m_hMenu, IDM_RENDERMODES_SMOOTHSHADED, MF_CHECKED );
			CheckMenuItem( this->m_hMenu, IDM_RENDERMODE_TEXTURED, MF_UNCHECKED );
			break;

		case IDM_RENDERMODE_TEXTURED:
			m_pOpenGl->setRenderMode( RENDERMODE_TEXTURE );
			CheckMenuItem( this->m_hMenu, IDM_RENDERMODES_VERTEX, MF_UNCHECKED );
			CheckMenuItem( this->m_hMenu, IDM_RENDERMODES_WIREFRAME, MF_UNCHECKED );
			CheckMenuItem( this->m_hMenu, IDM_RENDERMODES_FLATSHADED, MF_UNCHECKED );
			CheckMenuItem( this->m_hMenu, IDM_RENDERMODES_SMOOTHSHADED, MF_UNCHECKED );
			CheckMenuItem( this->m_hMenu, IDM_RENDERMODE_TEXTURED, MF_CHECKED );
			break;

		case IDM_MATERIAL_ZOOM_50:
			settings.zoom = 0.5f;
			CheckMenuItem( this->m_hMenu, IDM_MATERIAL_ZOOM_50, MF_CHECKED );
			CheckMenuItem( this->m_hMenu, IDM_MATERIAL_ZOOM_100, MF_UNCHECKED );
			CheckMenuItem( this->m_hMenu, IDM_MATERIAL_ZOOM_150, MF_UNCHECKED );
			CheckMenuItem( this->m_hMenu, IDM_MATERIAL_ZOOM_200, MF_UNCHECKED );
			CheckMenuItem( this->m_hMenu, IDM_MATERIAL_ZOOM_300, MF_UNCHECKED );
			break;

		case IDM_MATERIAL_ZOOM_100:
			settings.zoom = 1.0f;
			CheckMenuItem( this->m_hMenu, IDM_MATERIAL_ZOOM_50, MF_UNCHECKED );
			CheckMenuItem( this->m_hMenu, IDM_MATERIAL_ZOOM_100, MF_CHECKED );
			CheckMenuItem( this->m_hMenu, IDM_MATERIAL_ZOOM_150, MF_UNCHECKED );
			CheckMenuItem( this->m_hMenu, IDM_MATERIAL_ZOOM_200, MF_UNCHECKED );
			CheckMenuItem( this->m_hMenu, IDM_MATERIAL_ZOOM_300, MF_UNCHECKED );
			break;

		case IDM_MATERIAL_ZOOM_150:
			settings.zoom = 1.5f;
			CheckMenuItem( this->m_hMenu, IDM_MATERIAL_ZOOM_50, MF_UNCHECKED );
			CheckMenuItem( this->m_hMenu, IDM_MATERIAL_ZOOM_100, MF_UNCHECKED );
			CheckMenuItem( this->m_hMenu, IDM_MATERIAL_ZOOM_150, MF_CHECKED );
			CheckMenuItem( this->m_hMenu, IDM_MATERIAL_ZOOM_200, MF_UNCHECKED );
			CheckMenuItem( this->m_hMenu, IDM_MATERIAL_ZOOM_300, MF_UNCHECKED );
			break;

		case IDM_MATERIAL_ZOOM_200:
			settings.zoom = 2.0f;
			CheckMenuItem( this->m_hMenu, IDM_MATERIAL_ZOOM_50, MF_UNCHECKED );
			CheckMenuItem( this->m_hMenu, IDM_MATERIAL_ZOOM_100, MF_UNCHECKED );
			CheckMenuItem( this->m_hMenu, IDM_MATERIAL_ZOOM_150, MF_UNCHECKED );
			CheckMenuItem( this->m_hMenu, IDM_MATERIAL_ZOOM_200, MF_CHECKED );
			CheckMenuItem( this->m_hMenu, IDM_MATERIAL_ZOOM_300, MF_UNCHECKED );
			break;

		case IDM_MATERIAL_ZOOM_300:
			settings.zoom = 3.0f;
			CheckMenuItem( this->m_hMenu, IDM_MATERIAL_ZOOM_50, MF_UNCHECKED );
			CheckMenuItem( this->m_hMenu, IDM_MATERIAL_ZOOM_100, MF_UNCHECKED );
			CheckMenuItem( this->m_hMenu, IDM_MATERIAL_ZOOM_150, MF_UNCHECKED );
			CheckMenuItem( this->m_hMenu, IDM_MATERIAL_ZOOM_200, MF_UNCHECKED );
			CheckMenuItem( this->m_hMenu, IDM_MATERIAL_ZOOM_300, MF_CHECKED );
			break;

		case IDM_HELP_INFO:
			m_pInfo = new CInfoDlg( m_hInstance, m_hWnd );
			m_pInfo->show();
			delete m_pInfo;
			m_pInfo = NULL;
			break;
		}
		break;

	case WM_NOTIFY:
		switch ( wParam )
		{
		case ID_MODELVIEW:
			switch ( ((NMHDR*)lParam)->code )
			{
			case TVN_SELCHANGED:
				StudioModel temp;
				int i;
				temp = m_pOpenGl->getModel();
				seltext = m_pModelView->getItemText( m_pModelView->getSelection() );

				KillTimer( m_hWnd, ID_TIME );


				// REFERENCE
				if ( m_pModelView->getParent( m_pModelView->getSelection() ) == m_hReference )
				{
					studiohdr_t *hdr = temp.getStudioHeader ();
					if ( hdr )
					{
						mstudiobodyparts_t *pbodyparts = (mstudiobodyparts_t *) ((byte *) hdr + hdr->bodypartindex);
						for ( i = 0; i < hdr->numbodyparts; i++ )
						{
							if ( strcmp(pbodyparts[i].name, seltext.c_str()) == 0 )
							{
								m_pOpenGl->setBodygroup( i, 0 );
							}
						}
					}
				}

				// REFERENCE SUBMODEL
				else if ( m_pModelView->getParent( m_pModelView->getParent( m_pModelView->getSelection() ) ) == m_hReference )
				{
					partext = m_pModelView->getItemText( m_pModelView->getParent( m_pModelView->getSelection() ) );
					studiohdr_t *hdr = temp.getStudioHeader ();
					if ( hdr )
					{
						mstudiobodyparts_t *pbodyparts = (mstudiobodyparts_t *) ((byte *) hdr + hdr->bodypartindex);
						for ( i = 0; i < hdr->numbodyparts; i++ )
						{
							if ( strcmp(pbodyparts[i].name, partext.c_str()) == 0 )
							{
								m_pOpenGl->setBodygroup( i, getIndex(seltext) );
							}
						}
					}
				}

				// SKINS
				else if ( m_pModelView->getParent( m_pModelView->getSelection() ) == m_hSkins )
				{
					m_pOpenGl->setSkin( getIndex( seltext ) );
				}

				// CONTROLLER
				else if ( m_pModelView->getParent( m_pModelView->getSelection() ) == m_hControllers )
				{
					studiohdr_t *hdr = temp.getStudioHeader ();	
					if (hdr)
					{
						mstudiobonecontroller_t *pbonecontrollers = (mstudiobonecontroller_t *) ((byte *) hdr + hdr->bonecontrollerindex);
						
						if ( strstr(seltext.c_str(), "Mouth") != NULL )
						{
							m_nController = 4;
							m_pOpenGl->setMouth( 0.0f );
						}
						else
						{
							m_nController = getIndex( seltext );
							m_pOpenGl->setController( m_nController, 0.0f );
						}	
						m_pController->init((int) pbonecontrollers[getIndex( seltext )].start, (int) pbonecontrollers[getIndex( seltext )].end);	
					}
				}

				// SEQUENCE
				else if ( m_pModelView->getParent( m_pModelView->getSelection() ) == m_hSequence )
				{
					studiohdr_t *hdr = temp.getStudioHeader();
					if ( hdr )
					{
						mstudioseqdesc_t *pseqdescs = (mstudioseqdesc_t *) ((byte *) hdr + hdr->seqindex);
						for ( i = 0; i < hdr->numseq; i++ )
						{
							if ( strcmp(pseqdescs[i].label, seltext.c_str()) == 0 )
							{
								m_pOpenGl->setSequence( i );
								m_pSlider->init( 0, pseqdescs[i].numframes-1 );
							}
						}
					}
				}

				// BONES
				else if ( m_pModelView->getParent( m_pModelView->getSelection() ) == m_hBones )
				{
					studiohdr_t *hdr = temp.getStudioHeader();
					if ( hdr )
					{
						mstudiobone_t *pbones = (mstudiobone_t *) ((byte *) hdr + hdr->boneindex);
						for ( i = 0; i < hdr->numbones; i++ )
						{
							if ( strcmp(pbones[i].name, seltext.c_str()) == 0 )
							{
								m_pOpenGl->setSelBone( i );
							}
						}
					}
				}

				// MATERIALS
				else if ( m_pModelView->getParent( m_pModelView->getSelection() ) == m_hMaterial )
				{
					studiohdr_t *hdr = temp.getTextureHeader ();
					if (hdr)
					{
						mstudiotexture_t *ptextures = (mstudiotexture_t *) ((byte *) hdr + hdr->textureindex);
						for (i = 0; i < hdr->numtextures; i++)
						{
							if ( strcmp(ptextures[i].name, seltext.c_str()) == 0 )
							{
								m_pOpenGl->setMaterial( i );
								m_pOpenGl->resetView( VIEWTYPE_MATERIAL );
								SetTimer(hWnd, ID_TIME, TIMER_INTERVAL, NULL);
								return FALSE;
							}
						}
					}
				}

				// DESELBONES
				else if ( strcmp(seltext.c_str(), "Bones") == 0 )
				{
					m_pOpenGl->setSelBone( -1 );
				}

				// HITBOXES
				else if ( m_pModelView->getParent( m_pModelView->getSelection() ) == m_hHitboxes )
				{
					m_pOpenGl->setSelHitbox( getIndex( seltext ) );
				}

				// DESELHITBOXES
				else if ( strcmp(seltext.c_str(), "Hitboxes") == 0 )
				{
					m_pOpenGl->setSelHitbox( -1 );
				}
				
				m_pOpenGl->resetView( VIEWTYPE_NORMAL );
				SetTimer(hWnd, ID_TIME, TIMER_INTERVAL, NULL);
				break;

			case NM_RCLICK:
				MessageBox( m_hWnd, "R-Click", "Test", MB_OK );
				return FALSE;
			}
			break;
		}
		break;
	}
	return CWin::MsgProc(hWnd, uMsg, wParam, lParam);
}

void CMainFrame::load( char *filename ) throw()
{
	char str[64];
	HTREEITEM hItem;
	int i, j;
	
	try
	{
		m_pOpenGl->load( filename );
	}
	catch ( char *error )
	{
		throw( error );
		return;
	}
	m_pOpenGl->resetView( VIEWTYPE_CENTER );
	m_pOpenGl->setRenderMode( RENDERMODE_TEXTURE );

	m_pModelView->reset();
	char *title;
	title = new char[strlen(filename)+strlen(this->m_strWindowTitle)+3];
	strcpy(title, filename);
	strcat(title, " - ");
	strcat(title, this->m_strWindowTitle);
	SendMessage( m_hWnd, WM_SETTEXT, 0, (LPARAM)(LPTSTR)title );
	
	StudioModel temp;
	temp = m_pOpenGl->getModel();
	

	// REFERENCE
	studiohdr_t *hdr = temp.getStudioHeader ();
	m_hReference = m_pModelView->addItem( "References", TVGN_ROOT, 0 );
	mstudiobodyparts_t *pbodyparts = (mstudiobodyparts_t *) ((byte *) hdr + hdr->bodypartindex);
	if (hdr->numbodyparts > 0)
	{
		for (i = 0; i < hdr->numbodyparts; i++)
		{
			hItem = m_pModelView->addItem( pbodyparts[i].name, m_hReference, 0 );
			for (j = 0; j < pbodyparts[i].nummodels; j++)
			{
				sprintf (str, "Submodel %d", j);
				m_pModelView->addItem( str, hItem, 0 );
			}
		}
	}
	// END REFERENCE

	// SKIN
	m_hSkins = m_pModelView->addItem( "Skins", TVGN_ROOT, 0 );
	if (hdr)
	{
		for (int i = 0; i < hdr->numskinfamilies; i++)
		{
			sprintf (str, "Skin %d", i + 1);
			m_pModelView->addItem( str, m_hSkins, 0 );
		}
	}
	// END SKIN

	// CONTROLLER
	m_hControllers = m_pModelView->addItem( "Controllers", TVGN_ROOT, 0 );
	if (hdr)
	{
		mstudiobonecontroller_t *pbonecontrollers = (mstudiobonecontroller_t *) ((byte *) hdr + hdr->bonecontrollerindex);
		for (int i = 0; i < hdr->numbonecontrollers; i++)
		{
			if (pbonecontrollers[i].index == 4)
				sprintf (str, "Mouth %d", i);
			else
				sprintf (str, "Controller %d", i);
			m_pModelView->addItem( str, m_hControllers, 0 );
		}
	}
	// END CONTROLLER

	hdr = temp.getStudioHeader();

	// SEQUENCE
	m_hSequence = m_pModelView->addItem( "Sequences", TVGN_ROOT, 0 );
	if ( hdr )
	{
		mstudioseqdesc_t *pseqdescs = (mstudioseqdesc_t *) ((byte *) hdr + hdr->seqindex);
		for (i = 0; i < hdr->numseq; i++)
		{
			HTREEITEM hSequence = m_pModelView->addItem( pseqdescs[i].label, m_hSequence, 0 );
		}
		m_pSlider->init( 0, pseqdescs[0].numframes-1 );
	}
	// END SEQUENCE

	// MATERIAL
	hdr = temp.getTextureHeader ();
	m_hMaterial = m_pModelView->addItem( "Materials", TVGN_ROOT, 0 );
	if (hdr)
	{
		mstudiotexture_t *ptextures = (mstudiotexture_t *) ((byte *) hdr + hdr->textureindex);
		for (i = 0; i < hdr->numtextures; i++)
		{
			m_pModelView->addItem( ptextures[i].name, m_hMaterial, 0 );
		}
	}
	// END MATERIAL

	// BONES
	m_hBones = m_pModelView->addItem( "Bones", TVGN_ROOT, 0 );
	if ( hdr )
	{
		mstudiobone_t *pbones = (mstudiobone_t *) ((byte *) hdr + hdr->boneindex);
		for ( i = 0; i < hdr->numbones; i++ )
		{
			m_pModelView->addItem( pbones[i].name, m_hBones, 0 );
		}
	}
	// END BONES

	// HITBOXES
	m_hHitboxes = m_pModelView->addItem( "Hitboxes", TVGN_ROOT, 0 );
	if ( hdr )
	{
		mstudiobbox_t *pbboxes = (mstudiobbox_t *) ((byte *) hdr + hdr->hitboxindex);
		for (i = 0; i < hdr->numhitboxes; i++)
		{
			sprintf (str, "Hitbox %d", i);
			m_pModelView->addItem( str, m_hHitboxes, 0 );
		}
	}
	// END HITBOXES
}

int CMainFrame::getIndex( string str )
{
	char *result;
	char *temp;

	temp = const_cast <char*>(str.c_str());

	for ( int i = strlen(temp)-1; i > 0; i-- )
	{
		if ( !(temp[i] < 58 && temp[i] > 47) )
		{
			result = new char[strlen(temp)-i];
			for ( int j = 0; j < strlen(temp)-i; j++ )
			{
				result[j] = temp[j+i+1];
			}
			result[j] += '\0';
			return atoi(result);
		}
	}
	return 0;
}

char *CMainFrame::GetParameters( char *cmdline )
{
	char *result;
	int cmdlen;

	cmdlen = strlen(cmdline);

	if ( cmdline[cmdlen-1] == '\"' )
	{
		for ( int i = cmdlen-2; i >= 0; i-- )
		{
			if ( cmdline[i] == '\"' )
			{
				if ( i == 0 )
					return NULL;
				result = new char[cmdlen-i-2];
				for ( int j = 1; j < cmdlen - i - 1; j++ )
				{
					result[j-1] = cmdline[j+i];
				}
				result[j-1] = '\0';
				return result;
			}
		}
	}
	return NULL;
}

void CMainFrame::GetAppPath( char *cmdline, char *dest )
{
	int cmdlen;
	int i, j, k;

	cmdlen = strlen(cmdline);

	if ( cmdline[0] == '\"' )
	{
		for ( i = 1; i < cmdlen; i++ )
		{
			if ( cmdline[i] == '\"' )
			{
				for ( j = 0; j < i; j++ )
				{
					dest[j] = cmdline[j+1];
				}
				for ( k = j; k > 0; k-- )
				{
					if ( dest[k] == '\\' )
					{
						dest[k] = '\0';
						return;
					}
				}
			}
		}
	}
}