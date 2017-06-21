#include "CSequenceDlg.h"
#include <stdio.h>


mstudioseqdesc_t *CSequenceDlg::m_pSeq = NULL;
mstudioevent_t *CSequenceDlg::m_pEvents = NULL;
HWND CSequenceDlg::m_hOwner = NULL;
/////////////////////////////////////
// Constructors / Destructors      //
/////////////////////////////////////

CSequenceDlg::CSequenceDlg( HINSTANCE hInstance, HWND hWnd )
{
	this->m_hOwner		= hWnd;
	this->m_hInstance	= hInstance;
}

CSequenceDlg::~CSequenceDlg( void )
{
}

/////////////////////////////////////
// Memberfuncties                  //
/////////////////////////////////////

BOOL CSequenceDlg::DlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	char str[255];
	char opt[64];
	int j, index;

	switch( uMsg )
	{
	case WM_INITDIALOG:
		sprintf( str, "%s", m_pSeq->label );
		SetDlgItemText( hDlg, IDC_SEQ_NAME, str );
		sprintf( str, "%f", m_pSeq->fps );
		SetDlgItemText( hDlg, IDC_SEQ_FPS, str );
		sprintf( str, "%i", m_pSeq->numframes );
		SetDlgItemText( hDlg, IDC_SEQ_FRAMES, str );

		for ( j = 0; j < m_pSeq->numevents; j++ ) 
		{
			sprintf(str,"event:%i @ frame:%i", m_pEvents[j].event,m_pEvents[j].frame);
			SendMessage( GetDlgItem(hDlg, IDC_SEQ_EVENTS),LB_ADDSTRING, 0, (LPARAM)(LPTSTR)str );
		}
		break;

	case WM_COMMAND:
		switch ( LOWORD(wParam) )
		{
		case IDOK:
			switch ( HIWORD(wParam))
			{
			case BN_CLICKED:
				GetDlgItemText( hDlg, IDC_SEQ_FPS, str, 255 );
				m_pSeq->fps = (float)atof(str);
				EndDialog( hDlg, IDOK );
				break;
			}
			break;

		case IDCANCEL:
			switch ( HIWORD(wParam))
			{
			case BN_CLICKED:
				EndDialog( hDlg, IDCANCEL );
				break;
			}
			break;

		case IDC_SEQ_EVENTSAVE:
			switch ( HIWORD(wParam))
			{
			case BN_CLICKED:
				index = SendMessage( GetDlgItem(hDlg, IDC_SEQ_EVENTS), LB_GETCURSEL, 0, 0 );

				GetDlgItemText( hDlg, IDC_SEQ_EVENTFRAME, str, 255 );
				m_pEvents[index].frame = atoi(str);
				GetDlgItemText( hDlg, IDC_SEQ_EVENTOPT, opt, 64 );
				strcpy(m_pEvents[index].options, opt);
				EnableWindow( GetDlgItem(hDlg, IDC_SEQ_EVENTSAVE), FALSE );
				break;
			}
			break;

		case IDC_SEQ_EVENTFRAME:
		case IDC_SEQ_EVENTOPT:
			switch ( HIWORD(wParam))
			{
			case EN_CHANGE:
				EnableWindow( GetDlgItem(hDlg, IDC_SEQ_EVENTSAVE), TRUE );
				break;
			}
			break;

		case IDC_SEQ_EVENTS:
			switch ( HIWORD(wParam))
			{
			case LBN_SELCHANGE:
				index = SendMessage( GetDlgItem(hDlg, IDC_SEQ_EVENTS), LB_GETCURSEL, 0, 0 );
				
				sprintf( str, "%i", m_pEvents[index].frame );
				SetDlgItemText( hDlg, IDC_SEQ_EVENTFRAME, str );
				sprintf( str, "%i", m_pEvents[index].event );
				SetDlgItemText( hDlg, IDC_SEQ_EVENT, str );
				sprintf( str, "%s", m_pEvents[index].options );
				SetDlgItemText( hDlg, IDC_SEQ_EVENTOPT, str );
				EnableWindow( GetDlgItem(hDlg, IDC_SEQ_EVENTSAVE), FALSE );
				break;
			}
			break;
		}
		break;

	case WM_CLOSE:
		EndDialog( hDlg, ID_CLOSE );
		break;
	}
	return FALSE;
}

void CSequenceDlg::show( mstudioseqdesc_t *pSeq, mstudioevent_t *pEvents )
{
	this->m_pSeq = pSeq;
	this->m_pEvents = pEvents;
	DialogBox( m_hInstance, MAKEINTRESOURCE( IDD_SEQUENCE ), m_hOwner, (DLGPROC)DlgProc );	
}
