#pragma once

#include "BitmapHandle.h"

/*
几何变换
*/

class CGeometricTransformation:public CBitmapHandle
{
private:
	//原位图数据
	LPBYTE m_lpOldBuffer;

	//新位图数据
	LPBYTE m_lpNewBuffer;

	//位图信息头
	BITMAPINFOHEADER m_stInfoHead;
private:
	/*
	初始化
	*/
	VOID InitGeometricTransformation();

	/*
	清理
	*/
	VOID ClearGeometricTransformation();

	/*
	获取位图数据
	*/
	VOID GetBitmapData();

	/*
	获取位图信息头结构体
	*/
	VOID GetBitmapInfoHead();

public:
	CGeometricTransformation();
	~CGeometricTransformation();
public:
	/*
	位图平移
	*/
	VOID BitmapMove(INT nOffsetX,INT nOffsetY);

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
	virtual VOID BitmapDraw(HDC hdc, 
		INT nBeginX = NULL,
		INT nBeginY = NULL,
		LPBITMAPINFOHEADER lpBmpHeadInfo = nullptr,
		LPBYTE BmpData = nullptr);

};

