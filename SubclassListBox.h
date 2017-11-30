// SubclassListBox.h: interface for the SubclassListBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SUBCLASSLISTBOX_H__6B0A324E_4D57_4A56_83E6_A7290EA67C63__INCLUDED_)
#define AFX_SUBCLASSLISTBOX_H__6B0A324E_4D57_4A56_83E6_A7290EA67C63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


void SubclassWindow( HWND hWnd, bool bEnabled, WNDPROC wndproc, WNDPROC * pWndprocOld );
void SubclassSysVarsListBox( HWND hListBox, bool bEnabled );
void SubclassUsrVarsListBox( HWND hListBox, bool bEnabled );

#endif // !defined(AFX_SUBCLASSLISTBOX_H__6B0A324E_4D57_4A56_83E6_A7290EA67C63__INCLUDED_)
