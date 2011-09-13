// VariablesDlg.h: interface for the VariablesDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VARIABLESDLG_H__17FF7682_941F_4931_A16C_91AC4FD95B2E__INCLUDED_)
#define AFX_VARIABLESDLG_H__17FF7682_941F_4931_A16C_91AC4FD95B2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct VariablesDlg  
{
	HWND hDlg;
	bool bIsUser;
	bool bSelected;
	String strVarSelected;
};

extern DLG_DeclMap(VariablesDlg);

CLS_New(VariablesDlg);
CLS_Delete(VariablesDlg);

DLG_OnInitDialog(VariablesDlg);
DLG_Proc(VariablesDlg);
DLG_DoModal(VariablesDlg);



#endif // !defined(AFX_VARIABLESDLG_H__17FF7682_941F_4931_A16C_91AC4FD95B2E__INCLUDED_)
