// utils.cpp: implementation of the utils class.
//
//////////////////////////////////////////////////////////////////////
#pragma warning( disable : 4786 )

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <commctrl.h>
#include "utils.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

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

// UI相关 -----------------------------------------------------------------
// 窗口相关
void Window_Center( HWND hWnd, HWND hRelativeWnd )
{
	INT cx, cy, offsetLeft, offsetTop;
	if ( hRelativeWnd && IsWindow(hRelativeWnd) )
	{
		RECT rcWnd;
		GetWindowRect( hRelativeWnd, &rcWnd );
		cx = rcWnd.right - rcWnd.left;
		cy = rcWnd.bottom - rcWnd.top;
		offsetLeft = rcWnd.left;
		offsetTop = rcWnd.top;
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
	SetWindowPos( hWnd, NULL, left, top, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOREDRAW );
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

