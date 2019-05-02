#pragma once

#include "BitmapHandle.h"

/*
���α任
*/

class CGeometricTransformation:public CBitmapHandle
{
private:
	//ԭλͼ����
	LPBYTE m_lpOldBuffer;

	//��λͼ����
	LPBYTE m_lpNewBuffer;

	//λͼ��Ϣͷ
	BITMAPINFOHEADER m_stInfoHead;
private:
	/*
	��ʼ��
	*/
	VOID InitGeometricTransformation();

	/*
	����
	*/
	VOID ClearGeometricTransformation();

	/*
	��ȡλͼ����
	*/
	VOID GetBitmapData();

	/*
	��ȡλͼ��Ϣͷ�ṹ��
	*/
	VOID GetBitmapInfoHead();

public:
	CGeometricTransformation();
	~CGeometricTransformation();
public:
	/*
	λͼƽ��
	*/
	VOID BitmapMove(INT nOffsetX,INT nOffsetY);

	/*
	ˮƽ����
	*/
	VOID BitmapHorizontal();

	/*
	��ֱ����
	*/
	VOID BitmapVertically();

	/*
	˳ʱ��90
	*/
	VOID Clockwise90();


//ͼ��Ļ���
/*
@hdc:������
@nBeginX:x�Ὺʼ����
@nBeginY:y�Ὺʼ����
@lpBmpHeadInfo:λͼ��Ϣͷָ��
@BmpData:λͼ����ָ��
*/
	virtual VOID BitmapDraw(HDC hdc, 
		INT nBeginX = NULL,
		INT nBeginY = NULL,
		LPBITMAPINFOHEADER lpBmpHeadInfo = nullptr,
		LPBYTE BmpData = nullptr);

};

