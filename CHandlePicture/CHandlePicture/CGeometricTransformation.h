#pragma once

#include "BitmapHandle.h"

/*
几何变换
*/

class CGeometricTransformation:public CBitmapHandle
{
private:
	//原位图数据
	LPBYTE m_pBit;

	//新位图数据
	LPBYTE m_pNewBit;

	//位图信息头
	BITMAPINFOHEADER m_stBitInfo;
private:
	/*
	初始化
	*/
	VOID Init();

	/*
	清理
	*/
	VOID Clear();

	/*
	获取位图数据
	*/
	VOID GetBitBuffer();

	/*
	获取位图信息头结构体
	*/
	VOID GetBitInfo();

public:
	CGeometricTransformation();
	~CGeometricTransformation();
public:
	/*
	平移
	*/
	VOID BitMove(INT nOffsetX,INT nOffsetY);

	/*
	缩放
	*/
	VOID Zoom(double dX,double dY);

	/*
	水平镜像
	*/
	VOID BitmapHorizontal();

	/*
	垂直镜像
	*/
	VOID BitmapVertically();

	/*
	顺时针90
	*/
	VOID Clockwise90();


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

};

