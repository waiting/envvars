// EnvVars.cpp:
// WinMain()和Application实现
//////////////////////////////////////////////////////////////////////

#include "EnvVars.h"
#include "EnvVarsDlg.h"
#include <string.h>
#include <tchar.h>

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	__app.hInstance = hInstance;
	__app.nCmdShow = nCmdShow;
	App_Init();

	EnvVarsDlg * pMainDlg = CLS_Member( EnvVarsDlg, New )();
	__app.phMainWnd = &pMainDlg->hDlg;
	int nResult = CLS_Member( EnvVarsDlg, DoModal )( pMainDlg, HWND_DESKTOP );

	CLS_Member( EnvVarsDlg, Delete )(pMainDlg);
	return 0;
}

// Application Data ------------------------------------------------------
App __app;
void App_Init( void )
{
	App_InitCmdArguments();
	// get path of executable
	TCHAR szBuff[_MAX_PATH];
	GetModuleFileName( __app.hInstance, szBuff, _MAX_PATH );
	TCHAR * psz = _tcsrchr( szBuff, '\\' );
	TCHAR * pszFile;
	if ( psz != NULL )
	{
		*psz = 0;
		__app.strExePath = szBuff;
		__app.strExeFile = psz + 1;
		pszFile = psz + 1;
	}
	else
	{
		__app.strExePath = TEXT("");
		__app.strExeFile = szBuff;
		pszFile = szBuff;
	}
	psz = _tcsrchr( pszFile, '.' );
	if ( psz != NULL )
	{
		*psz = 0;
		__app.strExeTitle = pszFile;
		__app.strExeExt = psz + 1;
	} 
	else
	{
		__app.strExeTitle = pszFile;
		__app.strExeExt = TEXT("");
	}
}

void App_InitCmdArguments( void )
{
	// 命令行参数处理
	LPWSTR lpszCmdLine = GetCommandLineW();
	int nNumArgs;
	LPWSTR * pArgsArr = CommandLineToArgvW( lpszCmdLine, &nNumArgs );
	for ( int i = 0; i < nNumArgs; ++i )
	{
		__app.CmdArguments.push_back( UnicodeToString( pArgsArr[i] ) );
	}
	GlobalFree( (HGLOBAL)pArgsArr );
}
