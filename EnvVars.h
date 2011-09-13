// EnvVars.h:
// 包含一些系统头文件
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENVVARS_H__3B7360BD_FC81_4867_8FF7_57CA936E3BCA__INCLUDED_)
#define AFX_ENVVARS_H__3B7360BD_FC81_4867_8FF7_57CA936E3BCA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning( disable : 4786 )

#include <windows.h>
#include <map>
#include "utils.h"

/* 应用程序数据 */
struct App
{
	StringArray CmdArguments;
	HINSTANCE hInstance;
	int nCmdShow;
	HWND * phMainWnd;
	String strExePath;
	String strExeFile;
	String strExeTitle;
	String strExeExt;
};
extern App __app;

void App_Init( void );
/* 初始化命令参数 */
void App_InitCmdArguments( void );


/* 复用宏 */
#define MemberName(CLS,M) CLS##_##M
// 获得指定类型指针
#define GetPtr( T, V ) ((T *)(V))
// Method Name
#define CLS_Member(CLS,NAME) MemberName(CLS,NAME)
// Custom Method
#define CLS_Method(TYPE,CLS,NAME) TYPE CLS_Member(CLS,NAME)

#define CLS_New(CLS) CLS_Method(CLS *,CLS,New)( void )
#define CLS_Delete(CLS) CLS_Method(void,CLS,Delete)( CLS * This )
#define CLS_New_Impl(CLS) CLS_New(CLS)\
{\
	CLS * This = new CLS;\
	return This;\
}
#define CLS_Delete_Impl(CLS) CLS_Delete(CLS)\
{\
	delete This;\
}

/* Dialog */
// wnd handle to data
#define DLG_WndMap(DLG) __wndMap##DLG
#define DLG_DeclMap(DLG) std::map< HWND,DLG * > DLG_WndMap(DLG)
// get data in dlg_proc()
#define DLG_This(DLG) ( DLG_WndMap(DLG)[hDlg] )
// assoc wnd handle
#define DLG_BindHWND(DLG) do { GetPtr( DLG, lParam )->hDlg = hDlg; DLG_WndMap(DLG)[hDlg] = GetPtr( DLG, lParam ); } while (0)
// DoModal方法
#define DLG_DoModal(DLG) CLS_Method(int,DLG,DoModal)( DLG * This, HWND hParentWnd )
#define DLG_DoModal_Impl(DLG,TEMPL_ID) DLG_DoModal(DLG)\
{ return DialogBoxParam( __app.hInstance, MAKEINTRESOURCE(TEMPL_ID), hParentWnd, CLS_Member( DLG, Proc ), (LPARAM)This ); }
// OnInitDialog
#define DLG_OnInitDialog(DLG) CLS_Method(void,DLG,OnInitDialog)( DLG * This )
// OnDestroy
#define DLG_OnDestroy(DLG) CLS_Method(void,DLG,OnDestroy)( DLG * This )

// 对话框过程
#define DLG_Proc(DLG) CLS_Method(INT_PTR CALLBACK,DLG,Proc)( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )

// switch msg
#define BEGIN_MSG() \
switch ( uMsg )\
{\
case WM_NULL:\
	{


#define END_MSG() \
	}\
	break;\
}

#define ON_MSG(msg) \
	}\
	break;\
case msg:\
	{
// switch command
#define BEGIN_CMD()\
ON_MSG(WM_COMMAND)\
	switch ( LOWORD(wParam) )\
	{\
	case 0xFFFFFFFF:\
		{

#define END_CMD() \
	}\
	break;\
}

#define ON_ID(id) \
	}\
	break;\
case id:\
	{

// cmd event code
#define BEGIN_EVENT(ID)\
ON_ID(ID)\
	switch ( HIWORD(wParam) )\
	{\
	case 0xFFFFFFFF:\
		{

#define END_EVENT() \
	}\
	break;\
}

#define ON_EVENT(evt) \
	}\
	break;\
case evt:\
	{

// notify code
#define BEGIN_NOTIFY()\
ON_MSG(WM_NOTIFY)\
	switch ( GetPtr(NMHDR,lParam)->idFrom )\
	{\
	case 0xFFFFFFFF:\
		{

#define END_NOTIFY() \
	}\
	break;\
}

#define BEGIN_CODE(ControlID)\
ON_ID(ControlID)\
	switch ( GetPtr(NMHDR,lParam)->code )\
	{\
	case 0xFFFFFFFF:\
		{

#define END_CODE() \
	}\
	break;\
}

#define ON_CODE(code)\
	}\
	break;\
case code:\
	{


#endif // !defined(AFX_ENVVARS_H__3B7360BD_FC81_4867_8FF7_57CA936E3BCA__INCLUDED_)
