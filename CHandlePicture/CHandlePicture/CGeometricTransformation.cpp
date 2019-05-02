#include "CGeometricTransformation.h"


VOID CGeometricTransformation::InitGeometricTransformation()
{
	m_lpOldBuffer = nullptr;
	m_lpNewBuffer = nullptr;
	m_stInfoHead = { 0 };
}

VOID CGeometricTransformation::ClearGeometricTransformation()
{
	if (m_lpNewBuffer != nullptr)
	{
		delete[] m_lpNewBuffer;
		m_lpNewBuffer = nullptr;
	}
}

VOID CGeometricTransformation::GetBitmapData()
{
	INT nBmpSize = NULL;

	//��ȡλͼ���ݺ����ݴ�С
	m_lpOldBuffer = GetImgData(nBmpSize);
	if (m_lpOldBuffer == nullptr || nBmpSize == NULL)
	{
		return;
	}

	//���
	if (m_lpNewBuffer != nullptr)
	{
		delete[] m_lpNewBuffer;
		m_lpNewBuffer = nullptr;
	}

	//�����ڴ汣�渱��
	m_lpNewBuffer = new BYTE[nBmpSize];
	if (m_lpNewBuffer == nullptr)
	{
		return;
	}

	//���
	memset(m_lpNewBuffer, 0, sizeof(BYTE)*nBmpSize);
}

VOID CGeometricTransformation::GetBitmapInfoHead()
{
	LPBITMAPINFOHEADER lpInfoHead = nullptr;

	//��ȡλͼ��Ϣͷָ��
	lpInfoHead = GetImgInfoHead();
	if (lpInfoHead == nullptr)
	{
		return;
	}

	memcpy(&m_stInfoHead, lpInfoHead, sizeof(BITMAPINFOHEADER));
}

CGeometricTransformation::CGeometricTransformation()
{
	InitGeometricTransformation();
}

CGeometricTransformation::~CGeometricTransformation()
{
	ClearGeometricTransformation();
}

VOID CGeometricTransformation::BitmapMove(INT nOffsetX, INT nOffsetY)
{
	INT nBitmapLineLen = 0;
	INT nBitCount = 0;
	INT nHeight = 0;
	INT nWidth = 0;
	INT nBit = 0;

	//��ȡλͼ����
	GetBitmapData();
	if (m_lpNewBuffer == nullptr)
	{
		return;
	}

	//��ȡλͼ��Ϣͷ
	GetBitmapInfoHead();

	//��ȡλͼһ�еĴ�С
	nBitmapLineLen = (m_stInfoHead.biWidth*m_stInfoHead.biBitCount / 8 + 3) / 4 * 4;

	//ÿ�������ֽ���
	nBitCount = m_stInfoHead.biBitCount / 8;

	//�߶�����
	for (nHeight = 0; nHeight < m_stInfoHead.biHeight; nHeight++)
	{
		//�������
		for (nWidth = 0; nWidth < m_stInfoHead.biWidth; nWidth++)
		{
			//�жϵ��Ƿ��ڷ�Χ�ڣ����ڵĻ��Ͳ����д���
			if (nHeight - nOffsetY >= NULL &&
				nHeight - nOffsetY < m_stInfoHead.biHeight &&
				nWidth - nOffsetX >= NULL &&
				nWidth - nOffsetX < m_stInfoHead.biWidth)
			{
				//ѭ�����ص�ÿ��ͨ��
				for (nBit = 0; nBit < nBitCount; nBit++)
				{
					//������

					//nHeight * nBitmapLineLen
					//������Ǵ���ڼ��У����ϵ���

					//nWidth * nBitCount
					//������Ǵ���ڼ��У�������

					//nBit������ǵ��RGB

					//����Ҳ�������ӽ���

					*(m_lpNewBuffer +
						nHeight * nBitmapLineLen +
						nWidth * nBitCount +
						nBit)
						=
						*(m_lpOldBuffer +
						(nHeight - nOffsetY)*nBitmapLineLen +
							(nWidth - nOffsetX)*nBitCount +
							nBit);
				}
			}
		}
	}
}

VOID CGeometricTransformation::BitmapHorizontal()
{
	INT nBitmapLineLen = 0;
	INT nBitCount = 0;
	INT nHeight = 0;
	INT nWidth = 0;
	INT nBit = 0;

	//��ȡλͼ����
	GetBitmapData();
	if (m_lpNewBuffer == nullptr)
	{
		return;
	}

	//��ȡλͼ��Ϣͷ
	GetBitmapInfoHead();

	//��ȡλͼһ�еĴ�С
	nBitmapLineLen = (m_stInfoHead.biWidth*m_stInfoHead.biBitCount / 8 + 3) / 4 * 4;

	//ÿ�������ֽ���
	nBitCount = m_stInfoHead.biBitCount / 8;

	//�߶�����
	for (nHeight = 0; nHeight < m_stInfoHead.biHeight; nHeight++)
	{
		//�������
		for (nWidth = 0; nWidth < m_stInfoHead.biWidth; nWidth++)
		{
			//ÿ������ͨ��
			for (nBit = 0; nBit < nBitCount; nBit++)
			{
				*(m_lpNewBuffer +
					nHeight * nBitmapLineLen +
					nWidth * nBitCount +
					nBit)
					=
					*(m_lpOldBuffer +
						nHeight * nBitmapLineLen +
						(m_stInfoHead.biWidth - 1 - nWidth)*nBitCount +
						nBit);
			}
		}
	}
}

VOID CGeometricTransformation::BitmapVertically()
{
	INT nBitmapLineLen = 0;
	INT nBitCount = 0;
	INT nHeight = 0;
	INT nWidth = 0;
	INT nBit = 0;

	//��ȡλͼ����
	GetBitmapData();
	if (m_lpNewBuffer == nullptr)
	{
		return;
	}

	//��ȡλͼ��Ϣͷ
	GetBitmapInfoHead();

	//��ȡλͼһ�еĴ�С
	nBitmapLineLen = (m_stInfoHead.biWidth*m_stInfoHead.biBitCount / 8 + 3) / 4 * 4;

	//ÿ�������ֽ���
	nBitCount = m_stInfoHead.biBitCount / 8;

	//�߶�����
	for (nHeight = 0; nHeight < m_stInfoHead.biHeight; nHeight++)
	{
		//�������
		for (nWidth = 0; nWidth < m_stInfoHead.biWidth; nWidth++)
		{
			//ÿ������ͨ��
			for (nBit = 0; nBit < nBitCount; nBit++)
			{
				*(m_lpNewBuffer +
					nHeight * nBitmapLineLen +
					nWidth * nBitCount +
					nBit)
					=
					*(m_lpOldBuffer +
					(m_stInfoHead.biHeight - 1 - nHeight)*nBitmapLineLen +
						nWidth * nBitCount +
						nBit);
			}
		}
	}
}

VOID CGeometricTransformation::Clockwise90()
{
	INT nBitmapLineLen = 0;
	INT nBitCount = 0;
	INT nHeight = 0;
	INT nWidth = 0;
	INT nBit = 0;

	INT nReWidth = 0;
	INT nReHeight = 0;
	INT nBitmapLineLenOut = 0;

	//��ȡλͼ����
	GetBitmapData();
	if (m_lpNewBuffer == nullptr)
	{
		return;
	}

	//��ȡλͼ��Ϣͷ
	GetBitmapInfoHead();

	//��ȡλͼһ�еĴ�С
	nBitmapLineLen = (m_stInfoHead.biWidth*m_stInfoHead.biBitCount / 8 + 3) / 4 * 4;

	//ÿ�������ֽ���
	nBitCount = m_stInfoHead.biBitCount / 8;

	//������Ҫ��ת���������ԱȽ����⣬һ����Ҫ�ѿ�Ⱥ͸߶Ȼ���ת��
	//����������εĻ����ǾͲ���Ҫ�ٴν��д�����
	if (m_stInfoHead.biHeight != m_stInfoHead.biWidth)
	{
		nReHeight = m_stInfoHead.biWidth;
		nReWidth = m_stInfoHead.biHeight;

		//��ȡһ�еĴ�С
		nBitmapLineLenOut = (nReWidth*m_stInfoHead.biBitCount / 8 + 3) / 4 * 4;

		//�ͷ��ڴ�
		delete[] m_lpNewBuffer;
		m_lpNewBuffer = new BYTE[nBitmapLineLenOut*nReHeight];
		if (m_lpNewBuffer == nullptr)
		{
			return;
		}
		memset(m_lpNewBuffer, 0, nBitmapLineLenOut*nReHeight);
	}
	else
	{
		nReHeight = m_stInfoHead.biHeight;
		nReWidth = m_stInfoHead.biWidth;
		nBitmapLineLenOut = nBitmapLineLen;
	}

	//�߶�����
	for (nHeight = 0; nHeight < nReHeight; nHeight++)
	{
		//�������
		for (nWidth = 0; nWidth < nReWidth; nWidth++)
		{
			//ÿ������ͨ��
			for (nBit = 0; nBit < nBitCount; nBit++)
			{
				*(m_lpNewBuffer +
					nHeight * nBitmapLineLenOut +
					nWidth * nBitCount +
					nBit) 
					= 
					*(m_lpOldBuffer +
						nWidth * nBitmapLineLen +
						(m_stInfoHead.biWidth - 1 - nHeight)*nBitCount +
						nBit);
			}
		}
	}
}

VOID CGeometricTransformation::BitmapDraw(HDC hdc, 
	INT nBeginX /*= NULL*/, 
	INT nBeginY /*= NULL*/, 
	LPBITMAPINFOHEADER lpBmpHeadInfo /*= nullptr*/,
	LPBYTE BmpData /*= nullptr*/)
{
	CBitmapHandle::BitmapDraw(hdc, nBeginX, nBeginY, lpBmpHeadInfo, m_lpNewBuffer);
}
