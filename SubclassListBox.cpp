// SubclassListBox.cpp: implementation of the SubclassListBox class.
//
//////////////////////////////////////////////////////////////////////
#include "EnvVars.h"
#include "SubclassListBox.h"
#include "EnvVarsDlg.h"
#include "resource.h"

WNDPROC __wndprocSysVarsList = NULL;
WNDPROC __wndprocUsrVarsList = NULL;

LRESULT CALLBACK SysVarsListBoxWndProc( HWND hListBox, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( uMsg )
	{
	case WM_CONTEXTMENU:
		{
			HMENU hMenu;
			int xPos, yPos;
			xPos = LOWORD(lParam);
			yPos = HIWORD(lParam);
			POINT pt;
			pt.x = xPos;
			pt.y = yPos;
			ScreenToClient( hListBox, &pt );
			LRESULT l = SendMessage( hListBox, LB_ITEMFROMPOINT, 0, MAKELPARAM( pt.x, pt.y ) );
			int iSel = LOWORD(l), fOutSide = HIWORD(l);
			iSel = fOutSide ? -1 : iSel;
			SendMessage( hListBox, LB_SETCURSEL, iSel, 0 );

			hMenu = CreatePopupMenu();
			AppendMenu( hMenu, MF_STRING, IDM_SYS_ADD_VAL, LoadStringRes(IDS_ADDMENU).c_str() );
			if ( !fOutSide ) AppendMenu( hMenu, MF_STRING, IDM_SYS_MOD_VAL, LoadStringRes(IDS_MODMENU).c_str() );
			TrackPopupMenu( hMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON, xPos, yPos, 0, GetParent(hListBox), NULL );
			DestroyMenu(hMenu);
		}
		break;
	}
	return CallWindowProc( __wndprocSysVarsList, hListBox, uMsg, wParam, lParam );
}

LRESULT CALLBACK UsrVarsListBoxWndProc( HWND hListBox, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( uMsg )
	{
	case WM_CONTEXTMENU:
		{
			HMENU hMenu;
			int xPos, yPos;
			xPos = LOWORD(lParam);
			yPos = HIWORD(lParam);
			POINT pt;
			pt.x = xPos;
			pt.y = yPos;
			ScreenToClient( hListBox, &pt );
			LRESULT l = SendMessage( hListBox, LB_ITEMFROMPOINT, 0, MAKELPARAM( pt.x, pt.y ) );
			int iSel = LOWORD(l), fOutSide = HIWORD(l);
			iSel = fOutSide ? -1 : iSel;
			SendMessage( hListBox, LB_SETCURSEL, iSel, 0 );

			hMenu = CreatePopupMenu();
			AppendMenu( hMenu, MF_STRING, IDM_USR_ADD_VAL, LoadStringRes(IDS_ADDMENU).c_str() );
			if ( !fOutSide ) AppendMenu( hMenu, MF_STRING, IDM_USR_MOD_VAL, LoadStringRes(IDS_MODMENU).c_str() );
			TrackPopupMenu( hMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON, xPos, yPos, 0, GetParent(hListBox), NULL );
			DestroyMenu(hMenu);
		}
		break;
	}
	return CallWindowProc( __wndprocUsrVarsList, hListBox, uMsg, wParam, lParam );
}

void SubclassWindow( HWND hWnd, bool bEnabled, WNDPROC wndproc, WNDPROC * pWndprocOld )
{
	if ( bEnabled )
	{
		if ( *pWndprocOld == NULL )
		{
			*pWndprocOld = (WNDPROC)GetWindowLong( hWnd, GWL_WNDPROC );
			SetWindowLong( hWnd, GWL_WNDPROC, (LONG)wndproc );
		}
	}
	else
	{
		if ( *pWndprocOld != NULL )
		{
			SetWindowLong( hWnd, GWL_WNDPROC, (LONG)*pWndprocOld );
			*pWndprocOld = NULL;
		}
	}
}

void SubclassSysVarsListBox( HWND hListBox, bool bEnabled )
{
	SubclassWindow( hListBox, bEnabled, SysVarsListBoxWndProc, &__wndprocSysVarsList );
}

void SubclassUsrVarsListBox( HWND hListBox, bool bEnabled )
{
	SubclassWindow( hListBox, bEnabled, UsrVarsListBoxWndProc, &__wndprocUsrVarsList );
}

