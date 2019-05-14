#pragma once

#include <windows.h>

/*
λͼ�Ĵ���ʽ���Ǵ��µ��ϣ������� understand???
*/


class CBitmapHandle
{
private:
	//ͼ��DIBָ��
	LPBYTE m_lpBitDIB;

	//��ɫ����
	INT m_nBitColorTableLen;

	//ͼ������ָ��
	LPBYTE m_lpBitData;

	//ÿ������ռ��λ��
	INT m_nBitBitCount;

	//ͼƬ�Ĵ�С
	INT m_nBitSize;

	//ͼ��Ŀ��
	INT m_nBitWidth;

	//ͼ��ĸ߶�
	INT m_nBitHeight;

	//ͼ����Ϣͷָ��
	LPBITMAPINFOHEADER m_lpBitInfoHead;
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
	BOOL ReadBit(CONST PCHAR szBitPath);

	//DIBд��
	BOOL WriteBit(CONST PCHAR szBitPath);

	//��ȡ��ɫ��ĳ���
	INT GetBitColorLen(INT nLen);

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

	//����λͼ���ݵĳ���ָ��,����Ҫ�Լ���ͼ����
	LPBYTE BitDIB(INT &nBmpSize)CONST;

	//����λͼ��Ϣͷ�ṹ��ָ��
	LPBITMAPINFOHEADER BitInfo()CONST;
};
