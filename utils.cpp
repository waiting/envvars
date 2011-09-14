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

String OutputExV( int cchMsg, LPCTSTR lpszFormat, va_list args )
{
	String str;
	str.resize(cchMsg);
	_vsntprintf( &str[0], cchMsg, lpszFormat, args );
	return str;
}

String OutputEx( int cchMsg, LPCTSTR lpszFormat, ... )
{
	va_list args;
	va_start( args, lpszFormat );
	return OutputExV( cchMsg, lpszFormat, args );
}

String Output( LPCTSTR lpszFormat, ... )
{
	va_list args;
	va_start( args, lpszFormat );
	return OutputExV( 1024, lpszFormat, args );
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

// 图形图像相关 -----------------------------------------------------------
BOOL SaveBitmapToFile( HBITMAP hBitmap, LPSTR lpszFileName )
{
	HDC hDC;//设备描述表
	int iBits;  //当前显示分辨率下每个像素所占字节数
	WORD wBitCount = 0;    //位图中每个像素所占字节数
	//定义调色板大小，位图中像素字节大小，位图文件大小，写入文件字节数
	DWORD dwPaletteSize=0, dwBmBitsSize, dwDIBSize, dwWritten;
	BITMAP Bitmap; //位图属性结构
	BITMAPFILEHEADER bmfHdr; //位图文件头结构
	BITMAPINFOHEADER bi;//位图信息头结构
	LPBITMAPINFOHEADER lpbi; //指向位图信息头结构
	HANDLE fh, hDib;
	HPALETTE hPal, hOldPal = NULL; //定义文件，分配内存句柄，调色板句柄
	//计算位图文件每个像素所占字节数
	hDC = CreateDC( "DISPLAY", NULL, NULL, NULL );
	iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
	DeleteDC(hDC);
	if (iBits <= 1)
		wBitCount = 1;
	else if (iBits <= 4)
		wBitCount = 4;
	else if (iBits <= 8)
		wBitCount = 8;
	else if (iBits <= 24)
		wBitCount = 24;
	else
		wBitCount = 32;
	//计算调色板大小
	if (wBitCount <= 8)
		dwPaletteSize = (1 << wBitCount) * sizeof(RGBQUAD);
	
	//设置位图信息头结构
	GetObject(hBitmap, sizeof(BITMAP), &Bitmap);
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = Bitmap.bmWidth;
	bi.biHeight = Bitmap.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = wBitCount;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;
	
	dwBmBitsSize = ( ( Bitmap.bmWidth * wBitCount + 31 ) / 32 ) * 4 * Bitmap.bmHeight;
	//为位图内容分配内存
	hDib = GlobalAlloc( GHND, dwBmBitsSize + dwPaletteSize + sizeof( BITMAPINFOHEADER ) );
	lpbi = ( LPBITMAPINFOHEADER )GlobalLock( hDib );
	*lpbi = bi;
	//   处理调色板      
	hPal = ( HPALETTE )GetStockObject(DEFAULT_PALETTE);
	if ( hPal )
	{
		hDC = GetDC(NULL);
		hOldPal = SelectPalette( hDC, hPal, FALSE );
		RealizePalette(hDC);
	}
	//   获取该调色板下新的像素值
	GetDIBits(hDC, hBitmap, 0,(UINT)Bitmap.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER) + dwPaletteSize, ( LPBITMAPINFO )lpbi, DIB_RGB_COLORS);
	//恢复调色板
	if (hOldPal)
	{
		SelectPalette(hDC, hOldPal, TRUE);
		RealizePalette(hDC);
		ReleaseDC(NULL, hDC);
	}
	//创建位图文件        
	fh = CreateFile(lpszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (fh == INVALID_HANDLE_VALUE)
		return FALSE;
	//   设置位图文件头
	bmfHdr.bfType = 0x4D42;     //   "BM"
	dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
	bmfHdr.bfSize = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;
	//   写入位图文件头
	WriteFile(fh, &bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
	//   写入位图文件其余内容
	WriteFile(fh, lpbi, dwDIBSize, &dwWritten, NULL);
	//清除
	GlobalUnlock(hDib);
	GlobalFree(hDib);
	CloseHandle(fh);
	return TRUE;
}
