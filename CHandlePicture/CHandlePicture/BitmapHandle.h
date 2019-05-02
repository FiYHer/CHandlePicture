#pragma once

#include <windows.h>

/*
λͼ�Ĵ���ʽ���Ǵ��µ��ϣ������� understand???
*/


class CBitmapHandle
{
private:
	//ͼ��DIBָ��
	LPBYTE m_lpBitmapDibBuffer;

	//��ɫ����
	INT m_nBitmapColorTableLen;

	//ͼ������ָ��
	LPBYTE m_lpBitmapDataBuffer;

	//ÿ������ռ��λ��
	INT m_nBitmapBitCount;

	//ͼƬ�Ĵ�С
	INT m_nBitmapSize;

	//ͼ��Ŀ��
	INT m_nBitmapWidth;

	//ͼ��ĸ߶�
	INT m_nBitmapHeight;

	//ͼ����Ϣͷָ��
	LPBITMAPINFOHEADER m_lpBitmapInfoHead;
private:
	//��ʼ��
	VOID InitBitmapHandle();

	//����ռ�
	VOID ClearBitmapHandle();
public:
	CBitmapHandle();
	virtual ~CBitmapHandle();

	CBitmapHandle(const CBitmapHandle&) = delete;
	CBitmapHandle& operator=(const CBitmapHandle&) = delete;
	CBitmapHandle(CBitmapHandle&&) = delete;
public:
	//DIB��ȡ
	BOOL ReadBitmap(CONST PCHAR pszBitmapFilePath);

	//DIBд��
	BOOL WriteBitmap(CONST PCHAR pszBitmapFilePath);

	//��ȡ��ɫ��ĳ���
	INT BmpGetColorTableLen(INT);

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

	//����λͼ���ݵĳ���ָ��,����Ҫ�Լ���ͼ����
	LPBYTE GetImgData(INT &nBmpSize)CONST;

	//����λͼ��Ϣͷ�ṹ��ָ��
	LPBITMAPINFOHEADER GetImgInfoHead()CONST;
};
