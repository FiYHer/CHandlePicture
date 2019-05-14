#pragma once

#include "BitmapHandle.h"

/*
���α任
*/

class CGeometricTransformation:public CBitmapHandle
{
private:
	//ԭλͼ����
	LPBYTE m_pBit;

	//��λͼ����
	LPBYTE m_pNewBit;

	//λͼ��Ϣͷ
	BITMAPINFOHEADER m_stBitInfo;
private:
	/*
	��ʼ��
	*/
	VOID Init();

	/*
	����
	*/
	VOID Clear();

	/*
	��ȡλͼ����
	*/
	VOID GetBitBuffer();

	/*
	��ȡλͼ��Ϣͷ�ṹ��
	*/
	VOID GetBitInfo();

public:
	CGeometricTransformation();
	~CGeometricTransformation();
public:
	/*
	ƽ��
	*/
	VOID BitMove(INT nOffsetX,INT nOffsetY);

	/*
	����
	*/
	VOID Zoom(double dX,double dY);

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
	virtual VOID BitDraw(HDC hdc, 
		INT nBeginX = NULL,
		INT nBeginY = NULL,
		LPBITMAPINFOHEADER lpBmpHeadInfo = nullptr,
		LPBYTE BmpData = nullptr);

};

