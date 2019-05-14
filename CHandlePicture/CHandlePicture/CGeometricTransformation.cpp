#include "CGeometricTransformation.h"


VOID CGeometricTransformation::Init()
{
	m_pBit = nullptr;
	m_pNewBit = nullptr;
	m_stBitInfo = { 0 };
}

VOID CGeometricTransformation::Clear()
{
	if (m_pNewBit)
	{
		delete[] m_pNewBit;
		m_pNewBit = nullptr;
	}
}

VOID CGeometricTransformation::GetBitBuffer()
{
	INT nBitSize = NULL;

	//��ȡλͼ���ݺ����ݴ�С
	m_pBit = BitDIB(nBitSize);
	if (!m_pBit || !nBitSize)
	{
		return;
	}

	//���
	if (m_pNewBit)
	{
		delete[] m_pNewBit;
		m_pNewBit = nullptr;
	}

	//�����ڴ汣�渱��
	m_pNewBit = new BYTE[nBitSize];
	if (!m_pNewBit)
	{
		return;
	}

	//���
	ZeroMemory(m_pNewBit, nBitSize);
}

VOID CGeometricTransformation::GetBitInfo()
{
	LPBITMAPINFOHEADER lpInfoHead = nullptr;

	//��ȡλͼ��Ϣͷָ��
	lpInfoHead = BitInfo();
	if (!lpInfoHead)
	{
		return;
	}

	CopyMemory(&m_stBitInfo, lpInfoHead, sizeof(BITMAPINFOHEADER));
}

CGeometricTransformation::CGeometricTransformation()
{
	Init();
}

CGeometricTransformation::~CGeometricTransformation()
{
	Clear();
}

VOID CGeometricTransformation::BitMove(INT nOffsetX, INT nOffsetY)
{
	INT nBitmapLineLen = 0;
	INT nBitCount = 0;
	INT nHeight = 0;
	INT nWidth = 0;
	INT nBit = 0;

	//��ȡλͼ����
	GetBitBuffer();
	if (!m_pBit || !m_pNewBit)
	{
		return;
	}

	//��ȡλͼ��Ϣͷ
	GetBitInfo();

	//��ȡλͼһ�еĴ�С
	nBitmapLineLen = (m_stBitInfo.biWidth*m_stBitInfo.biBitCount / 8 + 3) / 4 * 4;

	//ÿ�������ֽ���
	nBitCount = m_stBitInfo.biBitCount / 8;

	//�߶�����
	for (nHeight = 0; nHeight < m_stBitInfo.biHeight; nHeight++)
	{
		//�������
		for (nWidth = 0; nWidth < m_stBitInfo.biWidth; nWidth++)
		{
			//�жϵ��Ƿ��ڷ�Χ�ڣ����ڵĻ��Ͳ����д���
			if (nHeight - nOffsetY >= NULL &&
				nHeight - nOffsetY < m_stBitInfo.biHeight &&
				nWidth - nOffsetX >= NULL &&
				nWidth - nOffsetX < m_stBitInfo.biWidth)
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

					*(m_pNewBit +
						nHeight * nBitmapLineLen +
						nWidth * nBitCount +
						nBit)
						=
						*(m_pBit +
						(nHeight - nOffsetY)*nBitmapLineLen +
							(nWidth - nOffsetX)*nBitCount +
							nBit);
				}
			}
		}
	}
}

VOID CGeometricTransformation::Zoom(double dX, double dY)
{
	INT nBitmapLineLen = 0;
	INT nBitCount = 0;
	INT nHeight = 0;
	INT nWidth = 0;
	INT nBit = 0;

	INT nBitHeight = 0;
	INT nBitWidth = 0;
	INT nBitmapLineLenOut = 0;
	INT nCoordinateX = 0;
	INT nCoordinateY = 0;

	//��ȡλͼ����
	GetBitBuffer();
	if (!m_pBit || !m_pNewBit)
	{
		return;
	}

	//��ȡλͼ��Ϣͷ
	GetBitInfo();

	nBitHeight = (INT)(m_stBitInfo.biHeight*dY + 0.5);
	nBitWidth = (INT)(m_stBitInfo.biWidth*dX + 0.5);

	//��ȡλͼһ�еĴ�С
	nBitmapLineLen = (m_stBitInfo.biWidth*m_stBitInfo.biBitCount / 8 + 3) / 4 * 4;
	nBitmapLineLenOut = (nBitWidth*m_stBitInfo.biBitCount / 8 + 3) / 4 * 4;;

	//ÿ�������ֽ���
	nBitCount = m_stBitInfo.biBitCount / 8;

	for (nHeight = 0; nHeight < nBitHeight; nHeight++)
	{
		for (nWidth = 0; nWidth < nBitWidth; nWidth++)
		{
			nCoordinateX = nWidth / dX + 0.5;
			nCoordinateY = nHeight / dY + 0.5;
			if (0 <= nCoordinateX &&
				nCoordinateX < m_stBitInfo.biWidth &&
				nCoordinateY >= 0 &&
				nCoordinateY < m_stBitInfo.biHeight)
			{
				for (nBit = 0; nBit < nBitCount; nBit++)
				{
					*(m_pNewBit +
						nHeight * nBitmapLineLen +
						nWidth * nBitCount + nBit)
						=
						*(m_pBit +
							nCoordinateY * nBitmapLineLen +
							nCoordinateX * nBitCount +
							nBit);
				}
			}
			else
			{
				for (nBit = 0; nBit < nBitCount; nBit++)
				{
					*(m_pNewBit +
						nHeight * nBitmapLineLen +
						nWidth * nBitCount +
						nBit) = 0;
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
	GetBitBuffer();
	if (!m_pBit || !m_pNewBit)
	{
		return;
	}

	//��ȡλͼ��Ϣͷ
	GetBitInfo();

	//��ȡλͼһ�еĴ�С
	nBitmapLineLen = (m_stBitInfo.biWidth*m_stBitInfo.biBitCount / 8 + 3) / 4 * 4;

	//ÿ�������ֽ���
	nBitCount = m_stBitInfo.biBitCount / 8;

	//�߶�����
	for (nHeight = 0; nHeight < m_stBitInfo.biHeight; nHeight++)
	{
		//�������
		for (nWidth = 0; nWidth < m_stBitInfo.biWidth; nWidth++)
		{
			//ÿ������ͨ��
			for (nBit = 0; nBit < nBitCount; nBit++)
			{
				*(m_pNewBit +
					nHeight * nBitmapLineLen +
					nWidth * nBitCount +
					nBit)
					=
					*(m_pBit +
						nHeight * nBitmapLineLen +
						(m_stBitInfo.biWidth - 1 - nWidth)*nBitCount +
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
	GetBitBuffer();
	if (!m_pBit || !m_pNewBit)
	{
		return;
	}

	//��ȡλͼ��Ϣͷ
	GetBitInfo();

	//��ȡλͼһ�еĴ�С
	nBitmapLineLen = (m_stBitInfo.biWidth*m_stBitInfo.biBitCount / 8 + 3) / 4 * 4;

	//ÿ�������ֽ���
	nBitCount = m_stBitInfo.biBitCount / 8;

	//�߶�����
	for (nHeight = 0; nHeight < m_stBitInfo.biHeight; nHeight++)
	{
		//�������
		for (nWidth = 0; nWidth < m_stBitInfo.biWidth; nWidth++)
		{
			//ÿ������ͨ��
			for (nBit = 0; nBit < nBitCount; nBit++)
			{
				*(m_pNewBit +
					nHeight * nBitmapLineLen +
					nWidth * nBitCount +
					nBit)
					=
					*(m_pBit +
					(m_stBitInfo.biHeight - 1 - nHeight)*nBitmapLineLen +
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
	GetBitBuffer();
	if (!m_pBit || !m_pNewBit)
	{
		return;
	}

	//��ȡλͼ��Ϣͷ
	GetBitInfo();

	//��ȡλͼһ�еĴ�С
	nBitmapLineLen = (m_stBitInfo.biWidth*m_stBitInfo.biBitCount / 8 + 3) / 4 * 4;

	//ÿ�������ֽ���
	nBitCount = m_stBitInfo.biBitCount / 8;

	//������Ҫ��ת���������ԱȽ����⣬һ����Ҫ�ѿ�Ⱥ͸߶Ȼ���ת��
	//����������εĻ����ǾͲ���Ҫ�ٴν��д�����
	if (m_stBitInfo.biHeight != m_stBitInfo.biWidth)
	{
		nReHeight = m_stBitInfo.biWidth;
		nReWidth = m_stBitInfo.biHeight;

		//��ȡһ�еĴ�С
		nBitmapLineLenOut = (nReWidth*m_stBitInfo.biBitCount / 8 + 3) / 4 * 4;

		//�ͷ��ڴ�
		delete[] m_pNewBit;
		m_pNewBit = new BYTE[nBitmapLineLenOut*nReHeight];
		if (m_pNewBit == nullptr)
		{
			return;
		}
		memset(m_pNewBit, 0, nBitmapLineLenOut*nReHeight);
	}
	else
	{
		nReHeight = m_stBitInfo.biHeight;
		nReWidth = m_stBitInfo.biWidth;
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
				*(m_pNewBit +
					nHeight * nBitmapLineLenOut +
					nWidth * nBitCount +
					nBit) 
					= 
					*(m_pBit +
						nWidth * nBitmapLineLen +
						(m_stBitInfo.biWidth - 1 - nHeight)*nBitCount +
						nBit);
			}
		}
	}
}

VOID CGeometricTransformation::BitDraw(HDC hdc, 
	INT nBeginX /*= NULL*/, 
	INT nBeginY /*= NULL*/, 
	LPBITMAPINFOHEADER lpBmpHeadInfo /*= nullptr*/,
	LPBYTE BmpData /*= nullptr*/)
{
	CBitmapHandle::BitDraw(hdc, nBeginX, nBeginY, &m_stBitInfo, m_pNewBit);
}
