// ModifyDlg.cpp:
//
//////////////////////////////////////////////////////////////////////
#include "EnvVars.h"
#include "RegOperate3.h"
#include "resource.h"
#include "EnvVarsDlg.h"
#include "VariablesDlg.h"
#include "ModifyDlg.h"


DLG_DeclMap(ModifyDlg);

CLS_New_Impl(ModifyDlg)
CLS_Delete_Impl(ModifyDlg)
DLG_DoModal_Impl( ModifyDlg, IDD_MODIFY )

DLG_OnInitDialog(ModifyDlg)
{
	Window_Center( This->hDlg, GetParent(This->hDlg) );

	String strTitle;
	strTitle += This->bIsAdd ? LoadStringRes(IDS_LANG_ADD) : LoadStringRes(IDS_LANG_MODIFY);
	strTitle += This->bIsUser ? LoadStringRes(IDS_LANG_USERENVVARS) : LoadStringRes(IDS_LANG_SYSENVVARS);
	Window_SetText( This->hDlg, strTitle );

	Window_SetText( GetDlgItem( This->hDlg, IDC_EDT_VAL ), This->strValue );

	EnvVarsDlg * pDlg = DLG_WndMap(EnvVarsDlg)[*__app.phMainWnd];
	SendMessage( This->hDlg, WM_SETICON, ICON_SMALL, (LPARAM)pDlg->hIcon );
}

DLG_OnDestroy(ModifyDlg)
{
	DLG_WndMap(ModifyDlg).erase(This->hDlg);
}

CLS_Method( void, ModifyDlg, OnOK )( ModifyDlg * This )
{
	HWND hEdit = GetDlgItem( This->hDlg, IDC_EDT_VAL );
	This->strValue = Window_GetText(hEdit);
	EndDialog( This->hDlg, IDOK );
}

CLS_Method( void, ModifyDlg, OnBtnVariable )( ModifyDlg * This )
{
	VariablesDlg * pDlg = CLS_Member( VariablesDlg, New )();
	pDlg->bIsUser = This->bIsUser;
	pDlg->bSelected = false;

	if ( IDOK == CLS_Member( VariablesDlg, DoModal )( pDlg, This->hDlg ) )
	{
		HWND hEdit = GetDlgItem( This->hDlg, IDC_EDT_VAL );
		LONG iStart, iEnd;
		SendMessage( hEdit, EM_GETSEL, (WPARAM)&iStart, (LPARAM)&iEnd );
		if ( pDlg->bSelected )
		{
			This->strValue = Window_GetText(hEdit);
			This->strValue = StrInsert( This->strValue.c_str(), iStart, iEnd, pDlg->strVarSelected.c_str() );
			Window_SetText( hEdit, This->strValue );
		}
	}

	CLS_Member( VariablesDlg, Delete )(pDlg);
}

DLG_Proc(ModifyDlg)
{
	BEGIN_MSG()
		ON_MSG(WM_INITDIALOG)
			DLG_BindHWND(ModifyDlg);
			CLS_Member( ModifyDlg, OnInitDialog )( GetPtr( ModifyDlg, lParam ) );
		BEGIN_CMD()
			ON_ID(IDC_BTN_VARIABLE)
				ModifyDlg * This = DLG_This(ModifyDlg);
				CLS_Member( ModifyDlg, OnBtnVariable )(This);
			ON_ID(IDCANCEL)
				EndDialog( hDlg, IDCANCEL );
			ON_ID(IDOK)
				CLS_Member( ModifyDlg, OnOK )( DLG_This(ModifyDlg) );
		END_CMD()
		ON_MSG(WM_DESTROY)
			CLS_Member( ModifyDlg, OnDestroy )( DLG_This(ModifyDlg) );
		ON_MSG(WM_CTLCOLORSTATIC)
			HDC hDC = (HDC)wParam;
			SetTextColor( hDC, 0xFF0000 );
			SetBkMode( hDC, TRANSPARENT );
			return (INT_PTR)GetStockObject(NULL_BRUSH);
			//return SendMessage( (HWND)lParam, 你自己定义一个消息, wParam, lParam );
	END_MSG()
	return 0;
}

