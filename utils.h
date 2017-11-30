// utils.h: interface for the utils class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UTILS_H__CC4A0943_DFA5_46E4_BA2C_60D59EB89599__INCLUDED_)
#define AFX_UTILS_H__CC4A0943_DFA5_46E4_BA2C_60D59EB89599__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning( disable : 4786 )

#include <ocidl.h>
#include <vector>
#include <string>
#include <gdiplus.h>
#include <math.h>
#include "MemImage.h"
#include "MemDC.h"

// 类型支持 ---------------------------------------------------------------
typedef std::basic_string<CHAR> AnsiString;
typedef AnsiString LocalString;
typedef std::basic_string<WCHAR> UnicodeString;
typedef std::basic_string<TCHAR> String;

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
String FormatExV( int cchMsg, LPCTSTR lpszFormat, va_list args );
String FormatEx( int cchMsg, LPCTSTR lpszFormat, ... );
String Format( LPCTSTR lpszFormat, ... );

// 资源相关 ---------------------------------------------------------------
/* 载入字符串资源 */
UnicodeString LoadStringResExW( HMODULE hModule, UINT uStringResID );
String LoadStringResEx( HMODULE hModule, UINT uStringResID );
String LoadStringRes( UINT uStringResID );

// 系统相关 ---------------------------------------------------------------
/* 返回模块路径，输出模块文件名 */
String ModulePath( HMODULE hModule = NULL, String * pStrFileName = NULL );
/* 获取路径名 */
String FilePath( String const & strFull, String * pStrFileName = NULL );
/* 获取一个文件名的标题和扩展名 */
String FileTitle( String const & strFileName, String * pStrExtName = NULL );
/* 判断是否是一个目录 */
BOOL IsDirectory( LPCTSTR lpszPath );

/* 获得程序的命令行参数 */
int GetCommandArguments( StringArray * pArr );
/* 使能关机 */
bool ShutdownPrivilege( bool bEnable );

// 时间相关 ---------------------------------------------------------------
ULONGLONG GetUTCTimeMS( void );
UINT GetUTCTime( void );
void GetUTCSysTimeByUTCMS( ULONGLONG ullMilliseconds, LPSYSTEMTIME lpSysTime );
void GetLocalSysTimeByUTCMS( ULONGLONG ullMilliseconds, LPSYSTEMTIME lpSysTime );

// UI相关 -----------------------------------------------------------------
/* 窗口定时器 */
class WindowTimer
{
public:
	WindowTimer( void );
	void Create( HWND hWnd, UINT uElapse );
	void Destroy( void );
	UINT GetID( void ) const;
	~WindowTimer( void );
private:
	HWND _hWnd;
	UINT_PTR _uID;
	static UINT_PTR _uIDAutoIncrement;
	
	WindowTimer( WindowTimer const & );
	WindowTimer & operator = ( WindowTimer const & );
};
// 窗口相关 ---------------------------------------------------------------
/* 弹出一个消息框 */
void MsgBox( String const & strMsg );

/* 获取客户区矩形 */
RECT Window_GetClient( HWND hWnd );
/* 获取窗口矩形,相对于父窗口 */
RECT Window_GetRect( HWND hWnd );
/* 设置窗口矩形,相对于父窗口 */
void Window_SetRect( HWND hWnd, LPCRECT lpRect );
/* 膨胀窗口 */
void Window_Inflate( HWND hWnd, int dx, int dy );
/* 以相对窗口为准居中窗口 */
void Window_Center( HWND hWnd, HWND hRelativeWnd, bool bInRelativeWnd = false );
/* 窗口文本 */
String Window_GetText( HWND hWnd );
void Window_SetText( HWND hWnd, String const & str );

// ListView操作
void ListView_AddStrings( HWND hListView, int nArgCount, ... );
void ListView_InsertStrings( HWND hListView, int iItemIndex, int nArgCount, ... );
void ListView_SetStrings( HWND hListView, int iItemIndex, int nArgCount, ... );
void ListView_GetStrings( HWND hListView, int iItemIndex, int nArgPairCount, ... );


#endif // !defined(AFX_UTILS_H__CC4A0943_DFA5_46E4_BA2C_60D59EB89599__INCLUDED_)
