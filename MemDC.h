// MemDC.h: interface for the MemDC class.
// 内存DC,管理绘制与位图显示
// 此类不得含有虚函数,继承此类意味着必须采用模板实现多态性
//////////////////////////////////////////////////////////////////////

#if !defined(__MEMDC_H__)
#define __MEMDC_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class MemDC
{
public:
	MemDC( void );
	MemDC( HDC hDC );
	MemDC( HDC hDC, int nWidth, int nHeight, COLORREF clrBackground = 0xFFFFFF );
	MemDC( MemDC & other );
	~MemDC( void );

	MemDC & operator = ( MemDC const & other );
	operator HDC( void ) const { return _hMemDC; }
	operator HBITMAP( void ) const { return _hBitmap; }
	operator BOOL( void ) const { return _hMemDC != NULL && _hBitmap != NULL; }
	int Width( void ) const { return _nWidth; }
	int Height( void ) const { return _nHeight; }
	//void SetSizes( SIZE si ) { _nWidth = si.cx; _nHeight = si.cy; }
	BOOL IsTransparent( void ) const { return _bIsTransparent; }
	COLORREF TransparentColor( void ) const { return _clrTransparent; }

	BOOL Create( HDC hDC );
	BOOL Create( HDC hDC, int nWidth, int nHeight );
	BOOL Create( HDC hDC, int nWidth, int nHeight, COLORREF clrBackground );
	BOOL Create( HDC hDC, int nWidth, int nHeight, COLORREF clrBackground, COLORREF clrTransparent );
	BOOL Copy( MemDC const & other );
	void Destroy( void );
	void EnableTransparent( BOOL bIsTransparent, COLORREF clrTransparent = 0 );
	void SetBackground( COLORREF clrBackground, BOOL bFill = FALSE );
	/* 管理一副位图,如果只是输出现有位图,则完毕后应该调用DetachBitmap()脱离管理 */
	HBITMAP AttachBitmap( HBITMAP hBitmap );
	/* 位图脱离管理 */
	HBITMAP DetachBitmap( void );
	/* 传递管理权,自己放弃管理资源 */
	BOOL PassTo( MemDC & other );
	/* 旋转 */
	BOOL RotateGdiplus( double angle, MemDC * pMemDC );
#ifdef _DEBUG
	BOOL RotateCxImage( double angle, MemDC * pMemDC );
#endif
	BOOL StretchBlt( HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, int x, int y, int width, int height, int nMode = HALFTONE ) const;
	BOOL StretchToDC( HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, int nMode = HALFTONE ) const;
	BOOL BitBlt( HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, int x, int y ) const;
	BOOL CopyToDC( HDC hDestDC, int xDest, int yDest ) const;
	BOOL TransparentBlt( HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, int x, int y, int width, int height, int nMode = HALFTONE ) const;
	BOOL TransparentToDC( HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, int nMode = HALFTONE ) const;

private:
	int _nWidth, _nHeight;
	HDC _hMemDC;
	HBITMAP _hBitmap;
	COLORREF _clrBackground; // 背景色
	COLORREF _clrTransparent; // 透明色
	BOOL _bIsTransparent; // 是否透明
};

#endif // !defined(__MEMDC_H__)
