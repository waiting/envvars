// VariablesDlg.cpp:
//
//////////////////////////////////////////////////////////////////////
#include "EnvVars.h"
#include "RegOperate3.h"
#include "resource.h"
#include "EnvVarsDlg.h"
#include "VariablesDlg.h"
#include <CommCtrl.h>


DLG_DeclMap(VariablesDlg);

CLS_New_Impl(VariablesDlg)
CLS_Delete_Impl(VariablesDlg)
DLG_DoModal_Impl( VariablesDlg, IDD_VARIABLES )

DLG_OnInitDialog(VariablesDlg)
{
	Window_Center( This->hDlg, GetParent(This->hDlg) );
	// 图标
	EnvVarsDlg * pDlg = DLG_WndMap(EnvVarsDlg)[*__app.phMainWnd];
	SendMessage( This->hDlg, WM_SETICON, ICON_SMALL, (LPARAM)pDlg->hIcon );
	// 标题
	String strTitle= This->bIsUser ? LoadStringRes(IDS_LANG_USERENVVARS) : LoadStringRes(IDS_LANG_SYSENVVARS);
	Window_SetText( This->hDlg, strTitle );
	// 初始化list view
	String strName, strValue;
	strName = LoadStringRes(IDS_LANG_NAME);
	strValue = LoadStringRes(IDS_LANG_VALUE);
	HWND hListCtrl = GetDlgItem( This->hDlg, IDC_LISTVIEW_VARS );
	SendMessage( hListCtrl, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)LVS_EX_FULLROWSELECT );
	int nFirstColWidth = 85;
	LVCOLUMN column;
	column.mask = LVCF_TEXT | LVCF_FMT;
	column.pszText = &strName[0];
	column.fmt = LVCFMT_LEFT;
	column.mask |= LVCF_WIDTH;
	column.cx = nFirstColWidth;
	SendMessage( hListCtrl, LVM_INSERTCOLUMN, 0, (LPARAM)&column );
	column.pszText = &strValue[0];
	RECT rcListCtrl;
	GetClientRect( hListCtrl, &rcListCtrl );
	int nListCtrlWidth = rcListCtrl.right - rcListCtrl.left;
	column.cx = nListCtrlWidth - nFirstColWidth;
	SendMessage( hListCtrl, LVM_INSERTCOLUMN, 1, (LPARAM)&column );

	// 枚举环境变量
	UINT uKeyResId = This->bIsUser ? IDS_USERENVKEY: IDS_SYSENVKEY;
	REG_VALUE_ARRAY arr;
	String strEnvKey;
	strEnvKey = LoadStringRes(uKeyResId);
	HKEY hKey = reg_open_key( strEnvKey.c_str() );
	reg_enum_value( hKey, &arr );
	reg_visit_val( arr, rv_ );
	TCHAR szValue[512] = {0};
	String strValueExpand;
	reg_read_string( hKey, rv_->value_name, szValue, sizeof(szValue) );
	strValueExpand = ExplainEnvVars(szValue).c_str();
	ListView_AddStrings( hListCtrl, 2, rv_->value_name, strValueExpand.c_str() );
	reg_end();
	reg_free_value_array(arr);
	reg_close_key(hKey);

}

DLG_OnDestroy(VariablesDlg)
{
	DLG_WndMap(VariablesDlg).erase(This->hDlg);
}

CLS_Method( void, VariablesDlg, OnOK )( VariablesDlg * This )
{
	HWND hListCtrl = GetDlgItem( This->hDlg, IDC_LISTVIEW_VARS );
	int iItem = -1;
	while ( ( iItem = ListView_GetNextItem( hListCtrl, iItem, LVIS_SELECTED ) ) != -1 )
	{
		TCHAR szVarName[256] = {0};
		ListView_GetStrings( hListCtrl, iItem, 1, szVarName, 256 );
		This->strVarSelected += String( TEXT("%") ) + szVarName + TEXT("%");
		This->bSelected = true;
	}
	EndDialog( This->hDlg, IDOK );
}

CLS_Method( void, VariablesDlg, OnRunModify )( VariablesDlg * This )
{
	HWND hListCtrl = GetDlgItem( This->hDlg, IDC_LISTVIEW_VARS );
	int iItem = -1;
	while ( ( iItem = ListView_GetNextItem( hListCtrl, iItem, LVIS_SELECTED ) ) != -1 )
	{
		TCHAR szVarName[256] = {0};
		ListView_GetStrings( hListCtrl, iItem, 1, szVarName, 256 );
		WinExec( StringToLocal( __app.strExeTitle + TEXT(" ") + szVarName ).c_str() , SW_NORMAL );
	}
}

CLS_Method( void, VariablesDlg, OnListViewVarsDbClk )( VariablesDlg * This, LPNMITEMACTIVATE lpnmitem )
{
	if ( lpnmitem->iItem == -1 )
	{
		return;
	}
	CLS_Member( VariablesDlg, OnOK )(This);
}

DLG_Proc(VariablesDlg)
{
	BEGIN_MSG()
		BEGIN_NOTIFY()
			BEGIN_CODE(IDC_LISTVIEW_VARS)
				ON_CODE(NM_DBLCLK)
					CLS_Member( VariablesDlg, OnListViewVarsDbClk )( DLG_This(VariablesDlg), GetPtr( NMITEMACTIVATE, lParam ) );
			END_CODE()
		END_NOTIFY()
		ON_MSG(WM_INITDIALOG)
			DLG_BindHWND(VariablesDlg);
			CLS_Member( VariablesDlg, OnInitDialog )( GetPtr( VariablesDlg, lParam ) );
		BEGIN_CMD()
			ON_ID(IDC_BTN_RUN_MODIFY)
				CLS_Member( VariablesDlg, OnRunModify )( DLG_This(VariablesDlg) );
			ON_ID(IDCANCEL)
				EndDialog( hDlg, IDCANCEL );
			ON_ID(IDOK)
				CLS_Member( VariablesDlg, OnOK )( DLG_This(VariablesDlg) );
		END_CMD()
		ON_MSG(WM_DESTROY)
			CLS_Member( VariablesDlg, OnDestroy )( DLG_This(VariablesDlg) );
	END_MSG()
	return 0;
}

