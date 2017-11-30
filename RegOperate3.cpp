/*此模块封装了一些注册表操作的API函数.*/
#include <Windows.h>
#include <string>
#include <vector>
#include <tchar.h>
#include "utils.h"
#include "RegOperate3.h"
//--------定义全局数据-----------
DWORD g_reg_type = REG_DWORD, g_size = sizeof(DWORD), g_reg_error = ERROR_NON;
String g_result;
//-----------------------------------
/*******************************************************************/
/*reg_open_key获取键句柄*/
REGOPERATE_IMPL_FUNC(HKEY) reg_open_key( LPCTSTR key_name, BOOL w /*= FALSE*/ )
{
	HKEY base_key_handle = NULL, key_handle = NULL;
	LPCTSTR str = _tcschr( key_name, TEXT('\\') );
	if ( !str ) str = key_name + _tcslen(key_name);	//只是根键的情况.
	if ( !_tcsnicmp( key_name, TEXT("HKEY_CLASSES_ROOT"), str - key_name ) || !_tcsnicmp( key_name, TEXT("HKCR"), str - key_name ) )
		base_key_handle = HKEY_CLASSES_ROOT;
	else if ( !_tcsnicmp( key_name, TEXT("HKEY_CURRENT_CONFIG"), str - key_name ) || !_tcsnicmp( key_name, TEXT("HKCC"), str - key_name ) )
		base_key_handle = HKEY_CURRENT_CONFIG;
	else if ( !_tcsnicmp( key_name, TEXT("HKEY_CURRENT_USER"), str - key_name ) || !_tcsnicmp( key_name, TEXT("HKCU"), str - key_name ) )
		base_key_handle = HKEY_CURRENT_USER;
	else if ( !_tcsnicmp( key_name, TEXT("HKEY_LOCAL_MACHINE"), str - key_name ) || !_tcsnicmp( key_name, TEXT("HKLM"), str - key_name ) )
		base_key_handle = HKEY_LOCAL_MACHINE;
	else if ( !_tcsnicmp( key_name, TEXT("HKEY_USERS"), str - key_name ) || !_tcsnicmp( key_name, TEXT("HKU"), str - key_name ) )
		base_key_handle = HKEY_USERS;
	else
	{
		g_reg_error |= ERROR_KEYNAME;
		return NULL;
	}
	if ( !*str )
	{
		return base_key_handle;
	}
	str++;	//跳过'\\'
	if ( base_key_handle )
	{
		if ( w )  //以写方式,若以存在,则打开,不存在则创建.
			RegCreateKey( base_key_handle, str, &key_handle );
		else
			RegOpenKey( base_key_handle, str, &key_handle );
	}
	return key_handle;
}

REGOPERATE_IMPL_FUNC(int) reg_close_key( HKEY key_handle )
{
	switch ( (DWORD)key_handle )
	{
	case (DWORD)HKEY_CLASSES_ROOT:
	case (DWORD)HKEY_CURRENT_USER:
	case (DWORD)HKEY_LOCAL_MACHINE:
	case (DWORD)HKEY_USERS:
	case (DWORD)HKEY_CURRENT_CONFIG:
		return 0;
	default:
		return RegCloseKey(key_handle);
	}
}
/*******************************************************************/
/*获取缓冲区大小*/
/*DWORD GetBufferLength(DWORD RegType,const LPBYTE buffer) {
	switch(RegType) {
	case REG_SZ:
	case REG_EXPAND_SZ:
		return _tcslen((LPCTSTR)buffer)+1;
//	case REG_BINARY:
	case REG_DWORD:
//	case REG_DWORD_LITTLE_ENDIAN:
	case REG_DWORD_BIG_ENDIAN:
		return 4;
//	case REG_LINK:
	case REG_MULTI_SZ:
		return _tcslen((LPCTSTR)buffer)+2;
//	case REG_RESOURCE_LIST:
//	case REG_FULL_RESOURCE_DESCRIPTOR:
//	case REG_RESOURCE_REQUIREMENTS_LIST:
	case REG_QWORD:
		return 8;
//	case REG_QWORD_LITTLE_ENDIAN:
	default:
	return 0;
	}
}*/
/*******************************************************************/
REGOPERATE_IMPL_FUNC(int) reg_write_ex( HKEY key_handle, LPCTSTR value_name, DWORD reg_type, BYTE const * buffer, int size )
{
	if ( key_handle )
	{
		if ( buffer )
		{
			RegSetValueEx( key_handle, value_name, 0, reg_type, buffer, size );
			g_reg_type = reg_type;
			g_size = size;
		}
		return ERROR_NON;
	}
	return ERROR_KEYNAME;
}

REGOPERATE_IMPL_FUNC(int) reg_write_string( HKEY key_handle, LPCTSTR value_name, LPCTSTR str )
{
	return reg_write_ex( key_handle, value_name, REG_SZ,(LPBYTE const)str, _tcslen(str) + 1 );
}

REGOPERATE_IMPL_FUNC(int) reg_write_long( HKEY key_handle, LPCTSTR value_name, DWORD value )
{
	return reg_write_ex( key_handle, value_name, REG_DWORD, (BYTE const *)&value, sizeof(DWORD) );
}

/*******************************************************************/
REGOPERATE_IMPL_FUNC(int) reg_read_ex( HKEY key_handle, LPCTSTR value_name, LPBYTE buffer, LPDWORD reg_type_, LPDWORD buf_size_ )
{
	int err = ERROR_NON;
	if( buffer )		// 缓冲区不空
	{
		if( key_handle )	//句柄不空
		{
			if( RegQueryValueEx( key_handle, value_name, NULL, reg_type_, buffer, buf_size_ ) ) err |= ERROR_VALUENAME;
			g_reg_type = *reg_type_;
			g_size = *buf_size_;
		}
		else err |= ERROR_KEYNAME;
	}
	else err |= ERROR_BUFFER;
	return err;
}
/*******************************************************************/
REGOPERATE_IMPL_FUNC(int) reg_read_string( HKEY key_handle, LPCTSTR value_name, LPTSTR buffer, DWORD size )
{
	DWORD reg_type;
	return reg_read_ex( key_handle, value_name, (LPBYTE)buffer, &reg_type, &size );
}
/*******************************************************************/
REGOPERATE_IMPL_FUNC(int) reg_read_long( HKEY key_handle, LPCTSTR value_name, LPDWORD value_ )
{
	DWORD size = sizeof(DWORD), reg_type;
	return reg_read_ex( key_handle, value_name,(LPBYTE)value_, &reg_type, &size );
}
/*******************************************************************/
REGOPERATE_IMPL_FUNC(int) reg_delete( LPCTSTR key_name, LPCTSTR value_name /*= NULL */ )
{
	int err = ERROR_NON;
	if ( value_name && *value_name ) //若值名不为空,则进行删除值的操作
	{
		HKEY key_handle = reg_open_key(key_name);
		if ( key_handle )
		{
			if ( RegDeleteValue( key_handle, value_name ) ) err |= ERROR_VALUENAME;
			if ( reg_close_key(key_handle) ) err |= ERROR_OTHER;
		}
		else err |= ERROR_KEYNAME;
	}
	else //值名为空,则删除键
	{
		TCHAR * p = (TCHAR *)_tcsrchr( key_name, '\\' );
		if ( p && *( p + 1 ) )	//不是根键
		{
			TCHAR parent_key[512] = {0};
			TCHAR const * sub_key;
			_tcsncpy( parent_key, key_name, p - key_name );
			p++; // skip '\\'
			sub_key = p;
			/*-----------以下是删除键----------*/
			HKEY key_handle = reg_open_key(parent_key);
			if ( key_handle )
			{
				if ( RegDeleteKey( key_handle, sub_key ) ) err |= ERROR_VALUENAME;
				if ( reg_close_key(key_handle) ) err |= ERROR_OTHER;
			}
			else err |= ERROR_KEYNAME;
		}
		else err |= ERROR_KEYNAME;	// 是根键的情况下,不能删除
	}
	return err;
}

REGOPERATE_IMPL_FUNC(int) reg_force_delete( LPCTSTR key_name )
{
	String key_buff;
	key_buff.resize(256);
	bool exist_subkey = false;
	HKEY key_handle = reg_open_key(key_name);
	if ( !key_handle ) return ERROR_KEYNAME;
	int i = 0;
	for( ; !RegEnumKey( key_handle, i, &key_buff[0], key_buff.size() ); i++ )
	{
		reg_force_delete( String( String(key_name) + TEXT("\\") + key_buff ).c_str() );
	}
	reg_close_key(key_handle);
	reg_delete(key_name);
	return 0;
}

REGOPERATE_IMPL_FUNC(BOOL) reg_error_info( int error_code, LPTSTR info_buff, int size )
{
	if ( !info_buff ) return FALSE;
	memset( info_buff, 0, size );
	if ( error_code == ERROR_NON )
	{
		_tcscat( info_buff, TEXT("没有错误!\n") );
		return TRUE;
	}
	if ( error_code & ERROR_KEYNAME ) _tcscat( info_buff, TEXT("键名错误!\n") );
	if ( error_code & ERROR_VALUENAME ) _tcscat( info_buff, TEXT("值名错误!\n") );
	if ( error_code & ERROR_REGTYPE ) _tcscat( info_buff, TEXT("类型错误!\n") );
	if ( error_code & ERROR_BUFFER ) _tcscat( info_buff, TEXT("缓冲区错误!\n") );
	if ( error_code & ERROR_OTHER ) _tcscat( info_buff, TEXT("其他错误!\n") );
	return TRUE;
}

REGOPERATE_IMPL_FUNC(DWORD) reg_query_value_size( HKEY key_handle, LPCTSTR value_name )
{
	if ( key_handle )
	{
		RegQueryValueEx( key_handle, value_name, NULL, &g_reg_type, NULL, &g_size );
		return g_size;
	}
	return 0;
}

REGOPERATE_IMPL_FUNC(LPCTSTR) reg_get_string( LPCTSTR key_name, LPCTSTR value_name )
{
	HKEY key_handle = reg_open_key(key_name);
	DWORD size = reg_query_value_size( key_handle, value_name );
	g_result.resize(size);
	reg_read_string( key_handle, value_name, &g_result[0], size );
	reg_close_key(key_handle);
	return g_result.c_str();
}

REGOPERATE_IMPL_FUNC(DWORD) reg_get_long( LPCTSTR key_name, LPCTSTR value_name )
{
	DWORD val;
	HKEY key_handle = reg_open_key(key_name);
	reg_read_long( key_handle, value_name, &val );
	reg_close_key(key_handle);
	return val;
}

REGOPERATE_IMPL_FUNC(int) reg_set_string( LPCTSTR key_name, LPCTSTR value_name, LPCTSTR str )
{
	HKEY key_handle = reg_open_key(key_name);
	int err = reg_write_string( key_handle, value_name, str );
	reg_close_key(key_handle);
	return err;
}

REGOPERATE_IMPL_FUNC(int) reg_set_long( LPCTSTR key_name, LPCTSTR value_name, DWORD value )
{
	HKEY key_handle = reg_open_key(key_name);
	int err = reg_write_long( key_handle, value_name, value );
	reg_close_key(key_handle);
	return err;
}

REGOPERATE_IMPL_FUNC(DWORD) reg_cur_reg_type( void )
{
	return g_reg_type;
}

REGOPERATE_IMPL_FUNC(DWORD) reg_cur_data_size( void )
{
	return g_size;
}


REGOPERATE_IMPL_FUNC(int) reg_enum_value( HKEY key_handle, REG_VALUE_ARRAY * value_arr_ )
{
	std::vector<REG_VALUE> value_vct;
	DWORD const value_name_size_const = 64;
	TCHAR value_name[value_name_size_const] = {0};
	DWORD value_name_size = value_name_size_const;
	DWORD reg_type, data_size;
	memset( value_name, 0, sizeof(TCHAR) * value_name_size_const );
	for ( int i = 0; ERROR_NO_MORE_ITEMS != RegEnumValue( key_handle, i, value_name, &value_name_size, NULL, &reg_type, NULL, &data_size ); ++i )
	{
		REG_VALUE rv;
		rv.value_name = new TCHAR[value_name_size + 1];
		_tcsncpy( rv.value_name, value_name, value_name_size );
		rv.value_name[value_name_size] = 0;
		rv.reg_type = reg_type;
		rv.data_size = data_size;
		value_vct.push_back(rv);
		memset( value_name, 0, sizeof(TCHAR) * value_name_size_const );
		value_name_size = value_name_size_const;
	}

	int count = value_vct.size();
	*value_arr_ = new REG_VALUE[count + 1];
	memcpy( *value_arr_, &value_vct[0], count * sizeof(REG_VALUE) );
	(*value_arr_)[count].value_name = NULL;
	(*value_arr_)[count].reg_type = 0;
	(*value_arr_)[count].data_size = 0;
	return count;
}

REGOPERATE_IMPL_FUNC(void) reg_free_value_array( REG_VALUE_ARRAY value_arr )
{
	REG_VALUE * p = value_arr;
	while ( p->value_name != NULL )
	{
		delete [] p->value_name;
		p++;
	}
	delete [] value_arr;
}