// ModifyDlg.h:
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODIFYDLG_H__893A3CD6_A465_4091_B106_3EA4FDB8FF86__INCLUDED_)
#define AFX_MODIFYDLG_H__893A3CD6_A465_4091_B106_3EA4FDB8FF86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct ModifyDlg  
{
	HWND hDlg;
	String strValue;
	bool bIsUser;
	bool bIsAdd;

};

extern DLG_DeclMap(ModifyDlg);

CLS_New(ModifyDlg);
CLS_Delete(ModifyDlg);

DLG_OnInitDialog(ModifyDlg);
DLG_Proc(ModifyDlg);
DLG_DoModal(ModifyDlg);

#endif // !defined(AFX_MODIFYDLG_H__893A3CD6_A465_4091_B106_3EA4FDB8FF86__INCLUDED_)
