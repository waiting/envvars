// EnvVarsDlg.h:
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENVVARSDLG_H__9B4FCC14_53DE_4FD1_9035_BE0F914E6CAC__INCLUDED_)
#define AFX_ENVVARSDLG_H__9B4FCC14_53DE_4FD1_9035_BE0F914E6CAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define IDM_SYS_ADD_VAL 2001
#define IDM_SYS_MOD_VAL 2002
#define IDM_SYS_DEL_VAL 2003
#define IDM_USR_ADD_VAL 2004
#define IDM_USR_MOD_VAL 2005
#define IDM_USR_DEL_VAL 2006

struct EnvVarsDlg
{
	HWND hDlg;
	HICON hIcon;
	StringArray userVars;
	StringArray sysVars;
	String strVarName;
	String strUserEnvKey;
	String strSysEnvKey;
	DWORD dwRegTypeUserVar;
	DWORD dwRegTypeSysVar;
	bool bUserVarHasModified;
	bool bSysVarHasModified;
	UINT uDragListMsg;
	int iDragSource;
};

extern DLG_DeclMap(EnvVarsDlg);

EnvVarsDlg * EnvVarsDlg_New( void );
void EnvVarsDlg_Delete( EnvVarsDlg * This );

void EnvVarsDlg_OnInitDialog( EnvVarsDlg * This );
INT_PTR CALLBACK EnvVarsDlg_Proc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam );
int EnvVarsDlg_DoModal( EnvVarsDlg * This, HWND hParentWnd );

#endif // !defined(AFX_ENVVARSDLG_H__9B4FCC14_53DE_4FD1_9035_BE0F914E6CAC__INCLUDED_)
