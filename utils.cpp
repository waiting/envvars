// utils.cpp: implementation of the utils class.
//
//////////////////////////////////////////////////////////////////////
#ifdef __GNUC__
#define WINVER 0x0501
#define _WIN32_IE 0x0501
#endif

#include <algorithm>
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <commctrl.h>
#include <olectl.h>
#pragma comment ( lib, "gdiplus" )
#include "utils.h"

// 环境相关 ---------------------------------------------------------------
bool IsExpandString( LPCTSTR lpsz )
{
	LPCTSTR p = lpsz;
	int i = 0, j;
	while ( p[i] )
	{
		if ( p[i] == '%' )
		{
			j = i + 1;
			while ( p[j] && p[j] != '%' ) j++;
			if ( p[j] != 0 )
			{
				i = j + 1;
				return true;
			}
		}
		else
		{
			i++;
		}
	}
	return false;
}

String ExplainEnvVars( LPCTSTR lpsz )
{
	//TCHAR sz[0x7FFFU] = {0};
	//ExpandEnvironmentStrings( lpsz, sz, 0x7FFFU );
	//return sz;
	String strRes;
	String str(lpsz);
	LPCTSTR p = lpsz;
	int i = 0, j;
	while ( p[i] )
	{
		if ( p[i] == '%' )
		{
			j = i + 1;
			while ( p[j] && p[j] != '%' ) j++;
			if ( p[j] == 0 )
			{
				strRes += str.substr( i, j - i );
				i = j;
			}
			else
			{
				String strEnvVar = str.substr( i + 1, j - i - 1 );
				strEnvVar = _tgetenv( strEnvVar.c_str() );
				strRes += strEnvVar;
				i = j + 1;
			}
		}
		else
		{
			strRes += p[i];
			i++;
		}
	}
	return strRes;//*/
}


// 字串相关 ---------------------------------------------------------------
UINT LocalCharsCount( LPCSTR lpszLocal )
{
	UINT uCount = 0;
	uCount = MultiByteToWideChar(
		CP_ACP,
		MB_PRECOMPOSED | MB_USEGLYPHCHARS,
		lpszLocal,
		lstrlenA(lpszLocal),
		NULL,
		0
	);
	return uCount;
}

UINT UnicodeMinLength( LPCWSTR lpszUnicode )
{
	UINT uLength = 0;
	uLength = WideCharToMultiByte(
		CP_ACP,
		WC_SEPCHARS | WC_COMPOSITECHECK,
		lpszUnicode,
		lstrlenW(lpszUnicode),
		NULL,
		0,
		NULL,
		NULL
	);
	return uLength;
}

LocalString UnicodeToLocal( UnicodeString const & strUnicode )
{
	LocalString str;
	UINT uLength = UnicodeMinLength( strUnicode.c_str() );
	str.resize(uLength);
	WideCharToMultiByte(
		CP_ACP,
		WC_SEPCHARS | WC_COMPOSITECHECK,
		strUnicode.c_str(),
		strUnicode.length(),
		&str[0],
		uLength,
		NULL,
		NULL
	);
	return str;
}

UnicodeString LocalToUnicode( LocalString const & strLocal )
{
	UnicodeString str;
	UINT uCount = LocalCharsCount( strLocal.c_str() );
	str.resize(uCount);
	MultiByteToWideChar(
		CP_ACP,
		MB_PRECOMPOSED | MB_USEGLYPHCHARS,
		strLocal.c_str(),
		strLocal.length(),
		&str[0],
		uCount
	);
	return str;
}

String LocalToString( LocalString const & strLocal )
{
#ifdef UNICODE
	return LocalToUnicode(strLocal);
#else
	return strLocal;
#endif
}

String UnicodeToString( UnicodeString const & strUnicode )
{
#ifdef UNICODE
	return strUnicode;
#else
	return UnicodeToLocal(strUnicode);
#endif
}

LocalString StringToLocal( String const & str )
{
#ifdef UNICODE
	return UnicodeToLocal(str);
#else
	return str;
#endif
}

UnicodeString StringToUnicode( String const & str )
{
#ifdef UNICODE
	return str;
#else
	return LocalToUnicode(str);
#endif
}

String UTF8ToString( AnsiString const & strUTF8 )
{
	UnicodeString strUnicode;
	int cch = MultiByteToWideChar(
		CP_UTF8,
		0,
		strUTF8.c_str(),
		strUTF8.length(),
		NULL,
		0
	);
	strUnicode.resize(cch);
	MultiByteToWideChar(
		CP_UTF8,
		0,
		strUTF8.c_str(),
		strUTF8.length(),
		&strUnicode[0],
		cch
	);
	return UnicodeToString(strUnicode);
}

AnsiString StringToUTF8( String const & str )
{
	UnicodeString strUnicode = StringToUnicode(str);
	AnsiString strUTF8;
	int nLength = WideCharToMultiByte(
		CP_UTF8,
		0,
		strUnicode.c_str(),
		strUnicode.length(),
		NULL,
		0,
		NULL,
		NULL
	);
	strUTF8.resize(nLength);
	WideCharToMultiByte(
		CP_UTF8,
		0,
		strUnicode.c_str(),
		strUnicode.length(),
		&strUTF8[0],
		nLength,
		NULL,
		NULL
	);
	return strUTF8;
}


int StrSplit( LPCTSTR lpsz, LPCTSTR lpszDelim, StringArray * pArr )
{
	TCHAR * pszDup;
	TCHAR * pszToken;
	int nCount;
	pszDup = _tcsdup(lpsz);
	pszToken = _tcstok( pszDup, lpszDelim );
	nCount = 0;
	while ( pszToken != NULL )
	{
		nCount++;
		if ( pArr != NULL ) pArr->push_back(pszToken);
		pszToken = _tcstok( NULL, lpszDelim );
	}
	free(pszDup);
	return nCount;
}

String StrJoin( LPCTSTR lpszDelim, StringArray const & arr )
{
	String strRes;
	int nCount = arr.size();
	int i;
	for ( i = 0; i < nCount; i++ )
	{
		if ( i != 0 )
		{
			strRes += lpszDelim;
		}
		strRes += arr[i];
	}
	return strRes;
}

String StrInsert( LPCTSTR lpsz, int iStart, int iEnd, LPCTSTR lpsz2 )
{
	String strRes;
	String str(lpsz);
	strRes += str.substr( 0, iStart );
	strRes += lpsz2;
	if ( iEnd < str.length() )
		strRes += str.substr( iEnd );
	return strRes;
}

String FormatExV( int cchMsg, LPCTSTR lpszFormat, va_list args )
{
	String str;
	str.resize(cchMsg);
	_vsntprintf( &str[0], cchMsg, lpszFormat, args );
	return str.c_str();
}

String FormatEx( int cchMsg, LPCTSTR lpszFormat, ... )
{
	va_list args;
	va_start( args, lpszFormat );
	return FormatExV( cchMsg, lpszFormat, args );
}

String Format( LPCTSTR lpszFormat, ... )
{
	va_list args;
	va_start( args, lpszFormat );
	return FormatExV( 1024, lpszFormat, args );
}

// 资源相关 ---------------------------------------------------------------
UnicodeString LoadStringResExW( HMODULE hModule, UINT uStringResID )
{
	UnicodeString strRes;
	WORD wBlockID = ( uStringResID >> 4 ) + 1;
	WORD wStrResIndex = uStringResID % 16;
	HRSRC hRSRC = FindResource( hModule, MAKEINTRESOURCE(wBlockID), RT_STRING );
	if ( hRSRC != NULL )
	{
		DWORD dwSize = SizeofResource( hModule, hRSRC );
		HGLOBAL hGlobal = LoadResource( hModule, hRSRC );
		LPBYTE pData = (LPBYTE)LockResource(hGlobal);
		DWORD dwOffset = 0;
		int iIndex = 0;
		while ( dwOffset + sizeof(WORD) <= dwSize )
		{
			WORD w = *(WORD *)( pData + dwOffset );
			dwOffset += sizeof(WORD);
			if ( w > 0 )
			{
				if ( iIndex == wStrResIndex )
				{
					UnicodeString strUnicode;
					strUnicode.resize(w);
					CopyMemory( &strUnicode[0], pData + dwOffset, w * sizeof(WCHAR) );
					strRes = strUnicode.c_str();
					break;
				}
				dwOffset += w * sizeof(WCHAR);
			}
			iIndex++;
		}
	}
	return strRes;
}

String LoadStringResEx( HMODULE hModule, UINT uStringResID )
{
	return UnicodeToString( LoadStringResExW( hModule, uStringResID ) );
}

String LoadStringRes( UINT uStringResID )
{
	return LoadStringResEx( GetModuleHandle(NULL), uStringResID );
}

// 系统相关 ---------------------------------------------------------------
String ModulePath( HMODULE hModule /*= NULL*/, String * pStrFileName /*= NULL */ )
{
	String strBuffer;
	strBuffer.resize(MAX_PATH);
	GetModuleFileName( hModule, &strBuffer[0], MAX_PATH );
	return FilePath( strBuffer, pStrFileName );
}

String FilePath( String const & strFull, String * pStrFileName /*= NULL */ )
{
	String strPath;
	String strBuffer = strFull.c_str();
	TCHAR * psz = _tcsrchr( &strBuffer[0], _T('\\') );
	TCHAR * pszFile;
	if ( psz != NULL )
	{
		*psz = 0;
		strPath = &strBuffer[0];
		if ( pStrFileName != NULL ) *pStrFileName = psz + 1;
		pszFile = psz + 1;
	}
	else
	{
		strPath = TEXT("");
		if ( pStrFileName != NULL ) *pStrFileName = &strBuffer[0];
		pszFile = &strBuffer[0];
	}
	return strPath;
}

String FileTitle( String const & strFileName, String * pStrExtName /*= NULL */ )
{
	String strFileNameTemp = strFileName;
	String strFileTitle;
	TCHAR * pszFile = &strFileNameTemp[0];
	TCHAR * psz;
	psz = _tcsrchr( pszFile, _T('.') );
	if ( psz != NULL )
	{
		*psz = 0;
		strFileTitle = pszFile;
		if ( pStrExtName != NULL ) *pStrExtName = psz + 1;
	}
	else
	{
		strFileTitle = pszFile;
		if ( pStrExtName != NULL ) *pStrExtName = TEXT("");
	}
	return strFileTitle;
}

/* 判断是否是一个目录 */
BOOL IsDirectory( LPCTSTR lpszPath )
{
	DWORD dwAttr = GetFileAttributes(lpszPath);
	if ( dwAttr != INVALID_FILE_ATTRIBUTES && dwAttr & FILE_ATTRIBUTE_DIRECTORY )
	{
		return TRUE;
	}
	return FALSE;
}


int GetCommandArguments( StringArray * pArr )
{
	pArr->clear();
	// 命令行参数处理
	LPWSTR lpszCmdLine = GetCommandLineW();
	int nNumArgs, i;
	LPWSTR * pArgsArr = CommandLineToArgvW( lpszCmdLine, &nNumArgs );
	for ( i = 0; i < nNumArgs; ++i )
	{
		pArr->push_back( UnicodeToString( pArgsArr[i] ) );
	}
	GlobalFree( (HGLOBAL)pArgsArr );
	return nNumArgs;
}

bool ShutdownPrivilege( bool bEnable )
{
	HANDLE hToken;
	bool bRet = OpenProcessToken( GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken ) != FALSE;
	if ( !bRet ) return FALSE;
	TOKEN_PRIVILEGES tp;
	tp.PrivilegeCount = 1;
	LookupPrivilegeValue( NULL, SE_SHUTDOWN_NAME, &tp.Privileges[0].Luid );
	tp.Privileges[0].Attributes = bEnable ? SE_PRIVILEGE_ENABLED : 0;
	AdjustTokenPrivileges( hToken, FALSE, &tp, 0, NULL, NULL );
	bRet = ( GetLastError() == ERROR_SUCCESS );
	CloseHandle(hToken);
	return bRet;
}


// 1970-01-01 00:00:00的ULARGE_INTEGER描述
static ULARGE_INTEGER __Time1970( void )
{
	SYSTEMTIME st1970 = {0};
	st1970.wYear = 1970;
	st1970.wMonth = 1;
	st1970.wDay = 1;
	st1970.wHour = 0;
	st1970.wMinute = 0;
	st1970.wSecond = 0;
	
	FILETIME ft1970;
	ULARGE_INTEGER time1970;
	SystemTimeToFileTime( &st1970, &ft1970 );
	CopyMemory( &time1970, &ft1970, sizeof(time1970) );
	return time1970;
}

ULONGLONG GetUTCTimeMS( void )
{
	SYSTEMTIME st;
	FILETIME ft;
	ULARGE_INTEGER time;
	GetSystemTime(&st);
	SystemTimeToFileTime( &st, &ft );
	CopyMemory( &time, &ft, sizeof(time) );
	
	ULARGE_INTEGER timeRes;
	timeRes.QuadPart = time.QuadPart - __Time1970().QuadPart;
	
	return timeRes.QuadPart / 10000;
}

UINT GetUTCTime( void )
{
	return (UINT)( GetUTCTimeMS() / 1000 );
}

void GetUTCSysTimeByUTCMS( ULONGLONG ullMilliseconds, LPSYSTEMTIME lpSysTime )
{
	ULONGLONG ullTime = ullMilliseconds * 10000;
	ULARGE_INTEGER time;
	time.QuadPart = ullTime + __Time1970().QuadPart;
	FILETIME ft;
	CopyMemory( &ft, &time, sizeof(ft) );
	FileTimeToSystemTime( &ft, lpSysTime );
}

void GetLocalSysTimeByUTCMS( ULONGLONG ullMilliseconds, LPSYSTEMTIME lpSysTime )
{
	ULONGLONG ullTime = ullMilliseconds * 10000;
	ULARGE_INTEGER time;
	time.QuadPart = ullTime + __Time1970().QuadPart;
	FILETIME ft,localft;
	CopyMemory( &ft, &time, sizeof(ft) );
	FileTimeToLocalFileTime( &ft, &localft );
	FileTimeToSystemTime( &localft, lpSysTime );
}

// UI相关 -----------------------------------------------------------------
UINT_PTR WindowTimer::_uIDAutoIncrement = 0;
WindowTimer::WindowTimer( void ) : _hWnd(NULL), _uID(0)
{
}

WindowTimer::~WindowTimer( void )
{
	Destroy();
}

void WindowTimer::Create( HWND hWnd, UINT uElapse )
{
	Destroy();
	_hWnd = hWnd;
	if ( _hWnd )
	{
		_uID = ++_uIDAutoIncrement;
		SetTimer( _hWnd, _uID, uElapse, NULL );
	}
}

void WindowTimer::Destroy( void )
{
	if ( _hWnd )
	{
		KillTimer( _hWnd, _uID );
		_hWnd = NULL;
	}
}

UINT WindowTimer::GetID( void ) const
{
	return _uID;
}

// 窗口相关 ---------------------------------------------------------------
void MsgBox( String const & strMsg )
{
	MessageBox( GetForegroundWindow(), strMsg.c_str(), _T("Information Box"), 0 );
}
/* 获取客户区矩形 */
RECT Window_GetClient( HWND hWnd )
{
	RECT rc;
	GetClientRect( hWnd, &rc );
	return rc;
}

RECT Window_GetRect( HWND hWnd )
{
	RECT rc;
	GetWindowRect( hWnd, &rc );
	HWND hParentWnd;
	hParentWnd = GetParent(hWnd);
	if ( IsWindow(hParentWnd) )
	{
		ScreenToClient( hParentWnd, (LPPOINT)&rc );
		ScreenToClient( hParentWnd, (LPPOINT)&rc + 1 );
	}
	return rc;
}

void Window_SetRect( HWND hWnd, LPCRECT lpRect )
{
	SetWindowPos( hWnd, NULL, lpRect->left, lpRect->top, lpRect->right - lpRect->left, lpRect->bottom - lpRect->top, SWP_NOZORDER );
}

void Window_Inflate( HWND hWnd, int dx, int dy )
{
	RECT rc = Window_GetRect(hWnd);
	InflateRect( &rc, dx, dy );
	Window_SetRect( hWnd, &rc );
}

void Window_Center( HWND hWnd, HWND hRelativeWnd, bool bInRelativeWnd )
{
	INT cx, cy, offsetLeft, offsetTop;
	if ( hRelativeWnd && IsWindow(hRelativeWnd) )
	{
		if ( bInRelativeWnd )
		{
			RECT rcWnd;
			GetClientRect( hRelativeWnd, &rcWnd );
			cx = rcWnd.right - rcWnd.left;
			cy = rcWnd.bottom - rcWnd.top;
			offsetLeft = 0;//rcWnd.left;
			offsetTop = 0;//rcWnd.top;
		}
		else
		{
			RECT rcWnd;
			GetWindowRect( hRelativeWnd, &rcWnd );
			cx = rcWnd.right - rcWnd.left;
			cy = rcWnd.bottom - rcWnd.top;
			offsetLeft = rcWnd.left;
			offsetTop = rcWnd.top;
		}
	}
	else // use screen coord
	{
		cx = GetSystemMetrics(SM_CXSCREEN);
		cy = GetSystemMetrics(SM_CYSCREEN);
		offsetLeft = 0;
		offsetTop = 0;
	}
	RECT rc;
	GetWindowRect( hWnd, &rc );
	LONG nWidth = rc.right - rc.left;
	LONG nHeight = rc.bottom - rc.top;
	INT left, top;
	left = offsetLeft + ( cx - nWidth ) / 2;
	top = offsetTop + ( cy - nHeight ) / 2;
	SetWindowPos( hWnd, NULL, left, top, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
}

String Window_GetText( HWND hWnd )
{
	String strRes;
	int nLen = GetWindowTextLength(hWnd);
	strRes.resize(nLen);
	GetWindowText( hWnd, &strRes[0], nLen + 1 );
	return strRes.c_str();
}

void Window_SetText( HWND hWnd, String const & str )
{
	SetWindowText( hWnd, str.c_str() );
}


// ListView操作
void ListView_AddStrings( HWND hListView, int nArgCount, ... )
{
	int nItemsCount = ListView_GetItemCount(hListView);
	int nColCount = Header_GetItemCount( ListView_GetHeader(hListView) );
	LVITEM item;
	item.mask = LVIF_TEXT;
	item.iItem = nItemsCount;
	item.iSubItem = 0;
	item.pszText = (LPTSTR)NULL;
	item.state = 0;
	item.stateMask = 0;
	item.iImage = 0;
	item.lParam = 0;
	ListView_InsertItem( hListView, &item );
	va_list vaArgList;
	va_start( vaArgList, nArgCount );
	for ( int i = 0; i < nArgCount && i < nColCount; i++ )
	{
		LPTSTR lpsz = va_arg( vaArgList, LPTSTR );
		ListView_SetItemText( hListView, nItemsCount, i, lpsz );
	}
	va_end(vaArgList);
}

void ListView_InsertStrings( HWND hListView, int iItemIndex, int nArgCount, ... )
{
	int nColCount = Header_GetItemCount( ListView_GetHeader(hListView) );
	LVITEM item;
	item.mask = LVIF_TEXT;
	item.iItem = iItemIndex;
	item.iSubItem = 0;
	item.pszText = (LPTSTR)NULL;
	item.state = 0;
	item.stateMask = 0;
	item.iImage = 0;
	item.lParam = 0;
	ListView_InsertItem( hListView, &item );

	va_list vaArglist;
	va_start( vaArglist, nArgCount );
	for ( int i = 0; i < nArgCount && i < nColCount; i++ )
	{
		LPTSTR lpsz = va_arg( vaArglist, LPTSTR );
		ListView_SetItemText( hListView, iItemIndex, i, lpsz );
	}
	va_end(vaArglist);
}

void ListView_SetStrings( HWND hListView, int iItemIndex, int nArgCount, ... )
{
	int nColCount = Header_GetItemCount( ListView_GetHeader(hListView) );
	va_list vaArgList;
	va_start( vaArgList, nArgCount );
	for ( int i = 0; i < nArgCount && i < nColCount; i++ )
	{
		LPTSTR lpsz = va_arg( vaArgList, LPTSTR );
		ListView_SetItemText( hListView, iItemIndex, i, lpsz );
	}
	va_end(vaArgList);

}

void ListView_GetStrings( HWND hListView, int iItemIndex, int nArgPairCount, ... )
{
	int nColCount = Header_GetItemCount( ListView_GetHeader(hListView) );
	va_list vaArgList;
	va_start( vaArgList, nArgPairCount );
	for ( int i = 0; i < nArgPairCount && i < nColCount; i++ )
	{
		LPTSTR lpsz = va_arg( vaArgList, LPTSTR );
		int cch = va_arg( vaArgList, int );
		if ( lpsz != NULL )
		{
			ListView_GetItemText( hListView, iItemIndex, i, lpsz, cch );
		}
	}
	va_end(vaArgList);
}

