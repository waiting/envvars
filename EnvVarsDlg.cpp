// EnvVarsDlg.cpp:
//
//////////////////////////////////////////////////////////////////////
#include "EnvVars.h"
#include "EnvVarsDlg.h"
#include "RegOperate3.h"
#include "resource.h"
#include "ModifyDlg.h"
#include "SubclassListBox.h"

DLG_DeclMap(EnvVarsDlg);

CLS_New_Impl(EnvVarsDlg)
CLS_Delete_Impl(EnvVarsDlg)
DLG_DoModal_Impl( EnvVarsDlg, IDD_ENVVARS_DIALOG )

// 载入环境变量
CLS_Method( void, EnvVarsDlg, LoadEnvVars )( EnvVarsDlg * This )
{
	UINT const BUF_SIZE = 32767;
	TCHAR sz[BUF_SIZE];
	DWORD dwSize;
	String strTitle;
	This->strSysEnvKey = LoadStringRes(IDS_SYSENVKEY);
	This->strUserEnvKey = LoadStringRes(IDS_USERENVKEY);
	This->strVarName = LoadStringRes(IDS_VARNAME);
	if ( __app.CmdArguments.size() > 1 )
	{
		This->strVarName = __app.CmdArguments[1];
	}
	strTitle = Window_GetText(This->hDlg);
	Window_SetText( This->hDlg, ( This->strVarName + strTitle ) );

	HKEY hSysEnvKey = reg_open_key( This->strSysEnvKey.c_str(), FALSE );
	dwSize = BUF_SIZE;
	ZeroMemory( sz, dwSize );
	reg_read_ex( hSysEnvKey, This->strVarName.c_str(), (LPBYTE)sz, &This->dwRegTypeSysVar, &dwSize );
	This->sysVars.clear();
	StrSplit( sz, ";", &This->sysVars );
	reg_close_key(hSysEnvKey);
	
	HKEY hUserEnvKey = reg_open_key( This->strUserEnvKey.c_str(), FALSE );
	dwSize = BUF_SIZE;
	ZeroMemory( sz, dwSize );
	reg_read_ex( hUserEnvKey, This->strVarName.c_str(), (LPBYTE)sz, &This->dwRegTypeUserVar, &dwSize );
	This->userVars.clear();
	StrSplit( sz, ";", &This->userVars );
	reg_close_key(hUserEnvKey);
}

// 提交变量
CLS_Method( void, EnvVarsDlg, CommitEnvVars )( EnvVarsDlg * This, String const & strEnvKey, String const & strVarName, String const & strValue )
{
	if ( strValue.empty() )
	{
		reg_delete( strEnvKey.c_str(), strVarName.c_str() );
	}
	else
	{
		bool bIsExpand = IsExpandString( strValue.c_str() );
		HKEY hKey = reg_open_key( strEnvKey.c_str(), FALSE );
		reg_write_ex( hKey, strVarName.c_str(), bIsExpand ? REG_EXPAND_SZ : REG_SZ, (BYTE const *)strValue.c_str(), strValue.length() * sizeof(TCHAR) );
		reg_close_key(hKey);
	}
}

// 更新变量列表
CLS_Method( void, EnvVarsDlg, UpdateVarsList )( EnvVarsDlg * This, UINT uListBoxID, StringArray & arrVars )
{
	HWND hListBox = GetDlgItem( This->hDlg, uListBoxID );
	int i;
	SendMessage( hListBox, LB_RESETCONTENT, 0, 0 );
	for ( i = 0; i < arrVars.size(); i++ )
	{
		SendMessage( hListBox, LB_ADDSTRING, 0, (LPARAM)ExplainEnvVars( arrVars[i].c_str() ).c_str() );
	}
}

// 添加值
CLS_Method( void, EnvVarsDlg, OnAddVal )( EnvVarsDlg * This, UINT uListBoxID, bool bIsUser, StringArray & arrVars, String const & strEnvKey, String const & strVarName )
{
	ModifyDlg * pModDlg = CLS_Member( ModifyDlg, New )();
	pModDlg->bIsUser = bIsUser;
	pModDlg->bIsAdd = true;
	if ( CLS_Member( ModifyDlg, DoModal )( pModDlg, This->hDlg ) == IDOK )
	{
		if ( !pModDlg->strValue.empty() )
		{
			arrVars.push_back( pModDlg->strValue.c_str() );
			int iSel = arrVars.size() - 1;
			CLS_Member( EnvVarsDlg, UpdateVarsList )( This, uListBoxID, arrVars );
			HWND hListBox = GetDlgItem( This->hDlg, uListBoxID );
			SendMessage( hListBox, LB_SETCURSEL, iSel, 0 );
			CLS_Member( EnvVarsDlg, CommitEnvVars )( This, strEnvKey, strVarName, StrJoin( ";", arrVars ) );
			This->bHasModified = true;
		}
	}
	CLS_Member( ModifyDlg, Delete )(pModDlg);
}

// 修改值
CLS_Method( void, EnvVarsDlg, OnModVal )( EnvVarsDlg * This, UINT uListBoxID, bool bIsUser, StringArray & arrVars, String const & strEnvKey, String const & strVarName )
{
	HWND hListBox = GetDlgItem( This->hDlg, uListBoxID );
	int iSel = SendMessage( hListBox, LB_GETCURSEL, 0, 0 );
	if ( iSel == -1 ) return;
	ModifyDlg * pModDlg = CLS_Member( ModifyDlg, New )();
	pModDlg->bIsUser = bIsUser;
	pModDlg->bIsAdd = false;
	pModDlg->strValue = arrVars[iSel].c_str();
	if ( CLS_Member( ModifyDlg, DoModal )( pModDlg, This->hDlg ) == IDOK )
	{
		arrVars[iSel] = pModDlg->strValue.c_str();
		bool bIsEmpty = arrVars[iSel].empty();
		if ( bIsEmpty ) arrVars.erase( arrVars.begin() + iSel );
		CLS_Member( EnvVarsDlg, UpdateVarsList )( This, uListBoxID, arrVars );
		SendMessage( hListBox, LB_SETCURSEL, bIsEmpty ? iSel - 1 : iSel, 0 );
		CLS_Member( EnvVarsDlg, CommitEnvVars )( This, strEnvKey, strVarName, StrJoin( ";", arrVars ) );
		This->bHasModified = true;
	}
	CLS_Member( ModifyDlg, Delete )(pModDlg);
}

// 对话框初始化消息
DLG_OnInitDialog(EnvVarsDlg)
{
	This->bHasModified = false;
	// 图标设置
	This->hIcon = LoadIcon( __app.hInstance, MAKEINTRESOURCE(IDR_MAINFRAME) );
	SendMessage( This->hDlg, WM_SETICON, ICON_BIG, (LPARAM)This->hIcon );
	SendMessage( This->hDlg, WM_SETICON, ICON_SMALL, (LPARAM)This->hIcon );

	// 位置居中
	Window_Center( This->hDlg, HWND_DESKTOP );

	// 载入环境变量
	CLS_Member( EnvVarsDlg, LoadEnvVars )(This);

	// 更新变量列表
	CLS_Member( EnvVarsDlg, UpdateVarsList)( This, IDC_LIST_USER, This->userVars );
	CLS_Member( EnvVarsDlg, UpdateVarsList)( This, IDC_LIST_SYS, This->sysVars );

	SubclassSysVarsListBox( GetDlgItem( This->hDlg, IDC_LIST_SYS ), true );
	SubclassUsrVarsListBox( GetDlgItem( This->hDlg, IDC_LIST_USER ), true );
}

DLG_OnDestroy(EnvVarsDlg)
{
	DLG_WndMap(EnvVarsDlg).erase(This->hDlg);
}

CLS_Method( void, EnvVarsDlg, OnOK )( EnvVarsDlg * This )
{
	if ( This->bHasModified )
	{
		LRESULT lResult;
		lResult = ::SendMessage( HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)"Environment" );
	}
	EndDialog( This->hDlg, IDOK );
}


DLG_Proc(EnvVarsDlg)
{
	BEGIN_MSG()
		ON_MSG(WM_INITDIALOG)
			DLG_BindHWND(EnvVarsDlg);
			CLS_Member( EnvVarsDlg, OnInitDialog )( GetPtr( EnvVarsDlg, lParam ) );
		BEGIN_CMD()
			BEGIN_EVENT(IDC_LIST_USER)
				ON_EVENT(LBN_DBLCLK)
					EnvVarsDlg * This = DLG_This(EnvVarsDlg);
					CLS_Member( EnvVarsDlg, OnModVal )( This, IDC_LIST_USER, true, This->userVars, This->strUserEnvKey, This->strVarName );
			END_EVENT()
			BEGIN_EVENT(IDC_LIST_SYS)
				ON_EVENT(LBN_DBLCLK)
					EnvVarsDlg * This = DLG_This(EnvVarsDlg);
					CLS_Member( EnvVarsDlg, OnModVal )( This, IDC_LIST_SYS, false, This->sysVars, This->strSysEnvKey, This->strVarName );
			END_EVENT()
			ON_ID(IDCANCEL)
				EndDialog( hDlg, IDCANCEL );
			ON_ID(IDOK)
				CLS_Member( EnvVarsDlg, OnOK )( DLG_This(EnvVarsDlg) );
			ON_ID(IDM_USR_ADD_VAL)
				EnvVarsDlg * This = DLG_This(EnvVarsDlg);
				CLS_Member( EnvVarsDlg, OnAddVal )( This, IDC_LIST_USER, true, This->userVars, This->strUserEnvKey, This->strVarName );
			ON_ID(IDM_USR_MOD_VAL)
				EnvVarsDlg * This = DLG_This(EnvVarsDlg);
				CLS_Member( EnvVarsDlg, OnModVal )( This, IDC_LIST_USER, true, This->userVars, This->strUserEnvKey, This->strVarName );
			ON_ID(IDM_SYS_ADD_VAL)
				EnvVarsDlg * This = DLG_This(EnvVarsDlg);
				CLS_Member( EnvVarsDlg, OnAddVal )( This, IDC_LIST_SYS, false, This->sysVars, This->strSysEnvKey, This->strVarName );
			ON_ID(IDM_SYS_MOD_VAL)
				EnvVarsDlg * This = DLG_This(EnvVarsDlg);
				CLS_Member( EnvVarsDlg, OnModVal )( This, IDC_LIST_SYS, false, This->sysVars, This->strSysEnvKey, This->strVarName );
		END_CMD()
		ON_MSG(WM_DESTROY)
			CLS_Member( EnvVarsDlg, OnDestroy )( DLG_This(EnvVarsDlg) );
	END_MSG()
	return 0;
}
