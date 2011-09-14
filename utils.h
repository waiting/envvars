// utils.h: interface for the utils class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UTILS_H__CC4A0943_DFA5_46E4_BA2C_60D59EB89599__INCLUDED_)
#define AFX_UTILS_H__CC4A0943_DFA5_46E4_BA2C_60D59EB89599__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include <string>
// 类型支持 ---------------------------------------------------------------
typedef std::string AnsiString;
typedef AnsiString LocalString;
typedef std::wstring UnicodeString;

#ifndef USE_TCHAR_DECLARE
	#ifdef UNICODE
		typedef UnicodeString String;
	#else
		typedef LocalString String;
	#endif
#else
	typedef std::basic_string<TCHAR> String;
#endif

typedef std::vector<AnsiString> AnsiStringArray;
typedef std::vector<LocalString> LocalStringArray;
typedef std::vector<UnicodeString> UnicodeStringArray;
typedef std::vector<String> StringArray;

// 环境相关 ---------------------------------------------------------------
bool IsExpandString( LPCTSTR lpsz );
String ExplainEnvVars( LPCTSTR lpsz );

// 字串相关 ---------------------------------------------------------------
/* 返回一个字符串里有多少个字符(by local CodePage),用于mbstowcs */
UINT LocalCharsCount( LPCSTR lpszLocal );
/* 返回一个unicode字符串最少长度(by local CodePage),用于wcstombs */
UINT UnicodeMinLength( LPCWSTR lpszUnicode );
/* Unicode转换到本地Ansi */
LocalString UnicodeToLocal( UnicodeString const & strUnicode );
/* 本地ANSI转到Unicode */
UnicodeString LocalToUnicode( LocalString const & strLocal );

/* 兼容字符串与Unicode,Local字符串相互转换 */
String LocalToString( LocalString const & strLocal );
String UnicodeToString( UnicodeString const & strUnicode );
LocalString StringToLocal( String const & str );
UnicodeString StringToUnicode( String const & str );

/* UTF8与兼容串之间的转换 */
String UTF8ToString( AnsiString const & strUTF8 );
AnsiString StringToUTF8( String const & str );

int StrSplit( LPCTSTR lpsz, LPCTSTR lpszDelim, StringArray * pArr );
String StrJoin( LPCTSTR lpszDelim, StringArray const & arr );
String StrInsert( LPCTSTR lpsz, int iStart, int iEnd, LPCTSTR lpsz2 );
String OutputExV( int cchMsg, LPCTSTR lpszFormat, va_list args );
String OutputEx( int cchMsg, LPCTSTR lpszFormat, ... );
String Output( LPCTSTR lpszFormat, ... );

// 资源相关 ---------------------------------------------------------------
/* 载入字符串资源 */
UnicodeString LoadStringResExW( HMODULE hModule, UINT uStringResID );
String LoadStringResEx( HMODULE hModule, UINT uStringResID );
String LoadStringRes( UINT uStringResID );

// 系统相关 ---------------------------------------------------------------
/** 使能关机 */
bool ShutdownPrivilege( bool bEnable );

ULONGLONG GetUTCTimeMS( void );
UINT GetUTCTime( void );
void GetUTCSysTimeByUTCMS( ULONGLONG ullMilliseconds, LPSYSTEMTIME lpSysTime );
void GetLocalSysTimeByUTCMS( ULONGLONG ullMilliseconds, LPSYSTEMTIME lpSysTime );

// UI相关 -----------------------------------------------------------------
// 窗口相关

/* 以相对窗口为准居中窗口 */
void Window_Center( HWND hWnd, HWND hRelativeWnd );
/* 窗口文本 */
String Window_GetText( HWND hWnd );
void Window_SetText( HWND hWnd, String const & str );

// ListView操作
void ListView_AddStrings( HWND hListView, int nArgCount, ... );
void ListView_InsertStrings( HWND hListView, int iItemIndex, int nArgCount, ... );
void ListView_SetStrings( HWND hListView, int iItemIndex, int nArgCount, ... );
void ListView_GetStrings( HWND hListView, int iItemIndex, int nArgPairCount, ... );

// 图形图像相关 -----------------------------------------------------------
BOOL SaveBitmapToFile( HBITMAP hBitmap, LPSTR lpszFileName );


#endif // !defined(AFX_UTILS_H__CC4A0943_DFA5_46E4_BA2C_60D59EB89599__INCLUDED_)
