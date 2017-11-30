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

	EnvVarsDlg * pMainDlg = EnvVarsDlg_New();
	__app.phMainWnd = &pMainDlg->hDlg;
	int nResult = EnvVarsDlg_DoModal( pMainDlg, HWND_DESKTOP );

	EnvVarsDlg_Delete(pMainDlg);
	return 0;
}

// Application Data ------------------------------------------------------
App __app;
void App_Init( void )
{
	InitCommonControls();
	GetCommandArguments( &__app.CmdArguments );
	// get path of executable
	__app.strExePath = ModulePath( __app.hInstance, &__app.strExeFile );
	__app.strExeTitle = FileTitle( __app.strExeFile, &__app.strExeExt );
}
