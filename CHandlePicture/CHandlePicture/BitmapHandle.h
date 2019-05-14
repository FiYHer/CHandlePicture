#pragma once

#include <windows.h>

/*
位图的处理方式就是从下到上，从左到右 understand???
*/


class CBitmapHandle
{
private:
	//图像DIB指针
	LPBYTE m_lpBitDIB;

	//颜色表长度
	INT m_nBitColorTableLen;

	//图像数据指针
	LPBYTE m_lpBitData;

	//每个像素占的位数
	INT m_nBitBitCount;

	//图片的大小
	INT m_nBitSize;

	//图像的宽度
	INT m_nBitWidth;

	//图像的高度
	INT m_nBitHeight;

	//图像信息头指针
	LPBITMAPINFOHEADER m_lpBitInfoHead;
private:
	//初始化
	VOID InitBitmapHandle();

	//清理空间
	VOID ClearBitmapHandle();
public:
	CBitmapHandle();
	virtual ~CBitmapHandle();

	CBitmapHandle(const CBitmapHandle&) = delete;
	CBitmapHandle& operator=(const CBitmapHandle&) = delete;
	CBitmapHandle(CBitmapHandle&&) = delete;
public:
	//DIB读取
	BOOL ReadBit(CONST PCHAR szBitPath);

	//DIB写入
	BOOL WriteBit(CONST PCHAR szBitPath);

	//获取颜色表的长度
	INT GetBitColorLen(INT nLen);

	//图像的绘制
	/*
	@hdc:上下文
	@nBeginX:x轴开始坐标
	@nBeginY:y轴开始坐标
	@lpBmpHeadInfo:位图信息头指针
	@BmpData:位图数据指针
	*/
	virtual VOID BitDraw(HDC hdc,
		INT nBeginX = NULL,
		INT nBeginY = NULL,
		LPBITMAPINFOHEADER lpBmpHeadInfo = nullptr,
		LPBYTE BmpData = nullptr);

	//返回位图数据的常量指针,给需要自己绘图的人
	LPBYTE BitDIB(INT &nBmpSize)CONST;

	//返回位图信息头结构体指针
	LPBITMAPINFOHEADER BitInfo()CONST;
};
