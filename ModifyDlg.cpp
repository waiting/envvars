// ModifyDlg.cpp:
//
//////////////////////////////////////////////////////////////////////
#include "EnvVars.h"
#include "RegOperate3.h"
#include "resource.h"
#include "ModifyDlg.h"
#include "utils.h"

DLG_DeclMap(ModifyDlg);

CLS_New_Impl(ModifyDlg)
CLS_Delete_Impl(ModifyDlg)
DLG_DoModal_Impl( ModifyDlg, IDD_MODIFY )

DLG_OnInitDialog(ModifyDlg)
{
	CenterWindow( This->hDlg, GetParent(This->hDlg) );

	String strTitle;
	strTitle += This->bIsAdd ? "添加" : "修改";
	strTitle += This->bIsUser ? "用户环境变量" : "系统环境变量";
	SetWindowText( This->hDlg, strTitle.c_str() );

	SetWindowText( GetDlgItem( This->hDlg, IDC_EDT_VAL ), This->strValue.c_str() );
}

DLG_OnDestroy(ModifyDlg)
{
	DLG_WndMap(ModifyDlg).erase(This->hDlg);
}

CLS_Method( void, ModifyDlg, OnOK )( ModifyDlg * This )
{
	HWND hEdit = GetDlgItem( This->hDlg, IDC_EDT_VAL );
	String strEdit;
	int nLen = GetWindowTextLength(hEdit);
	strEdit.resize(nLen);
	GetWindowText( hEdit, &strEdit[0], nLen + 1 );
	This->strValue = strEdit.c_str();
	EndDialog( This->hDlg, IDOK );
}

DLG_Proc(ModifyDlg)
{
	BEGIN_MSG()
		ON_MSG(WM_INITDIALOG)
			DLG_BindHWND(ModifyDlg);
			CLS_Member( ModifyDlg, OnInitDialog )( GetPtr( ModifyDlg, lParam ) );
		ON_MSG(WM_COMMAND)
			BEGIN_CMD()
				ON_ID(IDC_BTN_VARIABLE)
					ModifyDlg * This = DLG_WndMap(ModifyDlg)[hDlg];
					MessageBox( hDlg, "", "", 0 );
				ON_ID(IDCANCEL)
					EndDialog( hDlg, IDCANCEL );
				ON_ID(IDOK)
					CLS_Member( ModifyDlg, OnOK )( DLG_WndMap(ModifyDlg)[hDlg] );
			END_CMD()
		ON_MSG(WM_DESTROY)
			CLS_Member( ModifyDlg, OnDestroy )( DLG_WndMap(ModifyDlg)[hDlg] );
	END_MSG()
	return 0;
}

